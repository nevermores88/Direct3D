#pragma once
class CShader_3dapi_03 : public CBaseClass
{
	struct Vertex
	{
		float x, y, z;

		Vertex(){}
		Vertex(float _x, float _y, float _z)
		{
			x = _x;  y = _y; z = _z;
		}

		enum { FVF = (D3DFVF_XYZ) };
	};

	LPDIRECT3DVERTEXSHADER9	m_pShader;
	LPD3DXCONSTANTTABLE			m_pConstTable;
	LPDIRECT3DVERTEXDECLARATION9	m_pFVF;

	D3DXHANDLE	m_hdDiffuse;

	Vertex	m_Vertex[3];

public:
	CShader_3dapi_03();
	~CShader_3dapi_03();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

