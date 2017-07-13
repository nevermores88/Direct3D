#pragma once
class CShader_3dapi_09 : public CBaseClass
{
	/*struct Vertex
	{
	float x, y, z;
	float u, v;
	D3DCOLOR color;

	Vertex(){}
	Vertex(float _x, float _y, float _z, float _u, float _v, D3DCOLOR _color = 0)
	{
	x = _x;  y = _y; z = _z;
	u = _u; v = _v;
	color = _color;
	}

	enum { FVF = (D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_DIFFUSE) };
	};*/
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

	LPDIRECT3DVERTEXSHADER9	m_pShader;
	LPD3DXCONSTANTTABLE			m_pConstTable;
	LPDIRECT3DVERTEXDECLARATION9	m_pFVF;

	D3DXHANDLE m_hdViewProjMatrix;

	LPDIRECT3DTEXTURE9 m_pEarthTex;

	Vertex	m_Vertex[3];

public:
	CShader_3dapi_09();
	~CShader_3dapi_09();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

