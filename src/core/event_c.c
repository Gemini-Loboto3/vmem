#include "..\core.h"

void event0_sub()
{
	int i, bit, c;
	u8 *buffer = (u8*)&w->v28;

	w->v38 = 0xAEAEAEAE;

	for (i = 0; i < 16; i++)
		w->v38 += buffer[i];
	wait_blank();

	i = w->v0c & 0x30;
	w->v0f = 0;
	if (w->v10) i |= 2;
	if (w->v11) i |= 4;
	if (w->v12) i |= 8;

	// send bit stuff both to buffer and receiver
	w->v0c = i;
	receive = i;
	c = i & 0xFF;
	for (bit = 0x80; bit; bit >>= 1)
	{
		if (bit & 0x85) c |= 4;
		c |= 2;
		*receive = c;
		c &= 0x3D;
		*receive = c;
	}

	// 1F001A04
}

void event0_nest()
{
	if (w->v26 >= 0x32)
	{
		*(u32*)&w->v28 = w->v22;
		*(u32*)&w->v2c = w->v24;
		event0_sub();
		w->v26 -= 0x32;

		return;
	}

	if (w->v22)
	{
		wait_bank();
		w->v0f = 0;
	}
	else
	{

	}
}

int event00_main(int par0, int par1, int par2, int par3)
{
	if ((par0 || w->v30 >= 0) &&
		(par0 != 1 || w->v34 >= 0))
	{
		// lw v0, 0x7500(v0)
		event0_nest();
		return 1;
	}


	return -1;
}

void sub0(int par0, int par1, int par2, int par3)
{
	int i, c, bit, mask;

	wait_bank();

	i = w->v0c & 0x30;
	w->v0f = 0;
	if (w->v10) i |= 2;
	if (w->v11) i |= 4;
	if (w->v12) i |= 8;

	// 1F002018 @@bitstuff1
	c = i & 0xFF;
	for (bit = 0x80; bit; bit >>= 1)
	{
		c &= 0x3B;
		if (bit & 0x53) c |= 4;
		c |= 2;
		*receive = c;
		c &= 0x3D;
		*receive = c;
	}

	// 1F002060 @@bitstuff2
	mask = (par0 >> 7) & 0x1F;
	c = w->v0c;
	for (bit = 0x80; bit; bit >>= 1)
	{
		c &= 0x3B;
		if (bit & mask) c |= 4;
		c |= 2;
		*receive = c;
		c &= 0x3D;
		*receive = c;
	}

	// 1F0020A8 @@bitstuff3
	mask = (par0 << 1) & 0xFE;
	c = w->v0c;
	for (bit = 0x80; bit; bit >>= 1)
	{
		c &= 0x3B;
		if (bit & mask) c |= 4;
		c |= 2;
		*receive = c;
		c &= 0x3D;
		*receive = c;
	}

	// 1F0020E8 @@bit_stuff4
	c = w->v0c;
	for (bit = 0x80; bit; bit >>= 1)
	{
		c &= 0x3B;
		c |= 2;
		*receive = c;
		c &= 0x3D;
		*receive = c;
	}
}

void sub1(int par0, int par1, int par2, int par3) {}
void sub2(int par0, int par1, int par2, int par3) {}
void sub3(int par0, int par1, int par2, int par3) {}

void event1_nest(int par0, int par1)
{
	switch (w->v13)
	{
	case 0:
	case 1:
		sub0(par0 >> 1, (par0 & 1) << 7, par1, 0x80);
		break;
	case 2:
		sub1(par0 >> 2, (par0 & 3) << 7, par1, 0x80);
		break;
	}
}

int event01_main(int par0, int par1, int par2, int par3)
{
	u32 sr;
	int v7500, v7550, v7508;

	if ((par3 || w->v30 >= 0) &&
		(par3 != 1 || w->v34 >= 0))
	{
		v7500 = ((int*)0x7500)[par3];
		v7550 = ((int*)0x7550)[par3];
		v7508 = ((int*)0x7508)[par3];
		if (v7500 == 0x10) v7508 = 0x400;

		sr = get_sr();
		set_sr(sr & ~SR_IEC);	// disable interrupt
		event1_nest(v7508, v7550);
		set_sr(sr);				// restore interrupt

		return 1;
	}

	return -1;
}

void event2_nest(int par0, int par1)
{
	switch (w->v13)
	{
	case 0:
	case 1:
		sub2(par0 >> 1, (par0 & 1) << 7, par1, 0x80);
		break;
	case 2:
		sub3(par0 >> 2, (par0 & 3) << 7, par1, 0x80);
		break;
	}
}

int event02_main(int par0, int par1, int par2, int par3)
{
	u32 sr;
	int v7500, v7550, v7508;

	if ((par2 || w->v30 >= 0) &&
		(par2 != 1 || w->v34 >= 0))
	{
		v7500 = ((int*)0x7500)[par2];
		v7550 = ((int*)0x7550)[par2];
		v7508 = ((int*)0x7508)[par2];
		if (v7500 == 0x10) v7508 = 0x400;

		sr = get_sr();
		set_sr(sr & ~SR_IEC);	// disable interrupt
		event2_nest(v7508, v7550);
		set_sr(sr);				// restore interrupt

		return 1;
	}

	return -1;
}
