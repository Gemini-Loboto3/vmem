#include "..\core.h"

// initialize SPU to default values
void Snd_init()
{
	SpuReverbAttr r;

	// setup reverb depth to default
	r.mask = SPU_REV_DEPTHL | SPU_REV_DEPTHR;
	r.depth.left = r.depth.right = 64;

	// initialize SPU
	SpuInit();
	// kill callbacks
	SpuSetTransferCallback(NULL);
	// setup reverb work area
	SpuReserveReverbWorkArea(SPU_ON);
	SpuSetReverbModeType(SPU_REV_MODE_STUDIO_B);
	// kill reverb from BIOS screen
	while (SpuClearReverbWorkArea(3));
	// enable reverb
	SpuSetReverb(SPU_ON);
	SpuSetReverbDepth(&r);
	// set master volume levels
	SpuSetCommonMasterVolume(0x3fff, 0x3fff);
	// kill all voices
	SpuSetKey(SPU_OFF, SPU_ALLCH);
}
