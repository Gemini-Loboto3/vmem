#ifndef __FONT_H
#define __FONT_H

typedef struct tagFSine
{
	s16 x, y;	// base positions
	s16 sx, sy;	// reset positions
	s16 sine,	// wave counter
		width;	// width of string
	char *str;	// message
} FSINE;

// basic font handlers
void Font_load(u32 *font);
void Font_calc_width();
void Font_disp(int x, int y, const char *string, u32 *ot);
int  Font_str_getw(const char *string);

// executes code for sine effects
void Fsin_init(FSINE *s, int x, int y, char *str);
void Fsin_disp(FSINE *s, u32 *ot);

#endif
