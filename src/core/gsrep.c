/* ==================================== */
/* LibGS replacement module.			*/
/* ------------------------------------ */
/* Changes:								*/
/* ** 13 Jun 2016 **					*/
/* Added a few casts to kill type match	*/
/* warnings when Gs is volatile.		*/
/* ==================================== */
#include "..\core.h"

// global structure that gathers LibGS-alike internals
volatile GS_ENV Gs;

// Set up draw/display environment
// ---------------------------
// Parameters
// x/y: frame buffers starting position in VRAM 
// w/h: size of display/draw
// mode: bitflag to determine if we're using interlacement or sideways frame buffers
void GsSetDisplay(int x, int y, int w, int h, u32 mode)
{
	int x0, x1, y0, y1;

	// copy resolution for later needs
	Gs.Screen_w = w;
	Gs.Screen_h = h;

	// interlaced mode
	if (mode & RESMODE_INTERLACE)
	{
		SetDefDrawEnv((DRAWENV*)&Gs.Draw_env[0], x, y, w, h);
		SetDefDispEnv((DISPENV*)&Gs.Disp_env[0], x, y, w, h);
		SetDefDrawEnv((DRAWENV*)&Gs.Draw_env[1], x, y, w, h);
		SetDefDispEnv((DISPENV*)&Gs.Disp_env[1], x, y, w, h);
		Gs.Disp_env[0].isinter = TRUE;
		Gs.Disp_env[1].isinter = TRUE;
	}
	else
	{
		// frame buffers are stored sideways
		if (mode & RESMODE_SIDEWAYS)
		{
			x0 = x;
			x1 = x + w;
			y0 = y;
			y1 = y;
		}
		// otherwise they are placed vertically
		else
		{
			x0 = x;
			x1 = x;
			y0 = y;
			y1 = y + h;
		}

		// libgpu calls to set up the environment
		SetDefDrawEnv((DRAWENV*)&Gs.Draw_env[0], x0, y0, w, h);
		SetDefDispEnv((DISPENV*)&Gs.Disp_env[0], x0, y1, w, h);
		SetDefDrawEnv((DRAWENV*)&Gs.Draw_env[1], x1, y1, w, h);
		SetDefDispEnv((DISPENV*)&Gs.Disp_env[1], x1, y0, w, h);
		// disable interlacement, we don't need it
		Gs.Disp_env[0].isinter = FALSE;
		Gs.Disp_env[1].isinter = FALSE;
	}

	// enable draw on display area
	Gs.Draw_env[0].dfe = Gs.Draw_env[1].dfe = TRUE;
}

// static allocation for package
static char _gfxAlloc[2][PACKET_SIZE];

void GsInitPacket()
{
	Gs.Gfx_alloc[0] = (u32*)_gfxAlloc[0];
	Gs.Gfx_alloc[1] = (u32*)_gfxAlloc[1];
}

// set handy pointers for frame buffer swap
void GsResetEnv()
{
	int Id = Gs.OTag_id;
	// set references for quick access
	Gs.pGfx  = Gs.Gfx_alloc[Id];			// packet
	Gs.pOt   = (u32*)Gs.OTag[Id];			// otag
	Gs.pDraw = (DRAWENV*)&Gs.Draw_env[Id];	// enviroments
	Gs.pDisp = (DISPENV*)&Gs.Disp_env[Id];
}

// deal with frame buffer swap and clear background if it's necessary
// put this at the beginning of a screen loop
void GsBeginDraw()
{
	DRAWENV *pD;

	GsResetEnv();
	ClearOTagR(Gs.pOt, OT_SIZE);

	pD = Gs.pDraw;
	// enable clear color
	if (Gs.Clear_mode == GCM_CLEAR)
	{
		pD->r0 = Gs.Clear_rgb.r;
		pD->g0 = Gs.Clear_rgb.g;
		pD->b0 = Gs.Clear_rgb.b;
		pD->isbg = TRUE;
	}
	// there is no clear
	else pD->isbg = FALSE;
}

// draw all linked primitives and perform the actual swap for next frame buffer
// put this at the end of a screen loop
void GsEndDraw()
{
	// wait until we reach vsync
	DrawSync(0);
	VSync(Gs.VSync_rate);
	// display previous frame
	PutDispEnv(Gs.pDisp);
	// set current buffer for display
	PutDrawEnv(Gs.pDraw);
	// send list to DMA
	DrawOTag(&Gs.pOt[OT_SIZE - 1]);
	// swap buffer
	Gs.OTag_id ^= 1;
}