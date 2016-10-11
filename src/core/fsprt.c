#include "..\core.h"

void SortMapSprt(MAP_SPRT *sprt, int count, u32 *ot)
{
	int i;
	FAST_SPRT *s = gfxGetPtr();

	for (i = 0; i < count; i++, sprt++, s++)
	{
		s->mode = (TAG_MODE << 24) | sprt->mode;
		s->rgbc = (TAG_SPRT << 24) | MAKERGB(128, 128, 128);
		s->xy = *(u32*)&sprt->x;
		s->wh = *(u32*)&sprt->w;
		s->uvc = *(u32*)&sprt->u;
		addPrimSprt(ot, s);
	}

	gfxSetPtr(s);
}
