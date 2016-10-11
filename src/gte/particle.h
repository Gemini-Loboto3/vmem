#ifndef __PARTICLE_H
#define __PARTICLE_H

u8* Particle_allocate(ENTITY *ent, TM2_HEADER *mdl, u8 *pool);
void MappingParticle(TM2_HEADER *mdl, PARTS_W *pPart, int count, int Tpage, int Clut);
void FastParticle(void *ob, void *packet, CVECTOR *rgb, u32* ot);

#endif
