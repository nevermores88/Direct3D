#pragma once
class CBasic_3dapi_07_02 : public CBaseClass
{
	struct Vertex
	{
		float x, y, z;
		D3DCOLOR color;
		float u, v;

		Vertex(){}
		Vertex(float _x, float _y, float _z, D3DCOLOR _color, float _u, float _v)
		{
			x = _x; y = _y; z = _z;
			color = _color;
			u = _u; v = _v;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1) };
	};

	Vertex m_pVertices[4];
	LPDIRECT3DTEXTURE9 m_pTex;

public:
	CBasic_3dapi_07_02();
	~CBasic_3dapi_07_02();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

