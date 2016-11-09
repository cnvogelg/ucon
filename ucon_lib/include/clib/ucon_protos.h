#ifndef CLIB_UCON_PROTOS_H
#define CLIB_UCON_PROTOS_H


/*
**	$VER: ucon_protos.h 0.1 (09.11.2016)
**
**	C prototypes. For use with 32 bit integers only.
**
**	Copyright © 2016 
**	All Rights Reserved
*/

#ifndef  RESOURCES_UCON_H
#include <resources/ucon.h>
#endif

LONG UconOpen(ULONG readSigMask, struct Task * task);
VOID UconClose(void);
LONG UconRead(UBYTE * buffer, LONG length);
LONG UconWrite(UBYTE * buffer, LONG length);

#endif	/*  CLIB_UCON_PROTOS_H  */
