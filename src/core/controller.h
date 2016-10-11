#ifndef __CONTROLLER_H
#define __CONTROLLER_H

#include "control.h"

// raw pad flags
#define PAD_L2			0x0001
#define PAD_R2			0x0002
#define PAD_L1			0x0004
#define PAD_R1			0x0008
#define PAD_TRIANGLE	0x0010
#define PAD_CIRCLE		0x0020
#define PAD_CROSS		0x0040
#define PAD_SQUARE		0x0080
#define PAD_SEL			0x0100
#define PAD_SELECT		PAD_SEL
#define PAD_START		0x0800
#define PAD_UP			0x1000
#define PAD_RIGHT		0x2000
#define PAD_DOWN		0x4000
#define PAD_LEFT		0x8000

#define vibBuffLength	8

#define PAD_STATE_CHECK		0
#define PAD_STATE_EQUAL		1
#define PAD_STATE_STABLE	2

#define	DS_PAD_1	0		//port id for pad 1
#define	DS_PAD_2	1		//port id for pad 2

typedef struct tagVibData
{
	u16 cnt;
	u8 vib[2];
} VIB_DATA;

typedef struct tagVibPattern
{
	int cnt;
	VIB_DATA *data;
} VIB_PATTERN;

#ifndef DEFPAT
#define DEFPAT(x)	{_countof((x)),(x)}
#endif

enum Vibrations
{
	VibShot
};

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C"
{
#endif

extern u32 pad_raw, pad_raw_t;
extern u32 pad_user, pad_user_t;
extern int pad_axis0_x, pad_axis0_y;
extern int pad_axis1_x, pad_axis1_y;

void Read_pad();
int Vibrate(VIB_PATTERN *pattern);
void VibCallback();

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif
