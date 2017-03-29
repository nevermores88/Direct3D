#pragma once
class CShader_3dapi_02_12 : public CBaseClass
{
	struct Vertex
	{
		float x, y, z;
		DWORD color;

		Vertex(){}
		Vertex(float _x, float _y, float _z, DWORD _color)
		{
			x = _x; y = _y; z = _z;
			color = _color;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE) };
	};

	LPDIRECT3DVERTEXSHADER9	m_pShader;
	LPDIRECT3DVERTEXDECLARATION9	m_pFVF;
	LPD3DXCONSTANTTABLE	m_pConstTbl;

	D3DXMATRIX	m_mtWorld;

	int		m_iVerticesNum;
	Vertex*	m_pVertices;

public:
	CShader_3dapi_02_12();
	~CShader_3dapi_02_12();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

