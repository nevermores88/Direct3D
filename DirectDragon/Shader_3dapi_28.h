#pragma once
class CShader_3dapi_28 : public CBaseClass
{
	struct Vertex
	{
		float x, y, z;
		float u, v;

		Vertex(){}
		Vertex(float _x, float _y, float _z, float _u, float _v)
		{
			x = _x;  y = _y; z = _z;
			u = _u; v = _v;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_TEX1) };
	};

	LPDIRECT3DVERTEXSHADER9	m_pVertexShader;
	LPD3DXCONSTANTTABLE			m_pVertexConstTable;
	LPDIRECT3DVERTEXDECLARATION9	m_pFVF;

	LPDIRECT3DPIXELSHADER9	m_pPixelShader;
	LPD3DXCONSTANTTABLE		m_pPixelConstTable;

	LPDIRECT3DTEXTURE9			m_pTex;

	D3DXMATRIX m_mtWorld;

	Vertex*	m_Vertex;
	int m_iNvx;

public:
	CShader_3dapi_28();
	~CShader_3dapi_28();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

