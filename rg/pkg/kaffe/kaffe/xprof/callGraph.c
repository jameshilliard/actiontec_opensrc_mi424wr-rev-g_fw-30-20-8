/*
 * callGraph.c
 * Routines for tracking the call graph of jitted code
 *
 * Copyright (c) 2000, 2001, 2003 University of Utah and the Flux Group.
 * All rights reserved.
 *
 * This file is licensed under the terms of the GNU Public License.
 * See the file "license.terms" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 * Contributed by the Flux Research Group, Department of Computer Science,
 * University of Utah, http://www.cs.utah.edu/flux/
 */

#include "config.h"

#if defined(KAFFE_XPROFILER)

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/gmon.h>

#include "callGraph.h"
#include "jthread.h"
#include "xprofile-md.h"

/*
 * Hash function for an arc, obviously we need something that will spread them
 * out quite a bit.
 */
#define HASH_ARC(frompc, selfpc) \
	((((unsigned long)(frompc)) + ((unsigned long)(selfpc))) \
	 % CALL_GRAPH_TABLE_SIZE)

struct call_graph *createCallGraph(int count)
{
	struct call_graph *retval;
	struct call_arc *arcs;

	assert(count > 0);
	
	/*
	 * Allocate the arcs and the root structure, this needs to be executed
	 * early on since we use malloc.
	 */
	if( (retval = (struct call_graph *)
	     malloc(sizeof(struct call_graph))) &&
	    (arcs = (struct call_arc *)
	     malloc(sizeof(struct call_arc) *
		    (count + 8 /* Extra padding to be safe */))) )
	{
		int lpc;

		/* Initialize everything */
		retval->cg_misses = 0;
		retval->cg_free = 0;
		retval->cg_count = count;
		retval->cg_arcs = arcs;
		for( lpc = 0; lpc < CALL_GRAPH_TABLE_SIZE; lpc++ )
		{
			retval->cg_table[lpc] = 0;
		}
		for( lpc = 0; lpc < (count + 8); lpc++ )
		{
			arcs[lpc].ca_count = 0;
		}
	}
	else
	{
		free(retval);
		retval = 0;
	}
	return( retval );
}

void deleteCallGraph(struct call_graph *cg)
{
	if( cg )
	{
		free(cg->cg_arcs);
		free(cg);
	}
}

void resetCallGraph(struct call_graph *cg)
{
	int lpc;

	assert(cg != 0);
	
	cg->cg_free = 0;
	cg->cg_misses = 0;
	/* Unlink the arcs */
	for( lpc = 0; lpc < CALL_GRAPH_TABLE_SIZE; lpc++ )
		cg->cg_table[lpc] = 0;
}

void arcHit(struct call_graph *cg, char *frompc, char *selfpc)
{
	struct call_arc *ca;
	int _index;

	/*
	 * NOTE: This function isn't thread-safe since its would slow us down,
	 * quite a bit.  Instead we just try to be resistant to any races
	 * that might occur.
	 */
	
	/* Get the index in the hash table for this arc */
	_index = HASH_ARC(frompc, selfpc);
	ca = cg->cg_table[_index];
	/* Walk over the arcs in the table searching for a match */
	while( ca )
	{
		if( (ca->ca_from == frompc) && (ca->ca_to == selfpc) )
		{
			/* Found a match, increase the count */
			ca->ca_count++;
			break;
		}
		ca = ca->ca_next;
	}
	if( !ca )
	{
		/*
		 * No call_arc struct was found so this is the first time we've
		 * seen this arc, get a new one and add it in.
		 */
		if( cg->cg_free < cg->cg_count )
		{
			ca = &cg->cg_arcs[cg->cg_free++];
			ca->ca_count = 1;
			ca->ca_next = cg->cg_table[_index];
			cg->cg_table[_index] = ca;
			ca->ca_from = frompc;
			ca->ca_to = selfpc;
		}
		else
		{
			/* No more free call_arcs, record the miss */
			cg->cg_misses++;
		}
	}
}

int writeCallGraph(struct call_graph *cg, struct gmon_file *gf)
{
#if defined(KAFFE_CPROFILER)
	struct gmonparam *gp = getGmonParam();
	int from_len;
#endif
	int retval = 1, lpc;

	assert(cg != 0);
	assert(gf != 0);
	
	/*
	 * Walk over all of the arcs and output any that are non zero.
	 *
	 * NOTE: We're walking over the set of allocated arcs since the
	 * cg_table might've dropped some of the arcs due to a race with
	 * another thread.
	 */
	for( lpc = 0; (lpc < cg->cg_free) && retval; lpc++ )
	{
		if( cg->cg_arcs[lpc].ca_count )
		{
			struct call_arc *ca = &cg->cg_arcs[lpc];

			/* Make sure its a valid arc */
			if( (ca->ca_from < gf->gf_low) ||
			    (ca->ca_to < gf->gf_low) ||
			    (ca->ca_from > gf->gf_high) ||
			    (ca->ca_to > gf->gf_high) )
			{
#if defined(KAFFE_VMDEBUG)
			    /* FIXME: add debug code like Janos VM. */
				dprintf("Out of bounds call arc "
					"%p -> %p : %d\n"
					"low: %p high: %p\n",
					ca->ca_from,
					ca->ca_to,
					ca->ca_count,
					gf->gf_low,
					gf->gf_high);
#endif
			} else
			retval = writeGmonRecord(gf,
						 GRA_Type, GMON_TAG_CG_ARC,
						 GRA_FromPC, ca->ca_from,
						 GRA_SelfPC, ca->ca_to,
						 GRA_Count, ca->ca_count,
						 GRA_DONE);
		}
	}

#if defined(KAFFE_CPROFILER)
	if( gp )
	{
		/*
		 * Here we write out any arcs that were generated by the C
		 * compiler's mcount.
		 */
		from_len = gp->fromssize / sizeof(*gp->froms);
		for( lpc = 0; (lpc < from_len) && retval; lpc++ )
		{
			if( gp->froms[lpc] )
			{
				char *frompc;
				int to_index;
				
				frompc = (char *)(gp->lowpc +
						  (lpc * gp->hashfraction *
						   sizeof(*gp->froms)));
				for( to_index = gp->froms[lpc];
				     to_index != 0;
				     to_index = gp->tos[to_index].link )
				{
					retval = writeGmonRecord(
						gf,
						GRA_Type, GMON_TAG_CG_ARC,
						GRA_FromPC, frompc,
						GRA_SelfPC,
						gp->tos[to_index].selfpc,
						GRA_Count,
						gp->tos[to_index].count,
						GRA_DONE);
				}
			}
		}
	}
#endif
	return( retval );
}

#endif