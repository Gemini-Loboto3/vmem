/* ================================================ */
/* Generic GTE environment and functioning.			*/
/* ================================================ */
#include "..\core.h"

// identity matrix
MATRIX M_id =
{
	// rotation
	ONE, 0, 0,
	0, ONE, 0,
	0, 0, ONE,
	// translation
	0, 0, 0
};

GTE_ENV Gte;

void Gte_set_ambient(int r, int g, int b)
{
	r *= 16, g *= 16, b *= 16;
	Gte.Ambient[0] = r;
	Gte.Ambient[1] = g;
	Gte.Ambient[2] = b;

	gte_ldbkdir(r, g, b);
}

void Gte_upd_ambient()
{
	gte_ldbkdir(Gte.Ambient[0], Gte.Ambient[1], Gte.Ambient[2]);
}

void Gte_set_fog_color(int r, int g, int b)
{
	r *= 16, g *= 16, b *= 16;
	Gte.Fog_color[0] = r;
	Gte.Fog_color[1] = g;
	Gte.Fog_color[2] = b;

	gte_ldfcdir(r, g, b);
}

void Gte_upd_fog_color()
{
	gte_ldfcdir(Gte.Fog_color[0], Gte.Fog_color[1], Gte.Fog_color[2]);
}

void Gte_set_fog_ranges(int near, int far)
{
	Gte.Fog_near = near,
		Gte.Fog_far = far;
	SetFogNearFar(near, far, Gte.H);
}

void Gte_upd_fog_ranges()
{
	SetFogNearFar(Gte.Fog_near, Gte.Fog_far, Gte.H);
}

static void Set_flat_light(const int index, VECTOR *lpos, const CVECTOR *col, const int Mag)
{
	int len;

	len = SquareRoot0(lpos->vx*lpos->vx + lpos->vy*lpos->vy + lpos->vz*lpos->vz);
	if (len == 0) return;

	// set position row
	Gte.M_ll.m[index][0] = ((-lpos->vx) << 12) / len * Mag;
	Gte.M_ll.m[index][1] = ((-lpos->vy) << 12) / len * Mag;
	Gte.M_ll.m[index][2] = ((-lpos->vz) << 12) / len * Mag;

	// set color column
	Gte.M_lc.m[0][index] = (col->r*ONE) / 0xFF;
	Gte.M_lc.m[1][index] = (col->g*ONE) / 0xFF;
	Gte.M_lc.m[2][index] = (col->b*ONE) / 0xFF;
}

void Gte_set_light(const VECTOR *pos, const GTE_LIGHT *l)
{
	int mag, i, intensity, power,
		x, y, z;
	VECTOR lpos;
	CVECTOR cvec;

	for (i = 0; i < LIGHT_MAX; i++)
	{
		switch (l->Mode[i])
		{
		case LM_FALL:
			// calculate local range
			x = (pos->vx - l->Pos[i][0]);
			y = (pos->vy - l->Pos[i][1]);
			z = (pos->vz - l->Pos[i][2]);
			lpos.vx = x, lpos.vy = y, lpos.vz = z;
			// get magnitude for intensity check
			mag = SquareRoot0(x * x + z * z);
			power = l->Range[i] >> 1;
			// attenuation
			if (power != 0)
			{
				intensity = power - mag;
				if (intensity < 0) intensity = 0;
				cvec.r = intensity * l->Col[i][0] / power;
				cvec.g = intensity * l->Col[i][1] / power;
				cvec.b = intensity * l->Col[i][2] / power;
			}
			else *(u32*)&cvec.r = 0;
			// send data
			Set_flat_light(i, &lpos, &cvec, l->Mag);
			break;
		case LM_OMNI:
			Set_flat_light(i, (VECTOR*)l->Pos[i], (CVECTOR*)&l->Col[i], l->Mag);
			break;
		}
	}

	// set ambient color
	gte_ldbkdir(l->Ambient[0] * 16, l->Ambient[1] * 16, l->Ambient[2] * 16);
}

// rotation x cache
MATRIX Camera_rotx =
{
	ONE,	0,		0,
	0,		ONE,	0,
	0,		0,		ONE,
	0,		0,		0
},
// rotation y cache
Camera_roty =
{
	ONE,	0,		0,
	0,		ONE,	0,
	0,		0,		ONE,
	0,		0,		0
};

void Gte_camera_set()
{
	VECTOR v0;
	int x0, y0, y1, z0;
	int len_x, len_y, len_z;
	int mag, mag_xz;
	int normal_y, normal_xz;
	GTE_CAM *camera;

	camera = Gte.pCamera;

#define m1 Camera_rotx
#define m2 Camera_roty

	// cache frequently used coordinates
	x0 = camera->Look[0],
		y0 = camera->Look[1],
		z0 = camera->Look[2];
	y1 = camera->At[1];

	len_x = camera->At[0] - x0;
	len_y = camera->At[1] - y0;
	len_z = camera->At[2] - z0;

	// calculate normalized magnitude
	mag = SquareRoot0(len_x*len_x + len_y*len_y + len_z*len_z);
	if (mag) normal_y = (y0 - y1 << 12) / mag;
	else normal_y = y0 - y1;

	// calculate normalized xz magnitude
	mag_xz = SquareRoot0(len_x*len_x + len_z*len_z);
	if (mag) normal_xz = (mag_xz << 12) / mag;
	else normal_xz = mag_xz;

	// set up x-rotation matrix
	m1.m[1][1] = normal_xz, m1.m[1][2] = normal_y;
	m1.m[2][1] = -normal_y, m1.m[2][2] = normal_xz;

	if (mag_xz)
	{
		// normalize y rotations
		int rot_x = (len_x << 12) / mag_xz;
		int rot_z = (len_z << 12) / mag_xz;
		// set up y-rotation matrix
		m2.m[0][0] = rot_z, m2.m[0][2] = -rot_x;
		m2.m[2][0] = rot_x, m2.m[2][2] = rot_z;
		// multiply the two rotated matrices together
		MulMatrix0(&m1, &m2, &Gte.M_view);
	}

	v0.vx = -x0, v0.vy = -y0, v0.vz = -z0;
	ApplyMatrixLV(&Gte.M_view, &v0, (VECTOR*)Gte.M_view.t);
}

void Gte_rot_svecs(SVECTOR *dir, SVECTOR *in, SVECTOR *out, int count)
{
	int i;
	MATRIX m;

	// build rotation and apply
	RotMatrix(dir, &m);
	for (i = 0; i < count; i++)
		ApplyMatrixSV(&m, &in[i], &out[i]);
}
