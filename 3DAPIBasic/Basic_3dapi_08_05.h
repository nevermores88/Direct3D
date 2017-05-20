#pragma once
class CBasic_3dapi_08_05 : public CBaseClass
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

	Vertex*	m_pVertices;
	VertexRHW m_pRHW[4];
	D3DXMATRIX	m_mtObj;

public:
	CBasic_3dapi_08_05();
	~CBasic_3dapi_08_05();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

