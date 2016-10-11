#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#define TRANSFER	0x20
#define SENDBIT		0x10

// taken from assembly
extern u32 get_sr();
extern u32 set_sr(u32 sr);

// starts at 0x3db4
typedef struct tagWork
{
	u32 func[3];	// this is always jr ra : nop
	u8 v0c,			// mirrors receiver value
		v0d, v0e, v0f,
		v10, v11, v12, v13;
	u16 v14, v16, v18, v1a,
		v1c, v1e, v20,
		v22, v24,	// copies of w28 and w2C
		v26,
		// some buffers, used as byte arrays somewhere
		v28, v2a, v2c, v2e;
	s32 v30, v34;	// counters of some sort
	u32 v38;
} WORK;

// hardware ports
extern volatile u8 *receive, *bank, *port;
// internal variables from BIOS RAM
extern volatile WORK *w;

// receiver
void reset_receive();
u8 get_receive();
u8 set_receive(u8 byte);

// for sending stuff
void setup_trans(int mode);
void send_bit(u8 on);
void send_byte(u8 byte);
u8 get_byte();

// timing
void delay(int count);
void wait_bit_port(int mode);
void wait_bank();

// fetches stuff from other roms apparently
u32 send_cmd(u32 cmd);

#endif
