#include <exec/types.h>
#include <exec/libraries.h>
#include <exec/resident.h>

#include <proto/exec.h>
#include <proto/expansion.h>

#include "compiler.h"
#include "debug.h"
#include "boot.h"

#define VERS        MY_NAME " " MY_VERSION_STR
#define VSTRING     MY_NAME " " MY_VERSION_STR " (" MY_DATE ")\r\n"
#define VERSTAG     "\0$VER: " MY_NAME " " MY_VERSION_STR " (" MY_DATE ")"

static const char UserLibName[] = MY_NAME;
static const char UserLibVer[]  = VSTRING;

/* Setup Function */
SAVEDS ASM static APTR InitFunc(REG(d0, struct Library *lib_base),
                                REG(a0, BPTR seg_list),
                                REG(a6, struct ExecBase *exec_base))
{
  D(("+InitFunc\n"));
  boot_init(exec_base);
  D(("-InitFunc\n"));
  return (APTR)TRUE;
}

/* ---- RomTag ---- */
static const struct Resident ROMTag =
{
  RTC_MATCHWORD,
  (struct Resident *)&ROMTag,
  (struct Resident *)&ROMTag + 1,
  RTF_AFTERDOS,
  MY_VERSION,
  NT_RESOURCE,
  -124, /* prio */
  (APTR)UserLibName,
  (APTR)UserLibVer,
  (APTR)InitFunc
};
