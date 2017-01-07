#pragma once
class CShader_3dapi_03_01 : public CBaseClass
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

		enum { FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE|D3DFVF_TEX1) };
	};

	struct Index
	{
		WORD _0, _1, _2;

		Index() : _0(0), _1(1), _2(2){}
		Index(WORD a, WORD b, WORD c) : _0(a), _1(b), _2(c) {}
	};

	LPD3DXEFFECT m_pShader;

	LPDIRECT3DVERTEXDECLARATION9 m_pFVF;
	LPDIRECT3DTEXTURE9	m_pTex;

	Vertex m_pVertices[24];
	Index m_pIndices[12];

	D3DXMATRIX m_mtWorld;

public:
	CShader_3dapi_03_01();
	~CShader_3dapi_03_01();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

