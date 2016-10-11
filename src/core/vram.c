/* ==================================== */
/* Provide a few helper function for	*/
/* VRAM handling.						*/
/* ------------------------------------ */
/* Changelog:							*/
/* 16 Jun 2016: added package upload.	*/
/* ====================================	*/
#include "..\core.h"

void Vram_clear()
{
	RECT rect;
	setRECT(&rect, 0, 0, 1024, 512);
	ClearImage(&rect, 0, 0, 0);
	DrawSync(0);
}

void Upload_tim(u32 *data)
{
	TIM_IMAGE tim;

	OpenTIM(data);
	ReadTIM(&tim);

	// upload texture if there's any
	if (tim.paddr)
	{
		LoadImage(tim.prect, tim.paddr);
		DrawSync(0);
	}
	// upload clut if there's any
	if (tim.caddr)
	{
		LoadImage(tim.crect, tim.caddr);
		DrawSync(0);
	}
}

// TIM package uploader
void Upload_tpk(u32 *data)
{
	int i, si;
	u8 *p = (u8*)data;

	// process all TIMs in this package
	for (i = 0, si = *data / 4; i < si; i++)
		Upload_tim((u32*)&p[data[i]]);
}
