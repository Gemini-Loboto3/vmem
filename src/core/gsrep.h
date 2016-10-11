#ifndef __GS_REP
#define __GS_REP

// bit variables used by GsSetDisplay
#define RESMODE_VERTICAL	0
#define RESMODE_SIDEWAYS	BIT(0)
#define RESMODE_INTERLACE	BIT(1)

#define OT_SH				10				// linked list depth
#define OT_SIZE				(1 << OT_SH)	// turn depth into an actual value

#define PACKET_SIZE			15*1024			// 15 KB buffer

enum GsClearMode
{
	GCM_CLEAR,		// clear screen with CVECTOR Clear_rgb
	GCM_NONE		// do not clear
};

typedef struct tagGsEnv
{
	// rendering related structures
	DRAWENV Draw_env[2];
	DISPENV Disp_env[2];
	DRAWENV *pDraw;					// current DRAWENV
	DISPENV *pDisp;					// current DISPENV
	u32 *pOt;						// current OTag pointer
	u32 *pGfx;						// current packet seek
	u32 OTag[2][OT_SIZE];			// sort tables
	u16 OTag_id;					// current OTag index, flip every frame
	u8 VSync_rate;					// 0 = 60 fps, 2 = 30 fps
	u8 Clear_mode;					// see GsClearMode
	CVECTOR Clear_rgb;				// clear color
	s16 Screen_x, Screen_y;			// option menu adjustments
	u16 Screen_w, Screen_h;			// screen size, internal usage
	u32 *Gfx_alloc[2];				// packet allocators
} GS_ENV;

// makes GS_ENV globally visible
extern volatile GS_ENV Gs;

// prototypes
void GsSetDisplay(int x, int y, int w, int h, u32 mode);
void GsInitPacket();
void GsResetEnv();
void GsBeginDraw();
void GsEndDraw();

// retrieve current primitive pointer
static __inline void *gfxGetPtr()
{
	return Gs.pGfx;
}

// update packet allocator
static __inline void gfxSetPtr(void* p)
{
	Gs.pGfx = (u32*)p;
}

// a few helpers to get access to structures
static __inline u32* GsGetOTag()		{ return Gs.pOt; }
static __inline int  GsGetBufferIndex() { return Gs.OTag_id; }

static __inline RECT* GsGetTexWin()		{ return &Gs.pDraw->tw; }
static __inline u32   GsGetTPage()		{ return Gs.pDraw->tpage; }
static __inline RECT* GsGetDisplay()	{ return &Gs.pDisp->disp; }
static __inline RECT* GsGetScreen()		{ return &Gs.pDisp->screen; }

static __inline void  GsSetClearMode(int mode) { Gs.Clear_mode = mode; }
static __inline void  GsSetClearColor(u32 rgb) { *(u32*)&Gs.Clear_rgb = rgb; }

#endif
