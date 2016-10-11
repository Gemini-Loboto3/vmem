#ifndef __CD_H
#define __CD_H

typedef struct tagPackHeader
{
	u32 count,
		sectors;
} PACK_HEADER;

typedef struct tagPackFile
{
	u32 lba,
		size;
} PACK_FILE;

void Cd_init();
u32  Cd_load_file(u32 index, u32 *dest);

#endif
