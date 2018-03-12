//---------------------------------------------------------------------------------------------------
//
//Description :  Vertex Blending 02
//Original File : s0v_08_vertex_skinning.zip
//
//---------------------------------------------------------------------------------------------------

#pragma once
class CShader_3dapi_01_20 : public CBaseClass
{
	struct Vertex
	{
		D3DXVECTOR3 p;
		float g[3];					//Weight
		BYTE m[4];					//행렬의 인덱스
		D3DCOLOR color;
		float u, v;

		Vertex(){}
		Vertex(float _x, float _y, float _z, float _u, float _v, D3DCOLOR _color = 0xFFFFFFFF)
		{
			p.x = _x; p.y = _y; p.z = _z;
			u = _u; v = _v;
			color = _color;
		}

		enum { FVF = (D3DFVF_XYZB4 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_DIFFUSE | D3DFVF_TEX1) };
	};

	LPDIRECT3DVERTEXSHADER9	m_pVertexShader;
	LPDIRECT3DVERTEXDECLARATION9	m_pFVF;

	LPDIRECT3DTEXTURE9 m_pTex;

	D3DXMATRIX m_mtWorld[40];

	Vertex m_pVertices[42];

	float m_fRandom;

public:
	CShader_3dapi_01_20();
	~CShader_3dapi_01_20();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

