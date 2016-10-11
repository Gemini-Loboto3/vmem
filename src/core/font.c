/* ==================================== */
/* Font handling routine for ASCII.		*/
/* ====================================	*/
#include "..\core.h"
#include "prim.h"

// width table for the font
static u8 width_tbl[96];

// helper code used for calculating font width tables
typedef struct tagNibble
{
	u32 *data;
	u32 prev;
	int bit;
} NIBBLE;

static void Init_nibble(NIBBLE *n, u32 *data)
{
	n->data = &data[1];
	n->bit = 0;
}

static u32 Pull_nibble(NIBBLE *n)
{
	u32 read;

	// pull new word from data
	if (!n->bit) read = *n->data--;
	// pull old nibbles
	else read = n->prev;

	// push one nibble out
	n->prev = read << 4;
	// seek forward
	n->bit = (n->bit + 1) % 8;

	// return the nibble
	return read >> 28;
}

// texture coordinates
#define FONT_TX		0
#define FONT_TY		256
// clut coordinates
#define FONT_CX		0
#define FONT_CY		(256+96)
// primitive data
#define FONT_CLUT	getClut(FONT_CX,FONT_CY)
#define FONT_TPAGE	(FONT_TX / 64 + FONT_TY / 256 * 16) | DMODE_4BIT

void Font_load(u32 *font)
{
	int x, y;
	// send to vram
	//Upload_tim(font);
	// create width tables
	Font_calc_width();
}

void Font_calc_width()
{
	RECT rect;
	u32 buffer[32];	// store 16 scanlines from VRAM
	NIBBLE n;		// helper for reading the font
	int i, j, x, y, w, mw;
	u8 *tbl = width_tbl;

	for (y = 0; y < 6; y++)
	{
		for (x = 0; x < 16; x++)
		{
			// store current character
			setRECT(&rect, FONT_TX + x * (16 / 4), FONT_TY + y * 16, 16 / 4, 16);
			StoreImage(&rect, buffer);
			DrawSync(0);
			// process all scanlines to gather width
			for (i = 0, mw = 0; i < 16 * 2; i += 2)
			{
				Init_nibble(&n, &buffer[i]);
				// count leading blank pixels
				for (j = 0, w = 16; j < 16; j++, w--)
					if (Pull_nibble(&n))
						break;
				// assign new max width if we found a larger match
				if (w > mw) mw = w;
			}
			// assign width + 1
			*tbl++ = mw + 1;
		}
	}

	width_tbl[0] = 5;	// override blankspace width
}

static void Font_set_sprt(u32 xy, ALPHA_PRIM8 *p, int ch, u32 *ot)
{
	int i, j;
	ALPHA_SPRT8 *s;
	#define SUB0 0x808080
	#define SUB1 0x505050

	// get sprite chain
	s = p->sprt;
	for (i = 0; i < 2; i++, s++)
	{
		// tpage id
		s->mode = (TAG_MODE << 24) | FONT_TPAGE | (i != 0 ? DMODE_TRANS1 : DMODE_TRANS2);
		// coordinate and code
		s->xy = xy;
		// sprite id
		s->rgbc = ((TAG_SPRT16 | 2) << 24) | (i != 0 ? SUB0 : SUB1);
		// set uv map and clut
		setFSprtUVClut(s, ch % 16 * 16, ch / 16 * 16, FONT_CLUT);
	}

	__addPrim(ot, p, 8);
}

void Font_disp(int x, int y, const char *string, u32 *ot)
{
	int px = x, ch;
#if 0
	FAST_SPRT16 *s = gfxGetPtr();
#else
	ALPHA_PRIM8 *s = (ALPHA_PRIM8*)gfxGetPtr();
#endif

	while (1)
	{
		ch = *string++;
		switch (ch)
		{
		case '\0':
			gfxSetPtr(s);
			return;
		case '\n':
			px = x;
			y += 16;
			break;
		case ' ':	// blank space, don't print
			px += width_tbl[' ' - 0x20];
			break;
		default:
			ch -= 0x20;
#if 0
			setFSSprtPage(s, FONT_TPAGE);
			setFSprtRGB16(s, 128, 128, 128);
			setFSprtUVClut(s, (ch % 16) * 16, ch / 16 * 16, FONT_CLUT);
			setFSprtXY(s, px, y);
			addPrimSprt16(ot, s);
#else
			Font_set_sprt((px & 0xFFFF) | (y << 16), s, ch, ot);
#endif
			s++;
			px += width_tbl[ch];
		}
	}
}

int Font_str_getw(const char *string)
{
	int ch, w, mw;

	w = 0;
	mw = 0;

	while (1)
	{
		ch = *string++;

		switch (ch)
		{
		case '\0':
			if (w > mw) mw = w;
			return w;
		case '\n':
			if (w > mw)
			{
				mw = w;
				w = 0;
			}
			break;
		default:
			w += width_tbl[ch - 0x20];
		}
	}

	return 0;
}

///////////////////////////////////////////////////////////////
static void Fsin_set_poly(int x, int y, int rot, int ch, u32 *ot);

void Fsin_init(FSINE *s, int x, int y, char *str)
{
	s->x = s->sx = x + 16;
	s->y = s->sy = y;
	s->str = str;
	s->sine = 0;
	s->width = Font_str_getw(str) * 2;
}

void Fsin_disp(FSINE *s, u32 *ot)
{
	int x, y, ch, sine;
	char *string = s->str;

	x = s->x;
	y = s->y;

	// reset scrolling position
	if (x + s->width < -16)
	{
		x = s->x = s->sx;
		s->sine = 0;
	}

	sine = s->sine;

	while (1)
	{
		ch = *string++;

		if (!ch) break;

		// do not print outside of display area
		if (x >= RES_W + 16)
			break;

		switch (ch)
		{
		case ' ':	// blank space, don't print
			x += width_tbl[' ' - 0x20] * 2;
			sine++;
			break;
		default:
			ch -= 0x20;
			// only display visible characters
			if (x > -32)
				Fsin_set_poly(x, y + csin(sine) / 256, -csin(sine)/16, ch, ot);
			x += width_tbl[ch] * 2;
			sine+=64;
		}
	}

	s->x--;
	s->sine+=64;
}

void Fsin_set_poly(int x, int y, int rot, int ch, u32 *ot)
{
	int i, j, u, v;
	// mapping for sprite size
	static SVECTOR sv0[4]=
	{
		{-16, 0, -16},
		{ 16, 0, -16},
		{-16, 0,  16},
		{ 16, 0,  16}
	},
		dir = {0,0,0};
	SVECTOR sv1[4];
	ALPHA_FT4 *p = gfxGetPtr();
	ALPHA_FT4B *s;
#define SUB0 0x808080
#define SUB1 0x505050
#define SETXY(x,y)			((x) & 0xFFFF)|((y)<<16)
#define SETUVP(u,v,p)		(u)|((v)<<8)|((p)<<16)

	// calculate rotated points
	dir.vy = rot;
	Gte_rot_svecs(&dir, sv0, sv1, 4);

	// cache uv coordinates
	u = ch % 16 * 16;
	v = ch / 16 * 16;

	// get poly chain
	s = p->poly;
	for (i = 0; i < 2; i++, s++)
	{
		// coordinates
		s->xy0 = SETXY(x + sv1[0].vx, y + sv1[0].vz);
		s->xy1 = SETXY(x + sv1[1].vx, y + sv1[1].vz);
		s->xy2 = SETXY(x + sv1[2].vx, y + sv1[2].vz);
		s->xy3 = SETXY(x + sv1[3].vx, y + sv1[3].vz);
		// uvmap
		s->uv0c = SETUVP(u + 1, v + 1, FONT_CLUT);
		s->uv1p = SETUVP(u + 15, v + 1, FONT_TPAGE | (i != 0 ? DMODE_TRANS1 : DMODE_TRANS2));
		s->uv2 = SETUVP(u+1, v + 15, 0);
		s->uv3 = SETUVP(u+15, v + 15, 0);
		// sprite id
		s->rgb0c = ((TAG_PFT4 | TAG_TRANS) << 24) | (i != 0 ? SUB0 : SUB1);
	}

	__addPrim(ot, p, 18);
	gfxSetPtr(&p[1]);
}
