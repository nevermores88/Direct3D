#pragma once
#include "Cube.h"

class CEx07_Blending02 : public CBaseClass
{
	struct Vertex
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
	};


	LPDIRECT3DTEXTURE9				m_pCrateTex;
	D3DXMATRIX							m_CubeMat;

	LPDIRECT3DVERTEXBUFFER9	m_pBackDropVB;
	LPDIRECT3DTEXTURE9				m_pBackDropTex;

	CCube*									m_pCube;

public:
	CEx07_Blending02();
	~CEx07_Blending02();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();

	void	DataClear();
};

