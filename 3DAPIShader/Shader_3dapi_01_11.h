//---------------------------------------------------------------------------------------------------
//
//Description : Texture 04
//Original File : s0v_06_tex_vtx.zip
//
//---------------------------------------------------------------------------------------------------

#pragma once
class CShader_3dapi_01_11 : public CBaseClass
{
	struct Vertex
	{
		D3DXVECTOR3 p;
		float u, v;

		Vertex(){}
		Vertex(float _x, float _y, float _z, float _u, float _v)
		{
			p.x = _x; p.y = _y; p.z = _z;
			u = _u; v = _v;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_TEX1) };
	};

	struct Index
	{
		WORD _0, _1, _2;
		Index(){}
		Index(WORD A, WORD B, WORD C) : _0(A), _1(B), _2(C){}
	};

	LPDIRECT3DVERTEXSHADER9	m_pVertexShader;
	LPDIRECT3DVERTEXDECLARATION9	m_pFVF;

	int m_iVertexNum;
	int m_iIndexNum;
	Vertex* m_pVertices;
	Index* m_pIndices;

	LPDIRECT3DTEXTURE9 m_pTex;

public:
	CShader_3dapi_01_11();
	~CShader_3dapi_01_11();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

