#pragma once
class CBasic_3dapi_07_06 : public CBaseClass
{
	struct Vertex
	{
		float x, y, z;
		D3DCOLOR color;

		Vertex(){}
		Vertex(float _x, float _y, float _z, D3DCOLOR _color)
		{
			x = _x; y = _y; z = _z;
			color = _color;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE) };
	};

	LPD3DXMESH m_Teapot;
	Vertex	m_pVertices[4];

public:
	CBasic_3dapi_07_06();
	~CBasic_3dapi_07_06();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

