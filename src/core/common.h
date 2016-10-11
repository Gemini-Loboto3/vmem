#ifndef __COMMON_H
#define __COMMON_H

#define RES_W	320
#define RES_H	240

#define NULL	0

/* --------------------------------------------------------------------------
 * - All variable types														-
 * -------------------------------------------------------------------------- */
typedef unsigned char BYTE;
typedef unsigned short USHORT;
typedef unsigned long ULONG;
typedef void* LPVOID;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;

typedef signed char s8;
typedef signed short s16;
typedef signed long s32;
typedef signed long long s64;

#ifndef _WIN32
typedef int BOOL;
typedef int bool;
#endif

#define FALSE			0
#define TRUE			1

#define true			TRUE
#define false			FALSE

#ifndef _WIN32
#define _countof(argument) ( sizeof( argument ) / sizeof( *( argument ) ) )

#define offsetof(st, m) ((size_t)&(((st *)0)->m))
#endif

#define GetS8(ptr)		*((s8*)(ptr))
#define GetU8(ptr)		*((u8*)(ptr))
#define GetS16(ptr)		*((s16*)(ptr))
#define GetU16(ptr)		*((u16*)(ptr))
#define GetS32(ptr)		*((s32*)(ptr))
#define GetU32(ptr)		*((u32*)(ptr))

// allows something like a Mixin type to find the structure that contains it
#define container_of(ptr, type, member) ({ \
	const typeof(((type*)0)->member) *__mptr=(ptr); \
	(type*)((char*)__mptr - offsetof(type,member));})

#define Align(val,align)	if(val%align!=0) val+=align-(val%align);
#define Align16(val)		(((val)+1)&~1)
#define Align32(val)		(((val)+3)&~3)
#define GetAlign(val,align)	(((val)+(align-1))&~(align-1))

#define MAKERGB(r,g,b)		((r)|((g)<<8)|((b)<<16))
#define MAGIC(a,b,c,d)		((a)|((b)<<8)|((c)<<16)|((d)<<24))

#define BIT(x)				(1<<(x))
#define FIXED(x)			((int)((x)*4096.f))

#define DEG(x)				(int)((x)*4096.f/360.f)

// stack pointer optimizations with scratchpad
extern void Sp_to_scratch();
extern void Sp_restore();
// jump to whatever pointer
extern void Reboot(u32 ptr);

// incremental allocator starting point
// it's located right at the end of the main exe
extern u32 *__packable;

__inline static void* getPackable() { return (void*)__packable; }

#endif
