#pragma once
class CBasic_3dapi_08_06 : public CBaseClass
{
	struct Vertex
	{
		D3DXVECTOR3 p;

		Vertex(){}
		Vertex(D3DXVECTOR3 _p)
		{
			p = _p;
		}

		enum { FVF = (D3DFVF_XYZ) };
	};

	struct VertexColor
	{
		float x, y, z;
		DWORD color;

		VertexColor(){}
		VertexColor(float _x, float _y, float _z, DWORD _color = 0xFF006600)
		{
			x = _x; y = _y; z = _z;
			color = _color;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE) };
	};

	struct VertexRHW
	{
		D3DXVECTOR4 p;
		DWORD color;

		VertexRHW(){}
		VertexRHW(D3DXVECTOR4 _p, DWORD _color)
		{
			p = _p;
			color = _color;
		}

		enum { FVF = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE) };
	};

	LPD3DXMESH	m_pMesh;
	VertexRHW	m_pRHW[4];
	D3DXMATRIX m_mtObj;

	//ÇÊµå
	VertexColor		m_pFieldVertices[4];

	int iCase;

public:
	CBasic_3dapi_08_06();
	~CBasic_3dapi_08_06();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

