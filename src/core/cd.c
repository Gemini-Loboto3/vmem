#include "..\core.h"

#define MAX_FILES	1024	// way more than we'll ever need

static PACK_FILE Pack_pool[MAX_FILES];	// where we store file allocators
static u8 buffer[2048];					// buffer for byte precise reads

static void Cd_read(u32 lba, u32 size, u32 *dest);

void Cd_init()
{
	u32 base_lba;
	CdlFILE f;
	int i, si;
	// a few useful pointers
	const u32 *buf = (u32*)__packable;
	PACK_HEADER * pH = (PACK_HEADER*)buf;
	PACK_FILE *pF = (PACK_FILE*)&pH[1];

	// initialize libcd
	CdInit();

	// search for our big file
	while (!CdSearchFile(&f, "\\BIGFILE.PAK;1"));
	// MSF2 to lba
	base_lba = CdPosToInt(&f.pos);

	// cache first sector of the header
	Cd_read(base_lba, 2048, (u32*)buf);
	// check if we need more
	if (pH->sectors > 1)
		Cd_read(base_lba + 1, pH->sectors - 1, (u32*)&((u8*)buf)[2048]);

	// copy processed indices to the internal table
	for (i = 0, si = pH->count; i < si; i++)
	{
		Pack_pool[i].lba = pF[i].lba + base_lba;
		Pack_pool[i].size = pF[i].size;
	}
}

// read a file from BIGFILE.PAK
u32 Cd_load_file(u32 index, u32 *dest)
{
	u32 lba, size;

	lba = Pack_pool[index].lba;
	size = Pack_pool[index].size;

	Cd_read(lba, size, dest);

	return size;
}

// perform raw reads on disk in blocking mode
void Cd_read(u32 lba, u32 size, u32 *dest)
{
	u32 sectors;
	CdlLOC loc;

	// obtain sector count
	sectors = GetAlign(size, 2048) / 2048;

	// convert lba to MSF and seek
	CdIntToPos(lba, &loc);
	CdControlB(CdlSetloc, (u8*)&loc, NULL);

	// byte precise read required
	if (size % 2048)
	{
		// does it need sector reads first?
		sectors--;
		if (sectors)
		{
			// read sectors into destination
			CdRead(sectors, dest, CdlModeSpeed);
			CdReadSync(0, NULL);
			// seek to last sectors
			CdIntToPos(lba + sectors, &loc);
			CdControlB(CdlSetloc, (u8*)&loc, NULL);
		}
		// read reminder in a separate buffer
		CdRead(1, (u32*)buffer, CdlModeSpeed);
		CdReadSync(0, NULL);
		// copy reminder to destination
		memcpy(&((u8*)dest)[sectors * 2048], buffer, size % 2048);
	}
	// read is exactly a multiple of sector size
	else
	{
		CdRead(sectors, dest, CdlModeSpeed);
		CdReadSync(0, NULL);
	}
}
