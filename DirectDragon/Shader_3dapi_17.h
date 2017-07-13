#pragma once
class CShader_3dapi_17 : public CBaseClass
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

	LPDIRECT3DPIXELSHADER9	m_pShader;
	LPD3DXCONSTANTTABLE			m_pConstTable;

	D3DXHANDLE		m_hdType;

	Vertex	m_Vertex[4];
	int		m_nType;

public:
	CShader_3dapi_17();
	~CShader_3dapi_17();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

