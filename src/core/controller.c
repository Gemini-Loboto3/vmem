#include "..\core.h"

#define DEAD_ZONE	80
#define ANALOG_OFF	127

#define FRONT		ANALOG_OFF+DEAD_ZONE
#define BACK		ANALOG_OFF-DEAD_ZONE

#define NO_PADS			0
#define PAD_ONE			1
#define PAD_TWO			2
#define BOTH_PADS		3
#define IS_DUAL_SHOCK	0x80

static	u_char align[6]={0,1,0xFF,0xFF,0xFF,0xFF};
static short connected = 0;			// No. controllers connected.
//static short currController = 0;	// Current active controller.

u16 Pad_type, Type;

// pad buffer data
ControllerPacket pad_data[2];
u8 transmissionBuffer[2][vibBuffLength];		//vibration buffers for pads
u8 pad_state=PAD_STATE_CHECK, pad_id=INVALID_PAD;

// buffer with pad reads, non-timed and timed
u32 pad_raw, pad_raw_t;
u32 pad_user, pad_user_t;
int pad_axis0_x, pad_axis0_y;
int pad_axis0_x_old, pad_axis0_y_old;
int pad_axis1_x, pad_axis1_y;
// this is just a support variable for timed reads
static u32 pad_old;

void InitControllers()
{
	PadInitDirect((u8*)&pad_data[DS_PAD_1],(u8*)&pad_data[DS_PAD_2]);
	PadSetAct(DS_PAD_1,transmissionBuffer[DS_PAD_1],vibBuffLength);
	PadStartCom();
}

void CheckControllers(void)
{
	int ret, i;
	int type, set;

	ret=PadGetState(DS_PAD_1);

	switch(ret)
	{
	case PadStateDiscon:	// no pad has been connected
		connected = NO_PADS;
		return;
	case PadStateFindPad:	// currently finding pad
		connected = NO_PADS;
		pad_state = PAD_STATE_CHECK;
		pad_id = INVALID_PAD;
		set = 0;
		// loop until it's stable or try next time
		for (i = 0; i < 100 || !set; i++)
		{
			switch(PadGetState(DS_PAD_1))
			{
			case PadStateStable:	// found dual shock or analog
				type=PadInfoMode(DS_PAD_1,InfoModeCurID,0);
				if(type)
				{
					// send transmission buffer
					PadSetAct(DS_PAD_1,transmissionBuffer[DS_PAD_1],vibBuffLength);
					// wait until buffer is aligned
					while(PadSetActAlign(DS_PAD_1,align)==0)
						for (ret = 0; ret < 6; ret++)
							VSync(0);
				}
				set=1;
				break;
			case PadStateFindCTP1:	// normal controller found
				set=1;
			}
		}
		connected=PAD_ONE;
		return;
	case PadStateStable:
		// pad is stable, send vibration signal!
		if(PadSetActAlign(DS_PAD_1,align))
			pad_state=PAD_STATE_STABLE;
		break;
	}

	VibCallback();
	type=PadInfoMode(DS_PAD_1,InfoModeCurExID,0);
	// dual shock detected with analog disabled or not
	if(type==ANALOG_PAD)
		connected=PAD_ONE | IS_DUAL_SHOCK;
	// other pad type
	else
	{
		type=PadInfoMode(DS_PAD_1,InfoModeCurID,0);
		switch(type)
		{
		case STD_PAD:			// standard pad
		case ANALOG_PAD:		// old analog or dual shock in digital mode
			connected=PAD_ONE;
			break;
		default:				// other pads will be detected as nothing plugged in
			connected=NO_PADS;
		}
	}
}

static __inline int Get_analog(int analog)
{
	return ((analog - 127) >> 3) << 3;
}

u16 GetPadValue()
{
	PadData read;
	CheckControllers();

	if(connected)
	{
		read=~pad_data[0].data.pad;
		// read axis data if it's a dual shock
		if(connected & IS_DUAL_SHOCK)
		{
			pad_axis0_x=(Get_analog(pad_data[0].data.joystick.left_x)+pad_axis0_x_old)/2;
			pad_axis0_y=(Get_analog(pad_data[0].data.joystick.left_y)+pad_axis0_y_old)/2;
			pad_axis1_x=Get_analog(pad_data[0].data.joystick.right_x);
			pad_axis1_y=Get_analog(pad_data[0].data.joystick.right_y);

			pad_axis0_x_old=pad_axis0_x;
			pad_axis0_y_old=pad_axis0_y;
		}
		else
		{
			pad_axis0_x=0;
			pad_axis0_y=0;
			pad_axis1_x=0;
			pad_axis1_y=0;
		}
		return ((read>>8)|(read<<8));
	}
	return 0;
}

// =============================================================================
// Registers pad input, converts it to 'standard' flags used by PadRead().
// This takes also care of timed reads, allowing easy programming of button
// pressions that require precise input, instead of repeated at insane frequency
// =============================================================================
void Read_pad()
{
	int i;
	// raw input and cache values
	u32 press, c, c_t;

	// cache button flags
	press=GetPadValue();

	// raw reads need no special treatment
	pad_raw=press;

	// make sure to ignore repeated pressions
	pad_raw_t=press & (press^pad_old);
	// assign new pression to old read to keep it updated
	pad_old=press;
}

// =============================================================================
// VIBRATION SUPPORT
// =============================================================================
#define DEFPAT(x)	{_countof((x)),(x)}

static int vib_phase=0, vib_frame=0;
static VIB_PATTERN *cur_vib=NULL;

int Vibrate(VIB_PATTERN *pattern)
{
#if USE_FULL_ANALOG
	if(pad_state==PAD_STATE_STABLE)
	{
		cur_vib=pattern;

		vib_phase=0;
		vib_frame=cur_vib->data[0].cnt;
		*(u16*)&transmissionBuffer[DS_PAD_1]=*(u16*)&cur_vib->data[0].vib;
	}
#endif
}

void VibCallback()
{
#if USE_FULL_ANALOG
	VIB_DATA *vdata;
	if(pad_state==PAD_STATE_STABLE)
	{
		// no vibration data active
		if(!cur_vib) return;
		// vibration pattern phase end, pull new request
		if(vib_frame==0)
		{
			// get following
			vib_phase++;
			// if we're at the end release vibration state
			if(cur_vib->cnt==vib_phase)
			{
				cur_vib=NULL;
				*(u16*)&transmissionBuffer[DS_PAD_1]=0;
				return;
			}
			// otherwise process next
			vdata=&cur_vib->data[vib_phase];
			vib_frame=vdata->cnt;
			*(u16*)&transmissionBuffer[DS_PAD_1]=*(u16*)&vdata->vib;
		}
		vib_frame--;
	}
#endif
}