#include "..\core.h"

u32 init_sub0()
{
	return 0;
}

u32 init_sub1()
{
	return 0;
}

int init()
{
	u32 a, b = w->v0c;

	w->v0f = 1;
	w->v10 = 0;
	w->v11 = 0;
	w->v12 = 0;	// not necessary
	w->v12 = 1;
	//
	a = (b & 0x30) | 1;
	w->v0c = a;
	*receive = a;
	//
	a = (b & 0x30) | 9;
	w->v0c = a;
	*receive = a;

	a = init_sub0() & 0x38;
	switch (a)
	{
	case 18:	// 1F003388
		w->v14 = 0x100;
		w->v16 = 0x108;
		w->v18 = 0x800;
		w->v1a = 0x400;
		w->v1c = 0x401;
		w->v1e = 0x402;
		w->v20 = 0x412;
		w->v13 = 0;
		break;
	case 20:
		w->v14 = 0x100;
		w->v16 = 0x108;
		w->v18 = 0x1000;
		w->v1a = 0x400;
		w->v1c = 0x401;
		w->v1e = 0x402;
		w->v20 = 0x412;
		w->v13 = 1;
		break;
	case 28:
		w->v14 = 0x200;
		w->v16 = 0x210;
		w->v18 = 0x1000;
		w->v1a = 0x200;
		w->v1c = 0x201;
		w->v1e = 0x202;
		w->v20 = 0x20A;
		w->v13 = 2;
		break;
	}

	if (init_sub1() == w->v38)
	{
		w->v26 = 0;
		w->v22 = w->v28;
		w->v24 = w->v2c;
		return 0;
	}

	return -1;
}

// not sure really if it's for activation
void activate()
{
	u32 r = get_receive() & ~0x40;
	set_receive(r);
	delay(1);
	set_receive(r | 0x40);
	delay(10);
}

int preparation()
{
	int i, bit;
	u32 byte, st;

	activate();
	st = *port & 1;

	for (i = 0, byte = 0xAE; i < 999999; i++)
	{
		setup_trans(1);
		bit = byte & 0x80;
		byte <<= 1;
		if (bit) byte |= 1;
		send_bit(bit);
		delay(1);
		setup_trans(0);
		delay(1);
		if (*port & 1 != st) break;
		byte &= 0xFF;
	}

	return i < 999999 ? 1 : 0;
}
