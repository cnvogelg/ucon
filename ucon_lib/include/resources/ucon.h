#ifndef RESOURCES_UCON_H
#define RESOURCES_UCON_H

/*
** The UCon Resource
*/

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifndef EXEC_LIBRARIES_H
#include <exec/libraries.h>
#endif

struct UconResource {
  struct Library    ur_Library;
};

#define UCONNAME    "ucon.resource"

#endif /* RESOURCES_UCON_H */
