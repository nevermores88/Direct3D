#pragma once
class CShader_3dapi_07 : public CBaseClass
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

	//D3DXHANDLE	m_hdViewMatrix;
	D3DXHANDLE m_hdViewProjMatrix;

	Vertex	m_Vertex[3];
public:
	CShader_3dapi_07();
	~CShader_3dapi_07();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

