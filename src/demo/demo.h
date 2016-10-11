#ifndef __DEMO_H
#define __DEMO_H

typedef struct tagDemoEnv
{
	u8 R_0,
		R_1;
	u16 Timer0;
} DEMO_ENV;

enum
{
	DMMAIN_DASHBOARD,
	DMMAIN_INPUT,
	DMMAIN_ROTATE,
	DMMAIN_INFO
};

void Demo_init();
void Demo();

#endif
