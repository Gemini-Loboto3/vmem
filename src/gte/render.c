#include "..\core.h"

static void Render_part(ENTITY *pEnt, PARTS_W *pPart);

void Render(ENTITY *pEnt)
{
	int i;
	MATRIX rot, final;
	u32 Flg = pEnt->Flg;

	// calculate light matrices
	Gte_set_light((VECTOR*)&pEnt->Pos_x, Gte.pLight);
	gte_SetColorMatrix(&Gte.M_lc);

	// calculate entity rotation
	RotMatrix((SVECTOR*)&pEnt->Rot_x, (MATRIX*)pEnt->M);
	// M*super, allows linked objects
	gte_CompMatrix((MATRIX*)pEnt->pSuper, (MATRIX*)pEnt->M, &pEnt->Work);

	// draw
	for (i = 0; i < pEnt->Part_num; i++)
		Render_part(pEnt, &pEnt->pPart[i]);
}

void Render_part(ENTITY *pEnt, PARTS_W *pPart)
{
	MATRIX m;
	CVECTOR col;
	u32 Flg = pPart->Flg;

	if (Flg & FLGPRT_TRANS)
		pPart->Poly_rgb = pPart->Poly_rgb & 0xFFFFFF | (TAG_TRANS << 24);
	else pPart->Poly_rgb &= 0xFFFFFF;

	// M*super
	gte_CompMatrix((MATRIX*)pPart->pSuper, (MATRIX*)pPart->M, &pPart->Work);

	// apply light matrix to this object
#if 1
	Gte_set_light((VECTOR*)&pPart->Pos_x, Gte.pLight);	// update local light matrix
#endif
	gte_MulMatrix0(&Gte.M_ll, &pPart->Work, &m);
	gte_SetLightMatrix(&m);

	// build view * world matrix and set to GTE
	gte_CompMatrix(&Gte.M_view, &pPart->Work, &m);
	if (Flg & FLGPRT_SCALE)
	{
		VECTOR scale = { pPart->Scale_x, pPart->Scale_y, pPart->Scale_z };
		ScaleMatrix(&m, &scale);
	}
	gte_SetRotMatrix(&m);
	gte_SetTransMatrix(&m);

	// uses diffuse alone
	if (Flg & FLGPRT_COLOR)
	{
		// diffuse + lights
		FastG3L((void*)pPart->pTmd,  (void*)&((POLY_GT3*)(pPart->pPacket))[Gs.OTag_id],  (CVECTOR*)&pPart->Poly_rgb, GsGetOTag());
		FastG4L((void*)pPart->pTmd2, (void*)&((POLY_GT4*)(pPart->pPacket2))[Gs.OTag_id], (CVECTOR*)&pPart->Poly_rgb, GsGetOTag());
	}
	else if (Flg & FLGPRT_PARTICLE)
	{
		FastParticle((void*)pPart->pTmd, (void*)&((FAST_SPRT16*)(pPart->pPacket))[Gs.OTag_id], (CVECTOR*)&pPart->Poly_rgb, GsGetOTag());
	}
	// has texture
	else
	{
		// recalculate uv coordinates for chrome effect
		if (Flg & FLGPRT_CHROME)
		{
			calc_chrome_uvs_tri(&m,  (TM2_TRIANGLES*)pPart->pTmd, &((POLY_GT3*)(pPart->pPacket))[Gs.OTag_id]);
			calc_chrome_uvs_quad(&m, (TM2_QUADS*)pPart->pTmd2,    &((POLY_GT4*)(pPart->pPacket2))[Gs.OTag_id]);
		}

		// texture + lights
		FastTG3L((void*)pPart->pTmd,  (void*)&((POLY_GT3*)(pPart->pPacket))[Gs.OTag_id],  (CVECTOR*)&pPart->Poly_rgb, GsGetOTag());
		FastTG4L((void*)pPart->pTmd2, (void*)&((POLY_GT4*)(pPart->pPacket2))[Gs.OTag_id], (CVECTOR*)&pPart->Poly_rgb, GsGetOTag());
	}
}

void FastTG3L(void *ob, void *packet, CVECTOR *rgb, u32* ot)
{
	register u32 i, is, *tag;
	register int otz;
	register POLY_GT3 *sx;
	const TM2_TRIANGLES *obj = (const TM2_TRIANGLES*)ob;
	const TM2_TRIANGLE *t = (const TM2_TRIANGLE*)obj->tri_offset;
	const SVECTOR *vp = (const SVECTOR*)obj->vertex_offset;
	const SVECTOR *vn = (const SVECTOR*)obj->normal_offset;

	rgb->cd = (rgb->cd & 3) | TAG_PGT3;
	gte_ldrgb(rgb);

	sx = (POLY_GT3*)packet;

	for (i = 0, is = obj->tri_count; i < is; t++)
	{
		POLY_GT3 *si;
		gte_ldv3(&vp[t->v0], &vp[t->v1], &vp[t->v2]);	/* load model vertices */
		i++;
		si = sx;
		gte_rtpt_b();					/* perspective */

		gte_stflg_m(otz);
		if (otz & GTEFLG_ERROR) { sx += 2; continue; }
		//gte_nclip_b();					/* normal clipping */
		//gte_stopz_m(otz);
		//if (otz <= 0) { sx += 2; continue; }

		gte_stsxy3_gt3(si);				/* store transformed result */
		sx += 2;
		gte_nop();
		gte_avsz3_b();					/* calculate depth */
		gte_stotz_m(otz);
		if (!(otz >> 6)) continue;

		gte_ldv3(&vn[t->n0], &vn[t->n1], &vn[t->n2]);	/* set lighting */
		tag = &ot[otz >> 4];
		si->tag = (*tag & 0x00FFFFFF) | 0x09000000;
		gte_ncct_b();								/* calculate */
		gte_strgb3_gt3(si);							/* store rgb values */

		// link
		*tag = (u32)si & 0x00FFFFFF;
	}
}

void FastTG4L(void *ob, void *packet, CVECTOR *rgb, u32* ot)
{
	register u32 i, is, *tag;
	int otz, flg;
	register POLY_GT4 *sx;
	const TM2_QUADS *obj = (const TM2_QUADS*)ob;
	const TM2_QUAD *q = (const TM2_QUAD*)obj->quad_offset;
	const SVECTOR *vp = (const SVECTOR*)obj->vertex_offset;
	const SVECTOR *vn = (const SVECTOR*)obj->normal_offset;

	rgb->cd = (rgb->cd & 3) | TAG_PGT4;
	gte_ldrgb(rgb);

	sx = (POLY_GT4*)packet;

	for (i = 0, is = obj->quad_count; i < is; q++)
	{
		POLY_GT4 *si;
		gte_ldv3(&vp[q->v0], &vp[q->v1], &vp[q->v2]);
		si = sx;
		i++;
		gte_rtpt_b();			/* RotTransPers3 */

		gte_stflg_m(flg);
		if (flg & GTEFLG_ERROR) { sx += 2; continue; }
		//gte_nclip_b();			/* normal clipping */
		//gte_stopz_m(otz);		/* back clip */
		//if (otz <= 0) { sx += 2; continue; }	/* flipped, skip */

		gte_stsxy3_gt4((u_long *)si);

		gte_ldv0(&vp[q->v3]);
		sx += 2;
		gte_nop();
		gte_rtps_b();			/* RotTransPers */
		gte_stflg_m(flg);
		if (flg & GTEFLG_ERROR) continue;

		gte_stsxy_gt4_3(si);
		gte_avsz4();
		gte_stotz_m(otz);
		if (!(otz >> 6)) continue;

		gte_ldv3(&vn[q->n0], &vn[q->n1], &vn[q->n2]);
		tag = &ot[otz >> 4];
		gte_ncct_b();
		gte_strgb3_gt4(si);

		gte_ldv0(&vn[q->n3]);
		si->tag = (*tag & 0x00FFFFFF) | 0x0C000000;
		gte_nccs_b();
		gte_strr3_gt4(si);

		// link
		*tag = (u32)si & 0x00FFFFFF;
	}
}

void FastG3L(void *ob, void *packet, CVECTOR *rgb, u32* ot)
{
	register u32 i, is, *tag;
	register int otz;
	register POLY_G3 *sx;
	const TM2_TRIANGLES *obj = (const TM2_TRIANGLES*)ob;
	const TM2_TRIANGLE *t = (const TM2_TRIANGLE*)obj->tri_offset;
	const SVECTOR *vp = (const SVECTOR*)obj->vertex_offset;
	const SVECTOR *vn = (const SVECTOR*)obj->normal_offset;

	rgb->cd = (rgb->cd & 3) | TAG_PG3;
	gte_ldrgb(rgb);

	sx = (POLY_G3*)packet;

	for (i = 0, is = obj->tri_count; i < is; t++)
	{
		POLY_G3 *si;
		gte_ldv3(&vp[t->v0], &vp[t->v1], &vp[t->v2]);	/* load model vertices */
		i++;
		si = sx;
		gte_rtpt_b();					/* perspective */

		gte_stflg_m(otz);
		if (otz & GTEFLG_ERROR) { sx += 2; continue; }
		//gte_nclip_b();					/* normal clipping */
		//gte_stopz_m(otz);
		//if (otz <= 0) { sx += 2; continue; }

		gte_stsxy3_g3(si);				/* store transformed result */
		sx += 2;
		gte_nop();
		gte_avsz3_b();					/* calculate depth */
		gte_stotz_m(otz);
		if (!(otz >> 6)) continue;

		gte_ldv3(&vn[t->n0], &vn[t->n1], &vn[t->n2]);	/* set lighting */
		tag = &ot[otz >> 4];
		si->tag = (*tag & 0x00FFFFFF) | 0x06000000;
		gte_ncct_b();								/* calculate */
		gte_strgb3_g3(si);							/* store rgb values */

													// link
		*tag = (u32)si & 0x00FFFFFF;
	}
}

void FastG4L(void *ob, void *packet, CVECTOR *rgb, u32* ot)
{
	register u32 i, is, *tag;
	int otz, flg;
	register POLY_G4 *sx;
	const TM2_QUADS *obj = (const TM2_QUADS*)ob;
	const TM2_QUAD *q = (const TM2_QUAD*)obj->quad_offset;
	const SVECTOR *vp = (const SVECTOR*)obj->vertex_offset;
	const SVECTOR *vn = (const SVECTOR*)obj->normal_offset;

	rgb->cd = (rgb->cd & 3) | TAG_PG4;
	gte_ldrgb(rgb);

	sx = (POLY_G4*)packet;

	for (i = 0, is = obj->quad_count; i < is; q++)
	{
		POLY_G4 *si;
		gte_ldv3(&vp[q->v0], &vp[q->v1], &vp[q->v2]);
		si = sx;
		i++;
		gte_rtpt_b();			/* RotTransPers3 */

		gte_stflg_m(flg);
		if (flg & GTEFLG_ERROR) { sx += 2; continue; }
		//gte_nclip_b();			/* normal clipping */
		//gte_stopz_m(otz);		/* back clip */
		//if (otz <= 0) { sx += 2; continue; }	/* flipped, skip */

		gte_stsxy3_g4((u_long *)si);

		gte_ldv0(&vp[q->v3]);
		sx += 2;
		gte_nop();
		gte_rtps_b();			/* RotTransPers */
		gte_stflg_m(flg);
		if (flg & GTEFLG_ERROR) continue;

		gte_stsxy_g4_3(si);
		gte_avsz4();
		gte_stotz_m(otz);
		if (!(otz >> 6)) continue;

		gte_ldv3(&vn[q->n0], &vn[q->n1], &vn[q->n2]);
		tag = &ot[otz >> 4];
		gte_ncct_b();
		gte_strgb3_g4(si);

		gte_ldv0(&vn[q->n3]);
		si->tag = (*tag & 0x00FFFFFF) | 0x08000000;
		gte_nccs_b();
		gte_strr3_g4(si);

		// link
		*tag = (u32)si & 0x00FFFFFF;
	}
}

#define gte_a(r1,r2,r3)				\
				{	gte_ldv0(r2);	\
					gte_rtv0();		\
					gte_stlvnl(r3);	}

#define calc_chrome(trans,norm,temp,u)	gte_a(trans,norm,&temp);\
		u = (64-(temp.vx>>6))|((64-(temp.vy>>6))<<8)\

void calc_chrome_uvs_tri(MATRIX *trans, TM2_TRIANGLES *tri_obj, POLY_GT3 *tri)
{
	int	i, count;
	SVECTOR	*norm;
	VECTOR result;
	TM2_TRIANGLE *t;

	t = (TM2_TRIANGLE*)tri_obj->tri_offset;
	norm = (SVECTOR*)tri_obj->normal_offset;

	for (i = 0, count = tri_obj->tri_count; i < count; i++, t++, tri += 2)
	{
		calc_chrome(trans, &norm[t->n0], result, *(u16*)&tri->u0);
		calc_chrome(trans, &norm[t->n1], result, *(u16*)&tri->u1);
		calc_chrome(trans, &norm[t->n2], result, *(u16*)&tri->u2);
	}
}

void calc_chrome_uvs_quad(MATRIX *trans, TM2_QUADS *quad_obj, POLY_GT4 *quad)
{
	int	i, count;
	VECTOR	result;
	SVECTOR *norm;
	TM2_QUAD *q;

	q = (TM2_QUAD*)quad_obj->quad_offset;
	norm = (SVECTOR*)quad_obj->normal_offset;

	for (i = 0, count = quad_obj->quad_count; i < count; i++, q++, quad += 2)
	{
		calc_chrome(trans, &norm[q->n0], result, *(u16*)&quad->u0);
		calc_chrome(trans, &norm[q->n1], result, *(u16*)&quad->u1);
		calc_chrome(trans, &norm[q->n2], result, *(u16*)&quad->u2);
		calc_chrome(trans, &norm[q->n3], result, *(u16*)&quad->u3);
	}
}
