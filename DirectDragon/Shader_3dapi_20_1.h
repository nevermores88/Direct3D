#pragma once
class CShader_3dapi_20_1 : public CBaseClass
{
	struct Vertex
	{
		float x, y, z;
		float u, v;
		D3DCOLOR color;

		Vertex(){}
		Vertex(float _x, float _y, float _z, float _u, float _v)
		{
			x = _x;  y = _y; z = _z;
			u = _u; v = _v;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_TEX1) };
	};

	LPDIRECT3DPIXELSHADER9	m_pShader;
	LPD3DXCONSTANTTABLE		m_pConstTable;
	LPDIRECT3DTEXTURE9			m_pTex;

	D3DXMATRIX m_mtWorld;
	D3DXMATRIX m_mtView;
	D3DXMATRIX m_mtProj;

	Vertex*	m_Vertex;
	int m_iNvx;

public:
	CShader_3dapi_20_1();
	~CShader_3dapi_20_1();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

