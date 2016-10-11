#include "..\core.h"

typedef void (*exfnc)();

typedef struct tagKernel
{
	u32 f0000[5];		// 0x00 Exception handler
	u32 pad[8];			// 0x14 gap
	exfnc exception;	// 0x34 Exception function 0x34
} KERNEL;

extern void Exception();

extern KERNEL k[];

void install_exception()
{
	k->f0000[0] = 0xAC1A0038;	// sw k0, 0x38(0)
	k->f0000[1] = 0x3C1A0000 | ((const u32)Exception >> 16);	// li k0, Exception
	k->f0000[2] = 0x275A0000 | ((const u32)Exception & 0xFFFF);
	k->f0000[3] = 0x03400008;	// jr k0
	k->f0000[4] = 0x00000000;	// nop
}

void set_exception(exfnc func)
{
	k->exception = func;
}

void run_exception()
{
	if(k->exception)
		k->exception();
}

extern void install_event00(u32 p);
extern void install_event01(u32 p);
extern void install_event02(u32 p);
extern void install_event10(u32 p);
extern void install_event11(u32 p);
extern void install_event12(u32 p);

void main()
{
	reset_DCIC();
	set_exception(NULL);
	reset_receive();
	init();
	preparation();

	// check bios version?
	if (*(u32*)0x8D0 == 0xF00)
	{
		w->v0d = 5;
		w->v0e = 23;
		// event installation
		install_event00(send_cmd(0x8FB696EE));
		install_event01(send_cmd(0x8FB69BB2));
		install_event02(send_cmd(0x8FB68C8E));

		w->v0d = 2;
		w->v0e = 10;
		// possibly more stuff to install
		w->func[0] = send_cmd(0x14B03F53);
		w->func[1] = send_cmd(0x14B03A8F);
		w->func[2] = send_cmd(0x14B035EB);

		// kill some bios code not needed anymore
		*(u32*)0x3D44 = 0x03E00008;	// jr ra
		*(u32*)0x3D48 = 0x00000000;	// nop
	}
	// for 0x8D0 == 0xF40
	else
	{
		w->v0d = 1;
		w->v0e = 1;
		// event installation
		install_event00(send_cmd(0xF13EE761));
		install_event01(send_cmd(0xF13E3725));
		install_event02(send_cmd(0xF13E776F));

		w->v0d = 6;
		w->v0e = 30;
		// possibly more stuff to install
		w->func[0] = send_cmd(0x6F915BED);
		w->func[1] = send_cmd(0x6F915731);
		w->func[2] = send_cmd(0x6F91528D);

		// kill some bios code not needed anymore
		*(u32*)0x3DAC = 0x03E00008;	// jr ra
		*(u32*)0x3DB0 = 0x00000000;	// nop
	}

	// copy VMem manager into regular ram
	//memcpy(0x800A0000, prog_start, prog_end - prog_start);
	// enable cache back
	init_cache();
	// jump to manager
	__asm("j 0x800A0000");
}
