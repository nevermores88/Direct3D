#pragma once
class CCube
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

	LPDIRECT3DDEVICE9     m_pdev;
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;

public:
	CCube();
	~CCube();

	void Create(LPDIRECT3DDEVICE9	pdev);
	void Release();
	void Draw(D3DXMATRIX* world, const D3DMATERIAL9* mtrl, LPDIRECT3DTEXTURE9 tex);
};

