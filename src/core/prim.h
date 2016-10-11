#ifndef __PRIM_H
#define __PRIM_H

// DR_MODE bits
#define DMODE_TRANS0				(0<<5)	// 50% back, 50% front
#define DMODE_TRANS1				(1<<5)	// additive
#define DMODE_TRANS2				(2<<5)
#define DMODE_TRANS3				(3<<5)	// subtractive
#define DMODE_4BIT					(0<<7)
#define DMODE_8BIT					(1<<7)
#define DMODE_15BIT					(2<<7)
#define DMODE_DITHER				(1<<9)
#define DMODE_OUT_AREA				(1<<10)	// drawing to display area (0: prohibited, 1: allowed)
#define DMODE_HFLIP					(1<<11)
#define DMODE_VFLIP					(1<<12)

// attributes
#define TAG_BLIT					0x01		// RGB values ignored
#define TAG_TRANS					0x02		// enable transparency
// polygons
#define TAG_PF3						0x20
#define TAG_PFT3					0x24
#define TAG_PF4						0x28
#define TAG_PFT4					0x2C
#define TAG_PG3						0x30
#define TAG_PGT3					0x34
#define TAG_PG4						0x38
#define TAG_PGT4					0x3C
// lines
#define TAG_LF2						0x40
#define TAG_LF3						0x48
#define TAG_LF4						0x4C
#define TAG_LG2						0x50
#define TAG_LG3						0x58
#define TAG_LG4						0x5C
// rectangles
#define TAG_TILE					0x60
#define TAG_TILE1					0x68
#define TAG_TILE8					0x70
#define TAG_TILE16					0x78
// sprites
#define TAG_SPRT					0x64
#define TAG_SPRT8					0x74
#define TAG_SPRT16					0x7C
// draw
#define TAG_MODE					0xE1	// draw mode
#define TAG_TWIN					0xE2	// texture window
#define TAG_STP						0xE6	// set framebuffer transparency
#define TAG_LOAD					0xA0	// ram -> vram
#define TAG_STORE		// vram -> ram
#define TAG_MOVE		// vram -> vram

#define TMODE_4BIT					0
#define TMODE_8BIT					1
#define TMODE_15BIT					2
#define TMODE_24BIT					3

#define AMODE_SEMI					0		// 0.5 x Back + 0.5 x Forward
#define AMODE_ADD					1		// 1.0 x Back + 1.0 x Forward
#define AMODE_SUB					2		// 1.0 x Back - 1.0 x Forward
#define AMODE_BLEND					3		// 1.0 x Back + 0.25 x Forward

#include "fsprt.h"

// various merged primitives
typedef struct tagFastPolyG4
{
	u32 tag;
	u32 mode;
	u32 rgb0c;
	short x0, y0;
	u8 u0, v0;	u16	clut;
	u32 rgb1;
	short x1, y1;
	u8 u1, v1;	u16	tpage;
	u32 rgb2;
	short x2, y2;
	u8	u2, v2;	u16	pad2;
	u32 rgb3;
	short x3, y3;
	u8	u3, v3;	u16	pad3;
} FPOLY_GT4;

typedef struct tagFastPolyGT4
{
	u32 tag;
	u32 mode;
	u32 rgb0c;
	short x0, y0;
	u32 rgb1;
	short x1, y1;
	u32 rgb2;
	short x2, y2;
	u32 rgb3;
	short x3, y3;
} FPOLY_G4;

typedef struct tagFastLineFT2
{
	u32 tag;
	u32 mode;
	u32 rgbc;
	s16 x0, y0;
	s16 x1, y1;
} FLINE_F2;

typedef struct tagFastTile
{
	u32 tag;			// 4<<24
	u32 mode;			// 0xE1<<24
	u32 rgbc;			// 0x60<<24
	u32 xy;
	u32 wh;
} FTILE;

// DR_MOVE explained instead of just a bunch of u32s
typedef struct tagDrMoveX
{
	u32 tag;	// always 5<<24
	u32 clear;	// always 1<<24 (cache command)
	u32 code;	// always 1<<31 (move command)
	u16 sx, sy;	// source coordinates
	u16 dx, dy;	// destination coordinates
	u32 wh;		// width & height
} DR_MOVEX;

typedef struct tagDrModeX
{
	u32 tag;
	u32 code;
} DR_MODEX;

#endif
