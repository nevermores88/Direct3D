//---------------------------------------------------------------------------------------------------
//
//Description : 멀티 텍스쳐링 03
//
//---------------------------------------------------------------------------------------------------
#pragma once
class CShader_3dapi_02_24 : public CBaseClass
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

	LPDIRECT3DPIXELSHADER9	m_pShader;
	LPD3DXCONSTANTTABLE	m_pConstTbl;

	LPDIRECT3DTEXTURE9		m_pTex1;
	LPDIRECT3DTEXTURE9		m_pTex2;

public:
	CShader_3dapi_02_24();
	~CShader_3dapi_02_24();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

