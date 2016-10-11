#include "..\core.h"

volatile u8 *receive = (volatile u8*)0x1f006010;	// (W) set to send stuff
volatile u8 *bank = (volatile u8*)0x1f006030;		// (W) transmission status?
volatile u8 *port = (volatile u8*)0x1f006038;		// (R) read data from flashes
volatile WORK *w = (volatile WORK*)0x3db4;

void reset_receive()
{
	w->v0c = 0;
}

u8 get_receive()
{
	return w->v0c;
}

u8 set_receive(u8 byte)
{
	w->v0c = byte;
	*receive = byte;
}

void wait_bank()
{
	while (!(*bank & 2));
}

void delay(int count)
{
	int i;

	for (; count > 0; count--, i++)
		for (i = 99; i > 0; i--);
}

void setup_trans(int mode)
{
	u8 r = get_receive() & ~TRANSFER;
	if (mode) r |= TRANSFER;
	set_receive(r);
}

void wait_bit_port(int mode)
{
	mode = mode ? 2 : 0;

	while (!(*port & mode));
}

void send_bit(u8 on)
{
	// clear condition
	u8 r = get_receive() & ~SENDBIT;
	// send either 1 or 0
	if (on) r |= SENDBIT;
	// transfer bit
	set_receive(r);
}

void send_byte(u8 byte)
{
	int i;

	for (i = 0; i < 8; i++, byte <<= 1)
	{
		setup_trans(1);
		send_bit(byte & 0x80);
		wait_bit_port(1);
		setup_trans(0);
		wait_bit_port(0);
	}
}

u8 get_byte()
{
	int i;
	u32 b;

	for (i = 0, b = 0; i < 8; i++)
	{
		b = (b << 1) & 0xFF;
		wait_bit_port(1);
		setup_trans(1);
		wait_bit_port(0);
		if(*port & 1) b |= 1;
		setup_trans(0);
	}

	return b;
}

// seems to calculate a crc
u32 get_checksum(u8 *in, int size)
{
	int i, j;
	u32 bit, word, ck;

	for (i = 0, bit = 0xFFFF; i < size; i += 2)
	{
		word = in[i] | (in[i + 1] << 8);
		for (j = 0; j < 16; j++)
		{
			ck = bit & 0x8000;
			bit <<= 1;
			if (word & 0x8000) bit |= 1;
			word <<= 1;
			if (ck) bit ^= 0x8005;
			bit &= 0xFFFF;
		}
	}

	return bit;
}

u32 send_cmd(u32 cmd)
{
	int i;
	u32 r;
	u8 buffer[8];

	// setup command stuff
	buffer[0] = w->v0d;
	buffer[1] = w->v0e;
	buffer[2] = cmd >> 24;
	buffer[3] = cmd >> 16;
	buffer[4] = cmd >> 8;
	buffer[5] = cmd;
	r = get_checksum(buffer, 6);
	// copy result
	buffer[6] = r >> 8;
	buffer[7] = r;

	for (i = 0; i < 8; i++)
		send_byte(buffer[i]);
	delay(10);

	// create return DWORD
	for (i = 0, r = 0; i < 4; i++)
	{
		r <<= 8;
		r |= get_byte();
	}

	return r;
}
