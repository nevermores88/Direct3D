//---------------------------------------------------------------------------------------------------
//
//Description : Const
//Original File : s0v_05_const.zip
//
//---------------------------------------------------------------------------------------------------

#pragma once
class CShader_3dapi_01_07 : public CBaseClass
{
	struct Vertex
	{
		D3DXVECTOR3 p;
		D3DCOLOR color;

		Vertex(){}
		Vertex(float _x, float _y, float _z, D3DCOLOR _color)
		{
			p.x = _x; p.y = _y; p.z = _z;
			color = _color;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE) };
	};

	struct Index
	{
		WORD _0, _1, _2;
		Index(){}
		Index(WORD A, WORD B, WORD C) : _0(A), _1(B), _2(C){}
	};

	LPDIRECT3DVERTEXSHADER9	m_pVertexShader;
	LPDIRECT3DVERTEXDECLARATION9	m_pFVF;

	Vertex m_pVertices[24];
	Index	  m_pIndices[12];

public:
	CShader_3dapi_01_07();
	~CShader_3dapi_01_07();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

