/* ================================================ */
/* TM2: simplified TMD revision code				*/
/* ================================================ */
#include "..\core.h"

u8* AllocatePool(ENTITY *ent, TM2_HEADER *mdl, u8* pool)
{
	int i;
	u32 s_tri, s_quad;
	u8 *data = (u8*)&mdl->obj[0];

	int remap = mdl->map_flg;
	// assign part allocation (also accounts for parts that are not in the skeleton)
	ent->Part_num = mdl->obj_cnt / 2;		// takes into account also invisible parts
	ent->pPart = (PARTS_W*)pool;
	pool = &pool[sizeof(PARTS_W)*ent->Part_num];
	memset(ent->pPart, 0, sizeof(PARTS_W)*ent->Part_num);

	// allocate each single part of the model
	for (i = 0; i<ent->Part_num; i++)
	{
		// map TM2 internal indices to RAM addresses
		// skip if this model is already mapped
		if (!remap)
		{
			mdl->obj[i].tri.vertex_offset	= (u32)&data[mdl->obj[i].tri.vertex_offset];
			mdl->obj[i].tri.normal_offset	= (u32)&data[mdl->obj[i].tri.normal_offset];
			mdl->obj[i].tri.tri_offset		= (u32)&data[mdl->obj[i].tri.tri_offset];
			mdl->obj[i].tri.tri_map_offset	= (u32)&data[mdl->obj[i].tri.tri_map_offset];

			mdl->obj[i].quad.vertex_offset	= (u32)&data[mdl->obj[i].quad.vertex_offset];
			mdl->obj[i].quad.normal_offset	= (u32)&data[mdl->obj[i].quad.normal_offset];
			mdl->obj[i].quad.quad_offset	= (u32)&data[mdl->obj[i].quad.quad_offset];
			mdl->obj[i].quad.quad_map_offset= (u32)&data[mdl->obj[i].quad.quad_map_offset];
		}

		s_tri = mdl->obj[i].tri.tri_count * sizeof(POLY_GT3);
		s_quad = mdl->obj[i].quad.quad_count * sizeof(POLY_GT4);

		// apply neutral diffuse
		ent->pPart[i].Poly_rgb	= MAKERGB(128, 128, 128);
		// set packet and TM2 pointers
		ent->pPart[i].pPacket	= (u32*)pool;
		ent->pPart[i].pPacket2	= (u32*)&pool[s_tri * 2];
		ent->pPart[i].pTmd		= (u32*)&mdl->obj[i].tri;
		ent->pPart[i].pTmd2		= (u32*)&mdl->obj[i].quad;
		setIdentityM(((MATRIX*)ent->pPart[i].M));

		// seek forward
		pool = &pool[(s_tri + s_quad) * 2];
	}

	// set model with data coming from the first joint
	ent->pPacket	= ent->pPart[0].pPacket;
	ent->pTmd		= ent->pPart[0].pTmd;
	ent->pPacket2	= ent->pPart[0].pPacket2;
	ent->pTmd2		= ent->pPart[0].pTmd2;

	// activate map flag
	mdl->map_flg = 1;

	return pool;
}

void MappingTm2(TM2_HEADER *mdl, PARTS_W *pPart, int count, int Tpage,
	int Clut)
{
	int i, j, cnt;
	// pools
	POLY_GT3 *t0, *t1;
	POLY_GT4 *q0, *q1;
	// mapping data
	TM2_TRIANGLE_MAP *tm;
	TM2_QUAD_MAP *qm;

	for (i = 0; i < count; i++)
	{
		cnt = mdl->obj[i].tri.tri_count;

		// define triangles
		t0 = (POLY_GT3*)pPart[i].pPacket;
		t1 = &t0[1];
		tm = (TM2_TRIANGLE_MAP*)mdl->obj[i].tri.tri_map_offset;

		// map all properties that are not going to change during rendering
		for (j = 0; j < cnt; j++, t0 += 2, t1 += 2, tm++)
		{
			t0->clut = t1->clut = tm->clut + (Clut << 6) - (240 << 6);
			t0->tpage = t1->tpage = tm->page + Tpage;
			// uv map
			*(u16*)&t0->u0 = *(u16*)&t1->u0 = *(u16*)&tm->u0;
			*(u16*)&t0->u1 = *(u16*)&t1->u1 = *(u16*)&tm->u1;
			*(u16*)&t0->u2 = *(u16*)&t1->u2 = *(u16*)&tm->u2;
		}

		cnt = mdl->obj[i].quad.quad_count;

		// define quads
		q0 = (POLY_GT4*)pPart[i].pPacket2;
		q1 = &q0[1];
		qm = (TM2_QUAD_MAP*)mdl->obj[i].quad.quad_map_offset;

		// map all properties that are not going to change during rendering
		for (j = 0; j < cnt; j++, q0 += 2, q1 += 2, qm++)
		{
			q0->clut = q1->clut = qm->clut + (Clut << 6) - (240 << 6);
			q0->tpage = q1->tpage = qm->page + Tpage;
			// uv map
			*(u16*)&q0->u0 = *(u16*)&q1->u0 = *(u16*)&qm->u0;
			*(u16*)&q0->u1 = *(u16*)&q1->u1 = *(u16*)&qm->u1;
			*(u16*)&q0->u2 = *(u16*)&q1->u2 = *(u16*)&qm->u2;
			*(u16*)&q0->u3 = *(u16*)&q1->u3 = *(u16*)&qm->u3;
		}
	}
}

void LinkBones(PARTS_W *pParent, PARTS_W *pNode, VECTOR *pPos)
{
	// parent present, hook work matrix as super
	if (pParent) pNode->pSuper = &pParent->Work;

	// void rotation
	setRotM((MATRIX*)pNode->M, ONE, 0, 0, 0, ONE, 0, 0, 0, ONE);
	// set position
	pNode->Pos_x = pPos->vx;
	pNode->Pos_y = pPos->vy;
	pNode->Pos_z = pPos->vz;
	// copy more
	pNode->Old_x = pPos->vx;
	pNode->Old_y = pPos->vy;
	pNode->Old_z = pPos->vz;
}
