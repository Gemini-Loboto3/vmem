#ifndef __CORE_H
#define __CORE_H

#ifdef _PSX
#undef _WIN32
#endif

#include <SYS\TYPES.H>
#include <STDLIB.H>
#include <STDIO.H>
#include <LIBSPU.H>
#include <LIBSND.H>
#include <LIBGTE.H>
#include <INLINE_C.H>
#include <LIBGPU.H>
#include <LIBPRESS.H>
#include <LIBETC.H>
#include <LIBCD.H>
#include <LIBPAD.H>
#include <LIBAPI.H>

#include "core\common.h"
#include "core\protocol.h"

#endif	// __CORE_H
