#pragma once
#include "Camera.h"

class CEx12_Camera : public CBaseClass
{
private:
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

	LPDIRECT3DVERTEXBUFFER9	m_pFloorVB;
	LPDIRECT3DTEXTURE9				m_pTex;
	LPD3DXMESH							m_pPillarMesh;

	CCamera			m_Camera;

public:
	CEx12_Camera();
	~CEx12_Camera();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();

	void DrawBasicScene();
};

