
/* VBCC */
void __KPutCh(__reg("a6") void *, __reg("d0") UBYTE ch) = "\tjsr\t-516(a6)";
#define KPutCh(ch) __KPutCh(SysBase, ch)


#ifdef DEBUG

extern void KPrintF(char *, ...);
#define D(x) KPrintF x ;

#else

#define D(x)

#endif
