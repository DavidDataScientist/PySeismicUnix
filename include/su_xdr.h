/* Copyright (c) Colorado School of Mines, 2011.*/
/* All rights reserved.                       */

#ifndef SU_XDR
#define SU_XDR

#include <stdio.h>


/*
#ifdef SUXDR
#include <rpc/types.h>
#include <rpc/xdr.h>
#endif
*/

#ifdef SUXDR
#ifdef _WIN32
#include "win32_compat/rpc/types.h"
#include "win32_compat/rpc/xdr.h"
#elif defined(SUTIRPC)
#include <tirpc/rpc/types.h>
#include <tirpc/rpc/xdr.h>
#include <tirpc/netconfig.h>
#else
#include <rpc/types.h>
#include <rpc/xdr.h>
#endif
#endif



#include "su.h"
#include "segy.h"

#ifdef SUXDR
int xdrhdrsub(XDR *segyxdr, segy *tp);
int xdrbhdrsub(XDR *segyxdr, bhed *bhp);
#else
void xdrhdrsub();
void xdrbhdrsub();
#endif

#endif /* SU_XDR */ 

