#include "..\core.h"

DEMO_ENV Dm;

static ENTITY ent[2];
static FSINE fsin;

#define C(x)		(int)((x)*180.f)

#define X(x)		C(x)
#define Y(x)		-C(x)
#define Z(x)		C(x)

static GTE_LIGHT light =
{
	// Mag
	1,
	// Mode
	LM_FALL,	LM_FALL,	LM_FALL,
	// Col
	{
		{ 128,128,128 },
		{ 192,192,192 },
		{ 192,192,192 },
	},
	// Ambient
	{ 64,64,64 },
	// Range
	{ 20000,20000,0 },
	// Pos
	{
		{ X(192.931), Y(9.869), Z(-43.53) },
		{ X(0), Y(116.116), Z(0) },
		{ X(0), Y(500), Z(-500) }
	}
};

static GTE_CAMERA camera =
{
	{ X(168.905),	Y(0),		Z(0) },
	{ X(0),			Y(0),		Z(0) }
};

static char *mes =
{
	"Welcome to the CD-rom loader sample.      "
	"                  "
	"There isn't much to see here, besides the "
	"code to perform all loading from CD and "
	"particle rendering.                       "
	"                  "
	"Enjoy this simple throwback to cracktros."
};

static void Draw_bg()
{
	static MAP_SPRT bg[] =
	{
		//	mode				x	y	w	h	u	v	clut
		18 | DMODE_8BIT,	0,	0,	256,240,0,	0,	getClut(0,242),
		20 | DMODE_8BIT,	256,0,	64,	240,0,	0,	getClut(0,242)
	};

	// draw background
	SortMapSprt(bg, 2, &GsGetOTag()[OT_SIZE - 1]);
}

void Demo_init()
{
	// load all textures
	Cd_load_file(0, (u32*)__packable);
	Upload_tpk((u32*)__packable);
	// set up font
	Font_load(NULL);
	Fsin_init(&fsin, 320, 32, mes);
	// initialize pad data
	Swirl_init(ent);
	// environment
	Gte.pLight = &light;
	Gte.pCamera = &camera;
	// update camera
	Gte_camera_set();
	SetGeomScreen(FOCAL_HEIGHT(23.458));
}

void Demo()
{
	Fsin_disp(&fsin, GsGetOTag());
	
	// render 3D with scratchpad to SP
	Sp_to_scratch();
	Render(&ent[0]);
	Render(&ent[1]);
	Sp_restore();

	ent[0].Rot_z += DEG(4);
	ent[1].Rot_z += DEG(4);

	Draw_bg();
}
