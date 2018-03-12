//---------------------------------------------------------------------------------------------------
//
//Description :  Fog 01
//Original File : s0v_09_fog1_range.zip
//
//---------------------------------------------------------------------------------------------------

#pragma once
class CShader_3dapi_01_21 : public CBaseClass
{
	struct Vertex
	{
		D3DXVECTOR3 p;
		D3DCOLOR color;
		float u, v;

		Vertex(){}
		Vertex(float _x, float _y, float _z, float _u, float _v, D3DCOLOR _color = 0xFFFFFFFF)
		{
			p.x = _x; p.y = _y; p.z = _z;
			u = _u; v = _v;
			color = _color;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1) };
	};

	LPDIRECT3DVERTEXSHADER9	m_pVertexShader;
	LPDIRECT3DVERTEXDECLARATION9	m_pFVF;

	LPDIRECT3DTEXTURE9 m_pTex;

	D3DXMATRIX m_mtWorld;

	int m_iVertexNum;
	Vertex* m_pVertices;

public:
	CShader_3dapi_01_21();
	~CShader_3dapi_01_21();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

