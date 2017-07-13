#pragma once
class CShader_3dapi_26 : public CBaseClass
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

	LPD3DXCONSTANTTABLE		m_pConstTable;

	Vertex	m_pVertex[4];
	LPDIRECT3DTEXTURE9			m_pTex;

public:
	CShader_3dapi_26();
	~CShader_3dapi_26();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

