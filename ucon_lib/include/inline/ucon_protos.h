#ifndef _VBCCINLINE_UCON_H
#define _VBCCINLINE_UCON_H

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

LONG __UconOpen(__reg("a6") struct UConResource *, __reg("d0") ULONG readSigMask, __reg("d1") struct Task * task)="\tjsr\t-6(a6)";
#define UconOpen(readSigMask, task) __UconOpen(UconBase, (readSigMask), (task))

VOID __UconClose(__reg("a6") struct UConResource *)="\tjsr\t-12(a6)";
#define UconClose() __UconClose(UconBase)

LONG __UconRead(__reg("a6") struct UConResource *, __reg("d0") UBYTE * buffer, __reg("d1") LONG length)="\tjsr\t-18(a6)";
#define UconRead(buffer, length) __UconRead(UconBase, (buffer), (length))

LONG __UconWrite(__reg("a6") struct UConResource *, __reg("d0") UBYTE * buffer, __reg("d1") LONG length)="\tjsr\t-24(a6)";
#define UconWrite(buffer, length) __UconWrite(UconBase, (buffer), (length))

#endif /*  _VBCCINLINE_UCON_H  */
