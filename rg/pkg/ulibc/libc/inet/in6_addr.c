/* Copyright (C) 1997, 1998, 2002 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Philip Blundell <pjb27@cam.ac.uk>, 1997.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#define __FORCE_GLIBC
#include <features.h>
#include <netinet/in.h>

#ifdef __UCLIBC_HAS_IPV6__
const struct in6_addr in6addr_any =
{ { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 } } };
libc_hidden_proto(in6addr_loopback)
const struct in6_addr in6addr_loopback =
{ { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 } } };
libc_hidden_data_def(in6addr_loopback)
#endif /* __UCLIBC_HAS_IPV6__ */


