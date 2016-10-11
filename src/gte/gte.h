#ifndef __GTE_H
#define __GTE_H

#include "GTEMAC.H"
#include "inline.h"

#define FOCAL_HEIGHT(x)				(int)((x)*8.88f)

#define GTEFLG_UNUSED				0x00000FFF	// 0-11
#define GTEFLG_IR0_SATURATE			0x00001000	// 12 IR0 saturated to +0000h..+1000h
#define GTEFLG_SY2_SATURATE			0x00002000	// 13 SY2 saturated to -0400h..+03FFh
#define GTEFLG_SX2_SATURATE			0x00004000	// 14 SX2 saturated to -0400h..+03FFh
#define GTEFLG_MAC0_NEGATIVE		0x00008000	// 15 MAC0 Result larger than 31 bits and negative
#define GTEFLG_MAC0_POSITIVE		0x00010000	// 16 MAC0 Result larger than 31 bits and positive
#define GTEFLG_DIV_OVERFLOW			0x00020000	// 17 Divide overflow. RTPS/RTPT division result saturated to max=1FFFFh
#define GTEFLG_SZ3_OTZ_SATURATE		0x00040000	// 18 SZ3 or OTZ saturated to +0000h..+FFFFh
#define GTEFLG_FIFO_B_SATURATE		0x00080000	// 19 Color-FIFO-B saturated to +00h..+FFh
#define GTEFLG_FIFO_G_SATURATE		0x00100000	// 20 Color-FIFO-G saturated to +00h..+FFh
#define GTEFLG_FIFO_R_SATURATE		0x00200000	// 21 Color-FIFO-R saturated to +00h..+FFh
#define GTEFLG_IR3_SATURATE			0x00400000	// 22 IR3 saturated to +0000h..+7FFFh (lm=1) or to -8000h..+7FFFh (lm=0)
#define GTEFLG_IR2_SATURATE			0x00800000	// 23 IR2 saturated to +0000h..+7FFFh (lm=1) or to -8000h..+7FFFh (lm=0)
#define GTEFLG_IR1_SATURATE			0x01000000	// 24 IR1 saturated to +0000h..+7FFFh (lm=1) or to -8000h..+7FFFh (lm=0)
#define GTEFLG_MAC3_LARGE_NEG		0x02000000	// 25 MAC3 Result larger than 43 bits and negative
#define GTEFLG_MAC2_LARGE_NEG		0x04000000	// 26 MAC2 Result larger than 43 bits and negative
#define GTEFLG_MAC1_LARGE_NEG		0x08000000	// 27 MAC1 Result larger than 43 bits and negative
#define GTEFLG_MAC3_LARGE_POS		0x10000000	// 28 MAC3 Result larger than 43 bits and positive
#define GTEFLG_MAC2_LARGE_POS		0x20000000	// 29 MAC2 Result larger than 43 bits and positive
#define GTEFLG_MAC1_LARGE_POS		0x40000000	// 30 MAC1 Result larger than 43 bits and positive
#define GTEFLG_ERROR				0x80000000	// 31 Bits 30-23 and 18-13 ORed together

#define GTE_SUB		(GTEFLG_MAC1_LARGE_POS|GTEFLG_MAC2_LARGE_POS|GTEFLG_MAC3_LARGE_POS|GTEFLG_MAC1_LARGE_NEG|GTEFLG_MAC2_LARGE_NEG|GTEFLG_MAC3_LARGE_NEG|\
	GTEFLG_IR1_SATURATE|GTEFLG_IR2_SATURATE|GTEFLG_SZ3_OTZ_SATURATE|GTEFLG_MAC0_POSITIVE|GTEFLG_MAC0_NEGATIVE|GTEFLG_SX2_SATURATE|GTEFLG_SY2_SATURATE)

enum
{
	LM_FALL,
	LM_OMNI
};

#define LIGHT_MAX		3

// light source attributes
typedef struct tagGteLight
{
	u8 Mag;						// scaling
	u8 Mode[LIGHT_MAX];			// mode for each light source
	u8 Col[LIGHT_MAX][3];		// light color
	u8 Ambient[3];				// ambient color
	u16 Range[LIGHT_MAX];		// intensity for fall off lights
	s32 Pos[LIGHT_MAX][3];		// position of sources
} GTE_LIGHT, GTE_LIT;

// camera vectors
typedef struct tagGteCamera
{
	s32 Look[3],	// where the camera eye is
		At[3];		// where the camera is looking at
} GTE_CAMERA, GTE_CAM;

// the whole GTE environment
typedef struct
{
	// gte attributes
	u16 Ambient[3],			// ambient color
		Fog_color[3];		// fade color for fog
	u16 H,					// focal height
		Fog_near,			// fog effect ranges
		Fog_far;
	// various work matrices
	MATRIX M_view,			// view matrix
		M_ll,				// local light
		M_lc;				// local color
	// other data
	GTE_LIT *pLight;		// light attribute pointer, used to rebuild light matrices
	GTE_CAM *pCamera;		// camera pointer
} GTE_ENV;

extern GTE_ENV Gte;
extern MATRIX M_id;

// macro to set up matrices with aligned writes
#define setRotM(_m,m00,m01,m02,m10,m11,m12,m20,m21,m22)		*(u32*)&(_m)->m[0][0]=(m00)|((m01)<<16),\
	*(u32*)&(_m)->m[0][2]=(m02)|((m10)<<16),\
	*(u32*)&(_m)->m[1][1]=(m11)|((m12)<<16),\
	*(u32*)&(_m)->m[2][0]=(m20)|((m21)<<16),\
	*(u32*)&(_m)->m[2][2]=(m22)

#define setTransM(_m,t0,t1,t2)								(_m)->t[0]=t0, (_m)->t[1]=t1, (_m)->t[2]=t2

#define setIdentityM(_m)									setRotM(_m, ONE,0,0, 0,ONE,0, 0,0,ONE), setTransM(_m,0,0,0)

void Gte_set_light(const VECTOR *pos, const GTE_LIGHT *l);
void Gte_camera_set();

void Gte_rot_svecs(SVECTOR *dir, SVECTOR *in, SVECTOR *out, int count);

#endif
