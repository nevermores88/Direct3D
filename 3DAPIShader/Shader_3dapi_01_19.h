//---------------------------------------------------------------------------------------------------
//
//Description :  Vertex Blending 01
//Original File : s0v_08_vertex_blending.zip
//
//---------------------------------------------------------------------------------------------------

#pragma once
class CShader_3dapi_01_19 : public CBaseClass
{
	struct Vertex
	{
		D3DXVECTOR3 p;
		float g;					//Weight
		D3DCOLOR color;
		float u, v;

		Vertex(){}
		Vertex(float _x, float _y, float _z, float _u, float _v, float _g, D3DCOLOR _color = 0xFFFFFFFF)
		{
			p.x = _x; p.y = _y; p.z = _z;
			u = _u; v = _v;
			g = _g;
			color = _color;
		}

		enum { FVF = (D3DFVF_XYZB1 | D3DFVF_DIFFUSE | D3DFVF_TEX1) };
	};

	LPDIRECT3DVERTEXSHADER9	m_pVertexShader;
	LPDIRECT3DVERTEXDECLARATION9	m_pFVF;

	LPDIRECT3DTEXTURE9 m_pTex;

	D3DXMATRIX m_mtWorld0;
	D3DXMATRIX m_mtWorld1;

	Vertex m_pVertices[42];

	float m_fRandom;

public:
	CShader_3dapi_01_19();
	~CShader_3dapi_01_19();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

