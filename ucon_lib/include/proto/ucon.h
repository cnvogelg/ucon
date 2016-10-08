#ifndef _PROTO_UCON_H
#define _PROTO_UCON_H

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#if !defined(CLIB_UCON_PROTOS_H) && !defined(__GNUC__)
#include <clib/ucon_protos.h>
#endif

#ifndef __NOLIBBASE__
extern struct Library *UconBase;
#endif

#ifdef __GNUC__
#ifdef __AROS__
#include <defines/ucon.h>
#else
#include <inline/ucon.h>
#endif
#elif defined(__VBCC__)
#include <inline/ucon_protos.h>
#else
#include <pragma/ucon_lib.h>
#endif

#endif	/*  _PROTO_UCON_H  */
