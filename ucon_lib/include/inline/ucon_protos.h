#ifndef _VBCCINLINE_UCON_H
#define _VBCCINLINE_UCON_H

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

LONG __UconInit(__reg("a6") struct UConResource *, __reg("d0") ULONG readSigMask, __reg("d1") struct Task * task)="\tjsr\t-6(a6)";
#define UconInit(readSigMask, task) __UconInit(UconBase, (readSigMask), (task))

VOID __UconExit(__reg("a6") struct UConResource *)="\tjsr\t-12(a6)";
#define UconExit() __UconExit(UconBase)

VOID __UconOpen(__reg("a6") struct UConResource *, __reg("d0") APTR fh, __reg("d1") BYTE * name, __reg("d2") LONG type)="\tjsr\t-18(a6)";
#define UconOpen(fh, name, type) __UconOpen(UconBase, (fh), (name), (type))

VOID __UconClose(__reg("a6") struct UConResource *, __reg("d0") APTR fh)="\tjsr\t-24(a6)";
#define UconClose(fh) __UconClose(UconBase, (fh))

VOID __UconRead(__reg("a6") struct UConResource *, __reg("d0") UBYTE * buffer, __reg("d1") LONG length, __reg("d2") LONG * result)="\tjsr\t-30(a6)";
#define UconRead(buffer, length, result) __UconRead(UconBase, (buffer), (length), (result))

LONG __UconWrite(__reg("a6") struct UConResource *, __reg("d0") UBYTE * buffer, __reg("d1") LONG length)="\tjsr\t-36(a6)";
#define UconWrite(buffer, length) __UconWrite(UconBase, (buffer), (length))

#endif /*  _VBCCINLINE_UCON_H  */
