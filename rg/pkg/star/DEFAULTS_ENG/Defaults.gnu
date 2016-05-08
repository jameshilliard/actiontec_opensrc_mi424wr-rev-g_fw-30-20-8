#ident "%W% %E% %Q%"
###########################################################################
#
# global definitions for GNU (hurd) Systems
#
###########################################################################
#
# Compiler stuff
#
###########################################################################
#DEFCCOM=	cc
DEFCCOM=	gcc
###########################################################################
#
# If the next line is commented out, compilattion is done with max warn level
# If the next line is uncommented, compilattion is done with minimal warnings
#
###########################################################################
#CWARNOPTS=

DEFINCDIRS=	$(SRCROOT)/include /usr/src/linux/include
LDPATH=		-L/opt/schily/lib
RUNPATH=	-R $(INS_BASE)/lib -R /opt/schily/lib -R $(OLIBSDIR)

###########################################################################
#
# Installation config stuff
#
###########################################################################
#INS_BASE=	/opt/schily
#INS_KBASE=	/
INS_BASE=	/tmp/schily
INS_KBASE=	/tmp/schily/root
#
DEFUMASK=	002
#
DEFINSMODEF=	444
DEFINSMODEX=	755
DEFINSUSR=	bin
DEFINSGRP=	bin
