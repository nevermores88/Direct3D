#pragma once
class CSurface_3dapi_01 : public CBaseClass
{
	struct Vertex
	{
		float x, y, z, w;
		D3DCOLOR color;
		float u, v;

		Vertex(){}
		Vertex(float _x, float _y, float _z, D3DCOLOR _color, float _u, float _v)
		{
			x = _x; y = _y; z = _z; w = 1;
			color = _color;
			u = _u; v = _v;
		}

		enum { FVF = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1) };
	};

	LPDIRECT3DTEXTURE9		m_pTex;
	LPDIRECT3DSURFACE9		m_pSurf;
	LPDIRECT3DSURFACE9 m_pShadowDepthStencil;

	int		m_iTexWidth;

	Vertex m_pVertices[4];

	LPD3DXMESH	m_pTeapot;

public:
	CSurface_3dapi_01();
	~CSurface_3dapi_01();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

