#pragma once

#define SHOW_HOW_TO_USE_TCI

class CEx06_Textures : public CBaseClass
{
	struct Vertex
	{
		float x, y, z;
		D3DCOLOR color;
#ifndef SHOW_HOW_TO_USE_TCI
		float u, v;
#endif
		

		Vertex(){}
		Vertex(float _x, float _y, float _z, D3DCOLOR _color, float _u, float _v)
		{
			x = _x; y = _y; z = _z;
			color = _color;
#ifndef SHOW_HOW_TO_USE_TCI
			u = _u; v = _v;
#endif
		}

#ifdef SHOW_HOW_TO_USE_TCI
		enum { FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE ) };
#else
		enum { FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1) };
#endif
	};

	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DTEXTURE9 m_pTex;

public:
	CEx06_Textures();
	~CEx06_Textures();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

