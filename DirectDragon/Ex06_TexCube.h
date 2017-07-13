#pragma once
#include "Cube.h"

class CEx06_TexCube : public CBaseClass
{
private:
	/*struct Vertex
	{
		float x, y, z;
		float nx, ny, nz;
		float u, v;

		Vertex(){}
		Vertex(float _x, float _y, float _z, float _nx, float _ny, float _nz, float _u, float _v)
		{
			x = _x; y = _y; z = _z;
			nx = _nx; ny = _ny; nz = _nz;
			u = _u; v = _v;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1) };
	};*/

	LPDIRECT3DTEXTURE9				m_pTex;
	D3DLIGHT9				m_Light;

	CCube*						m_pBox;
	D3DXMATRIX				m_mtWorld;

	float			m_fAngle;
	float			m_fHeight;

public:
	CEx06_TexCube();
	~CEx06_TexCube();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

