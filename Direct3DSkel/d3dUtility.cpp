#include "stdafx.h"
#include "d3dUtility.h"

DWORD d3d::VectortoRGBA(D3DXVECTOR3* v, FLOAT fHeight)
{
	DWORD r = (DWORD)(127.0f * v->x + 128.0f);
	DWORD g = (DWORD)(127.0f * v->y + 128.0f);
	DWORD b = (DWORD)(127.0f * v->z + 128.0f);
	DWORD a = (DWORD)(255.0f * fHeight);

	return((a << 24L) + (r << 16L) + (g << 8L) + (b << 0L));
};

float d3d::Linear(float v0, float v1, float t)
{
	return v0*(1.0f - t) + v1*t;
}
