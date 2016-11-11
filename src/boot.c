#define __NOLIBBASE__

#include <proto/exec.h>
#include <proto/expansion.h>

#include <exec/libraries.h>
#include <libraries/expansion.h>
#include <dos/dos.h>

#include "compiler.h"
#include "debug.h"

static const char conName[] = "UCON";
static const ULONG paramPkt[] = {
  (ULONG)conName,
  0,
  0,
  0,
  0
};

typedef int (*main_func_t)(void);

extern int ConMain(void);

struct fake_seg_list {
  ULONG size;
  BPTR next;
  UWORD jump_opcode;
  APTR func;
};

static BPTR create_fake_seg_list(struct ExecBase *SysBase, main_func_t func)
{
  struct fake_seg_list *seg = AllocMem(sizeof(*seg), MEMF_CLEAR);
  if(seg == NULL) {
    D(("no seglist!\n"));
    return 0;
  }
  D(("got seg list: @%lx -> func @%lx\n", seg, func));

  seg->size = sizeof(*seg);
  seg->next = (BPTR)0;
  seg->jump_opcode = 0x4ef9;
  seg->func = (APTR)func;

  return MKBADDR(&seg->next);
}

BOOL boot_init(struct ExecBase *SysBase)
{
  BOOL ok = FALSE;
  struct Library *ExpansionBase;
  BPTR seglist;

  seglist = create_fake_seg_list(SysBase, ConMain);
  if(seglist == NULL) {
    return ok;
  }

  ExpansionBase = (struct Library *)OpenLibrary("expansion.library", 36);
  if(ExpansionBase != NULL) {
    struct DeviceNode *dn = MakeDosNode(paramPkt);
    D(("got dev node=%08lx\n", dn));
    if(dn != NULL) {
      dn->dn_StackSize = 3000;
      dn->dn_SegList = seglist;
      dn->dn_Startup = (BPTR)0;
      dn->dn_Priority = 5;
      dn->dn_GlobalVec = (BPTR)-1;

      ok = AddBootNode( -128, 0, dn, NULL );
      D(("add boot node=%d\n", ok));
    }
    CloseLibrary(ExpansionBase);
  }
  return ok;
}
