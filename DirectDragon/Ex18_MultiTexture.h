#pragma once
class CEx18_MultiTexture : public CBaseClass
{
	struct Vertex
	{
		float x, y, z;
		float u0, v0;
		float u1, v1;
		float u2, v2;

		Vertex(){}
		Vertex(float _x, float _y, float _z, float _u0, float _v0, float _u1, float _v1, float _u2, float _v2)
		{
			x = _x;  y = _y; z = _z;
			u0 = _u0; v0 = _v0;
			u1 = _u1; v1 = _v1;
			u2 = _u2; v2 = _v2;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_TEX3) };
	};

	LPDIRECT3DPIXELSHADER9	m_pMultiTexPS;
	LPD3DXCONSTANTTABLE		m_pMultiTexCT;

	LPDIRECT3DVERTEXBUFFER9	m_pQuadVB;

	LPDIRECT3DTEXTURE9			m_pBaseTex;
	LPDIRECT3DTEXTURE9			m_pSpotLightTex;
	LPDIRECT3DTEXTURE9			m_pStringTex;

	D3DXHANDLE		m_hdBaseTex;
	D3DXHANDLE		m_hdSpotLight;
	D3DXHANDLE		m_hdStringTex;

	D3DXCONSTANT_DESC		m_BaseTexDesc;
	D3DXCONSTANT_DESC		m_SpotLightTextDesc;
	D3DXCONSTANT_DESC		m_StringTexDesc;

public:
	CEx18_MultiTexture();
	~CEx18_MultiTexture();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

