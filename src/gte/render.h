#ifndef __RENDER_H
#define __RENDER_H

void Render(ENTITY *pEnt);

void FastTG3L(void *ob, void *packet, CVECTOR *rgb, u32* ot);
void FastTG4L(void *ob, void *packet, CVECTOR *rgb, u32* ot);
void FastG3L (void *ob, void *packet, CVECTOR *rgb, u32* ot);
void FastG4L (void *ob, void *packet, CVECTOR *rgb, u32* ot);

void calc_chrome_uvs_tri (MATRIX *trans, TM2_TRIANGLES *tri_obj, POLY_GT3 *tri);
void calc_chrome_uvs_quad(MATRIX *trans, TM2_QUADS *quad_obj,    POLY_GT4 *quad);

#endif
