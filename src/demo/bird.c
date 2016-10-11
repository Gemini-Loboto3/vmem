#include "..\core.h"

typedef struct tagBird
{
	int timer;		// animation timer
	int frame;		// animation frame
	int fnext;		// next frame
	short rotz;		// z rotation
	short velocity;	// gravity
	short gain;		// flap gain
	short x, y;
} BIRD, *LPBIRD;

#define SETXY(x,y)			((x) & 0xFFFF)|((y)<<16)

void Bird_draw(LPBIRD pBird, u32 *ot)
{
	int x, y;
	POLY_FT4 *p = gfxGetPtr();
	static SVECTOR
		dir = { 0, 0, 0 },	// empty rotation
		sv0[4]=				// points for fluffy's sprites
		{
			{-12, 0, -12},
			{ 12, 0, -12},
			{-12, 0,  12},
			{ 12, 0,  12}
		};
	SVECTOR sv1[4];			// transformed points for fluffy

	// set POLY_FT4 code and ignore rgb channels
	*(u32*)&p->r0 = (TAG_PFT4 | TAG_BLIT) << 24;
	// get rotated points for fluffy
	dir.vy = pBird->rotz;
	Gte_rot_svecs(&dir, sv0, sv1, _countof(sv0));

	x = pBird->x, y = pBird->y;
	// map points to screen coordinates
	*(u32*)&p->x0 = SETXY(x + sv1[0].vx, y + sv1[0].vz);
	*(u32*)&p->x1 = SETXY(x + sv1[1].vx, y + sv1[1].vz);
	*(u32*)&p->x2 = SETXY(x + sv1[2].vx, y + sv1[2].vz);
	*(u32*)&p->x3 = SETXY(x + sv1[3].vx, y + sv1[3].vz);

	// optimized sort and update primitive pool
	__addPrim(ot, p, sizeof(*p) / 4 - 1);
	gfxSetPtr(&p[1]);
}

void Bird_update(LPBIRD pBird)
{

}
