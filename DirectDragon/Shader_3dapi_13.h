#pragma once
class CShader_3dapi_13 : public CBaseClass
{
	struct Vertex
	{
		float x, y, z;
		float nx, ny, nz;

		Vertex(){}
		Vertex(float _x, float _y, float _z, float _nx, float _ny, float _nz)
		{
			x = _x;  y = _y; z = _z;
			nx = _nx; ny = _ny; nz = _nz;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_NORMAL) };
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
	D3DXHANDLE m_hdRot;
	D3DXHANDLE m_hdLightDir;
	D3DXHANDLE m_hdLightDiffuse;

	Vertex*	m_Vertex;
	int	m_iNvx;

public:
	CShader_3dapi_13();
	~CShader_3dapi_13();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

