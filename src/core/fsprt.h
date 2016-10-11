#ifndef __FSPRT_H
#define __FSPRT_H

#include "common.h"

// any size
typedef struct tagFastSprt
{
	u32 tag;
	u32 mode;
	u32 rgbc;
	u32 xy;
	u32 uvc;
	u32 wh;
} FAST_SPRT;

// 8x8 or 16x16
typedef struct tagFastSSprt
{
	u32 tag;
	u32 mode;
	u32 rgbc;
	u32 xy;
	u32 uvc;
} FAST_SPRT8, FAST_SPRT16;

// tagless sprite with tpage
typedef struct tagAlphaSprt8
{
	u32 mode;
	u32 rgbc;
	u32 xy;
	u32 uvc;
} ALPHA_SPRT8;

typedef struct tagFontPrim8
{
	u32 tag;
	ALPHA_SPRT8 sprt[2];
} ALPHA_PRIM8;

typedef struct tagAlphaSprt
{
	u32 mode;
	u32 rgbc;
	u32 xy;
	u32 uvc;
	u32 wh;
} ALPHA_SPRT;

typedef struct tagFontPrim
{
	u32 tag;
	ALPHA_SPRT sprt[2];
} ALPHA_PRIM;

typedef struct tagAlphaFT4b
{
	u32	rgb0c;
	u32 xy0;
	u32 uv0c;
	u32 xy1;
	u32 uv1p;
	u32 xy2;
	u32 uv2;
	u32 xy3;
	u32 uv3;
} ALPHA_FT4B;

typedef struct tagAlphaFT4
{
	u32 tag;
	ALPHA_FT4B poly[2];
} ALPHA_FT4;

// macros to build fast sprites
#define setFSprt(p)					(p)->tag=5<<24
#define setFSSprt(p)				(p)->tag=4<<24

#define setFSprtPage(p,tpage)		(p)->mode=(TAG_MODE<<24)|(tpage)
#define setFSSprtPage(p,tpage)		(p)->mode=(TAG_MODE<<24)|(tpage)

#define setFSprtTag(p,tpage)		(p)->tag=((p)->tag&0xFFFFFF)|(5<<24), (p)->mode=0xE1000000|(tpage)
#define setFSSprtTag(p,tpage)		(p)->tag=((p)->tag&0xFFFFFF)|(4<<24), (p)->mode=0xE1000000|(tpage)

#define setFSprtRGB(p,r,g,b)		(p)->rgbc=(TAG_SPRT<<24)  |((r)|((g)<<8)|((b)<<16))
#define setFSprtRGB8(p,r,g,b)		(p)->rgbc=(TAG_SPRT8<<24) |((r)|((g)<<8)|((b)<<16))
#define setFSprtRGB16(p,r,g,b)		(p)->rgbc=(TAG_SPRT16<<24)|((r)|((g)<<8)|((b)<<16))

#define setFSprtXY(p,x,y)			(p)->xy=((x)&0xFFFF)|((y)<<16)
#define setFSprtUVClut(p,u,v,clut)	(p)->uvc=((u)|((v)<<8)|((clut)<<16))
#define setFSprtWH(p,w,h)			(p)->wh=(w)|((h)<<16)

// optimized addPrim macro
#define __addPrim(ot,p,len)			{ u32 *tag = (ot); (p)->tag = (*tag & 0x00FFFFFF) | ((len) << 24); *tag = (u32)(p) & 0x00FFFFFF; }
// optimized addPrim macros for each fast sprite type
#define addPrimSprt(ot,p)			__addPrim(ot,p,5)
#define addPrimSprt8(ot,p)			__addPrim(ot,p,4)
#define addPrimSprt16(ot,p)			__addPrim(ot,p,4)

// mapping data
typedef struct tagMapSprtX
{
	u32 mode;
	s16 x, y;
	u16 w, h;
	u8 u, v;
	u16 clut;
} MAP_SPRT;

void SortMapSprt(MAP_SPRT *sprt, int count, u32 *ot);

#endif
