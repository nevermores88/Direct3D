#pragma once
class CBasic_3dapi_07_05 : public CBaseClass
{
	struct Vertex1
	{
		float x, y, z;
		D3DCOLOR color;

		Vertex1(){}
		Vertex1(float _x, float _y, float _z, D3DCOLOR _color)
		{
			x = _x; y = _y; z = _z;
			color = _color;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE) };
	};

	struct Vertex2
	{
		float x, y, z;
		D3DCOLOR color;
		float u, v;

		Vertex2(){}
		Vertex2(float _x, float _y, float _z, D3DCOLOR _color, float _u, float _v)
		{
			x = _x; y = _y; z = _z;
			color = _color;
			u = _u; v = _v;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1) };
	};

	Vertex1 m_pVertices1[4];
	Vertex2 m_pVertices2[4];
	LPDIRECT3DTEXTURE9 m_pTex;

	DWORD	m_dwFunc;
	int			m_iRef;

public:
	CBasic_3dapi_07_05();
	~CBasic_3dapi_07_05();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

