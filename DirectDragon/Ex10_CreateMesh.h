#pragma once
#include <vector>
#include <fstream>

class CEx10_CreateMesh : public CBaseClass
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

	LPD3DXMESH							m_pMesh;
	DWORD									m_dwNumSubsets;
	LPDIRECT3DTEXTURE9				m_pTextures[3];

	float										m_fYRot;
	D3DXMATRIX							m_mtWorld;
	std::ofstream							m_OutFile;

public:
	CEx10_CreateMesh();
	~CEx10_CreateMesh();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

