#include "..\core.h"

#define C(x)		(int)((x)*180.f)

#define X(x)		C(x)
#define Y(x)		-C(x)
#define Z(x)		C(x)

static VECTOR Pos[]=
{
	0,			0,			0,
};

#define PAD_TPAGE		10 | DMODE_4BIT | DMODE_TRANS1
#define PAD_CLUT		getClut(0,353)

void Swirl_init(ENTITY *pEnt)
{
	int i;
	PARTS_W *pP;
	TM2_HEADER *Model = (TM2_HEADER*)getPackable();
	u8 *pPool = (u8*)Model;

	// load data
	pPool += Cd_load_file(1, (u32*)Model);
	// open and setup model data
	pPool = Particle_allocate(pEnt, (TM2_HEADER*)Model, pPool);
	MappingParticle(Model, pEnt[0].pPart, pEnt[0].Part_num, PAD_TPAGE, PAD_CLUT);

	AllocatePool(&pEnt[1], Model, pPool);
	MappingTm2(Model, pEnt[1].pPart, pEnt[1].Part_num, PAD_TPAGE, PAD_CLUT);

	LinkBones(NULL, &pEnt[0].pPart[0], Pos);
	LinkBones(NULL, &pEnt[1].pPart[0], Pos);

	setIdentityM((MATRIX*)pEnt[0].M);
	pEnt[0].pSuper = &M_id;
	// setup particles
	pP = pEnt[0].pPart;
	pP->Flg = FLGPRT_PARTICLE | FLGPRT_TRANS;	// controller
	pP->pSuper = &pEnt->Work;
	pP->Poly_rgb = MAKERGB(128, 128, 128);
	// setup wave model
	setIdentityM((MATRIX*)pEnt[1].M);
	pP = pEnt[1].pPart;
	pP->Flg = FLGPRT_COLOR | FLGPRT_TRANS;
	pP->pSuper = &pEnt->Work;
	pP->Poly_rgb = MAKERGB(0, 128, 192);
}
