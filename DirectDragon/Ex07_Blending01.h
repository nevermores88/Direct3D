#pragma once

class CEx07_Blending01 : public CBaseClass
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


	LPD3DXMESH							m_pTeapot;
	D3DMATERIAL9						m_TeapotMtrl;

	LPDIRECT3DVERTEXBUFFER9	m_pBkGndQuad;
	LPDIRECT3DTEXTURE9				m_pBkGndTex;
	D3DMATERIAL9						m_BkGndMtrl;

	D3DLIGHT9							m_Light;

public:
	CEx07_Blending01();
	~CEx07_Blending01();

public:
	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

