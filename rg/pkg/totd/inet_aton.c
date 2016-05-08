/* 	$Id: inet_aton.c,v 1.1.6.2 2012/02/10 06:32:41 simba Exp $	*/

#include "totd.h"

#if !HAVE_INET_ATON
int inet_aton(const char *cp, struct in_addr *inp)
{
  in_addr_t lala;

  if((lala = inet_addr(cp)) < 0)
    return(0);
  inp->s_addr = lala;
  return(1);
}
#endif
