#pragma once


class CEx06_Texture01 : public CBaseClass
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

	LPDIRECT3DVERTEXBUFFER9	VB;
	LPDIRECT3DTEXTURE9				m_Tex;

	char	m_cFilter;
	char	m_cMipFilter;

public:
	CEx06_Texture01();
	~CEx06_Texture01();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

