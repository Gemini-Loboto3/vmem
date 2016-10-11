#ifndef __TM2_H
#define __TM2_H

typedef struct tagTm2Triangle
{
	u16 n0, v0;
	u16 n1, v1;
	u16 n2, v2;
} TM2_TRIANGLE;

// for both tris and quads
// 16 bytes to access with rapid "x << 4"
typedef struct tagTm2PolyColor
{
	u8 r0, g0, b0, pad0;
	u8 r1, g1, b1, pad1;
	u8 r2, g2, b2, pad2;
	u8 r3, g3, b3, pad3;
} TM2_POLY_COLOR;

typedef struct tagTm2Quad
{
	u16 n0, v0;
	u16 n1, v1;
	u16 n2, v2;
	u16 n3, v3;
} TM2_QUAD;

typedef struct tagTm2TriangleMap
{
	u8 u0, v0; u16 clut;			// 0-1-2
	u8 u1, v1; u16 page;			// 4-5-6
	u8 u2, v2; u16 zero;			// 8-9-A
} TM2_TRIANGLE_MAP, TM2_TRI_MAP;

typedef struct tagTm2QuadMap
{
	u8 u0, v0; u16 clut;			// 0-1-2
	u8 u1, v1; u16 page;			// 4-5-6
	u8 u2, v2; u16 zero1;			// 8-9-A
	u8 u3, v3; u16 zero2;			// C-D-E
} TM2_QUAD_MAP;

typedef struct tagTm2Triangles
{
	u32 vertex_offset;		// offset to vertex data
	u32 vertex_count;		// vertex count
	u32 normal_offset;		// offset to normal data
	u32 normal_count;		// normal count
	u32 tri_offset;			// offset to triangle data
	u32 tri_count;			// triangle count
	u32 tri_map_offset;		// Offset to triangle texture data
} TM2_TRIANGLES;

typedef struct tagTm2Quads
{
	u32 vertex_offset;		// offset to vertex data
	u32 vertex_count;		// vertex count
	u32 normal_offset;		// offset to normal data
	u32 normal_count;		// normal count
	u32 quad_offset;		// Offset to quad index data
	u32 quad_count;			// quad count
	u32 quad_map_offset;	// offset to quad texture data
} TM2_QUADS;

typedef struct tagTm2Object
{
	TM2_TRIANGLES tri;
	TM2_QUADS quad;
} TM2_OBJECT;

typedef struct tagTm2Header
{
	u32 length;				// section length in bytes
	u32 map_flg;			// 1=pointers have been updated to RAM addresses
	int obj_cnt;			// number of objects in model
	TM2_OBJECT obj[1];
} TM2_HEADER;

#define FLGENT_ACTIVE		BIT(0)

#define FLGPRT_ACTIVE		BIT(0)
#define FLGPRT_TRANS		BIT(1)		// enable semi transparency
#define FLGPRT_CHROME		BIT(2)		// use chrome effect to render
#define FLGPRT_COLOR		BIT(3)		// use diffuse attributes to draw
#define FLGPRT_PARTICLE		BIT(4)		// render as particles
#define FLGPRT_SCALE		BIT(5)		// use Scale_xyz to scale part

////////////////////////////////////////
typedef struct tagPart
{
	u32 Flg;		// attribute flags
	u32 Poly_rgb;	// diffuse attributes
	// explicit matrix for skeletal data
	s16 M[3][3];	// rotations
	u16 pad0;		// padding in case GCC fails
	s32 Pos_x,		// positions
		Pos_y,
		Pos_z;
	// matrices
	MATRIX Work,	// transformed matrix
		*pSuper;	// parent
					// TM2 pointers and packets
	u32 *pTmd,		// triangles
		*pPacket;
	u32 *pTmd2,		// quadrilaterals
		*pPacket2;
	// rotation data (can typecast to SVECTOR)
	s16 Rot_x,
		Rot_y,
		Rot_z;
	// position copy
	s16 Old_x,
		Old_y,
		Old_z;
	// scaling
	u16 Scale_x,
		Scale_y,
		Scale_z;
} PARTS_W;

typedef struct tagEntity
{
	u32 Flg;		// attribute flags
	// allocators
	u16 Tpage,
		Clut;
	// explicit matrix for skeletal data
	s16 M[3][3];	// rotations
	u16 pad0;		// padding in case GCC fails
	s32 Pos_x,		// positions
		Pos_y,
		Pos_z;
	// matrices
	MATRIX Work,	// transformed matrix
		*pSuper;	// parent
	// TM2 pointers and packets
	u32 *pTmd,		// triangles
		*pPacket;
	u32 *pTmd2,		// quadrilaterals
		*pPacket2;
	// rotation data (can typecast to SVECTOR)
	s16 Rot_x,
		Rot_y,
		Rot_z;
	u16 Part_num;
	PARTS_W *pPart;
} ENTITY;

u8* AllocatePool(ENTITY *ent, TM2_HEADER *mdl, u8* pool);
void MappingTm2(TM2_HEADER *mdl, PARTS_W *pPart, int count, int Tpage, int Clut);

void LinkBones(PARTS_W *pParent, PARTS_W *pNode, VECTOR *pPos);

#endif
