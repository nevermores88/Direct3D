#pragma once
class CShader_3dapi_12 : public CBaseClass
{
	struct Vertex
	{
		float x, y, z;
		D3DCOLOR color;

		Vertex(){}
		Vertex(float _x, float _y, float _z, D3DCOLOR _color = 0)
		{
			x = _x;  y = _y; z = _z;
			color = _color;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE) };
	};

	LPDIRECT3DVERTEXSHADER9	m_pShader;
	LPD3DXCONSTANTTABLE			m_pConstTable;
	LPDIRECT3DVERTEXDECLARATION9	m_pFVF;

	D3DXMATRIX	m_mtWorld;
	D3DXMATRIX m_mtView;
	D3DXMATRIX m_mtProj;

	D3DXHANDLE m_hdWorld;
	D3DXHANDLE m_hdView;
	D3DXHANDLE m_hdProj;
	D3DXHANDLE m_hdFogColor;
	D3DXHANDLE m_hdFogEnd;
	D3DXHANDLE m_hdFogBegin;

	Vertex*	m_Vertex;
	int	m_iNvx;

public:
	CShader_3dapi_12();
	~CShader_3dapi_12();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

