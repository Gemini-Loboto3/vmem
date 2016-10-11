/* ================================================ */
/* Particle effects									*/
/* ================================================ */
#include "..\core.h"

typedef struct tagParticle
{
	u32 Flg;
	u32 *pPacket;
} PARTICLE;

typedef struct tagPrim
{
	u32 tag;

} PRIM;

u8* Particle_allocate(ENTITY *ent, TM2_HEADER *mdl, u8 *pool)
{
	int i;
	u32 s_part;
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
			mdl->obj[i].tri.vertex_offset = (u32)&data[mdl->obj[i].tri.vertex_offset];
			mdl->obj[i].tri.normal_offset = (u32)&data[mdl->obj[i].tri.normal_offset];
			mdl->obj[i].tri.tri_offset = (u32)&data[mdl->obj[i].tri.tri_offset];
			mdl->obj[i].tri.tri_map_offset = (u32)&data[mdl->obj[i].tri.tri_map_offset];

			mdl->obj[i].quad.vertex_offset = (u32)&data[mdl->obj[i].quad.vertex_offset];
			mdl->obj[i].quad.normal_offset = (u32)&data[mdl->obj[i].quad.normal_offset];
			mdl->obj[i].quad.quad_offset = (u32)&data[mdl->obj[i].quad.quad_offset];
			mdl->obj[i].quad.quad_map_offset = (u32)&data[mdl->obj[i].quad.quad_map_offset];
		}

		s_part = mdl->obj[i].quad.vertex_count * sizeof(FAST_SPRT16);

		// apply neutral diffuse
		ent->pPart[i].Poly_rgb = MAKERGB(128, 128, 128);
		// set packet and TM2 pointers
		ent->pPart[i].pPacket = (u32*)pool;
		ent->pPart[i].pTmd = (u32*)&mdl->obj[i].quad;
		setIdentityM(((MATRIX*)ent->pPart[i].M));

		// seek forward
		pool = &pool[(s_part) * 2];
	}

	// set model with data coming from the first joint
	ent->pPacket = ent->pPart[0].pPacket;
	ent->pTmd = ent->pPart[0].pTmd;

	// activate map flag
	mdl->map_flg = 1;

	return pool;
}

void MappingParticle(TM2_HEADER *mdl, PARTS_W *pPart, int count, int Tpage,
	int Clut)
{
	int i, j, cnt;
	// pools
	FAST_SPRT16 *q0, *q1;

	for (i = 0; i < count; i++)
	{
		cnt = mdl->obj[i].quad.vertex_count;

		// define quads
		q0 = (FAST_SPRT16*)pPart[i].pPacket;
		q1 = &q0[1];

		// map all properties that are not going to change during rendering
		for (j = 0; j < cnt; j++, q0 += 2, q1 += 2)
		{
			q0->tag = q1->tag = 5 << 24;
			q0->rgbc = q1->rgbc = ((TAG_SPRT16 | TAG_TRANS) << 24) | MAKERGB(128,10,10);
			q0->uvc = q1->uvc = Clut << 16;
			q0->mode = q1->mode = (TAG_MODE << 24) | Tpage;
		}
	}
}

void FastParticle(void *ob, void *packet, CVECTOR *rgb, u32* ot)
{
	register u32 i, is, *tag;
	int otz, flg;
	register FAST_SPRT16 *sx;
	const TM2_QUADS *obj = (const TM2_QUADS*)ob;
	const SVECTOR *vp = (const SVECTOR*)obj->vertex_offset;
	u32 code = ((rgb->cd & 3) | TAG_SPRT16) << 24 | MAKERGB(128,128,128);

	sx = (FAST_SPRT16*)packet;

	for (i = 0, is = obj->vertex_count; i < is; vp++)
	{
		FAST_SPRT16 *si;
		gte_ldv0(vp);
		si = sx;
		i++;
		gte_rtps_b();
		gte_stflg_m(flg);
		if (flg & GTEFLG_ERROR) { sx += 2; continue; }

		gte_stsxy(&si->xy);
		sx += 2;
		gte_stsz(&otz);
		if (!(otz >> 8)) continue;

		// link
		__addPrim(&ot[otz>>6], si, 4);
	}
}
