#pragma once
class CBasic_3dapi_08_03 : public CBaseClass
{
	struct Vertex
	{
		float x, y, z;
		DWORD color;
		float u, v;

		Vertex(){}
		Vertex(float _x, float _y, float _z, DWORD _color, float _u, float _v)
		{
			x = _x; y = _y; z = _z;
			color = _color;
			u = _u; v = _v;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1) };
	};

	struct Index
	{
		WORD _0, _1, _2;

		Index() : _0(0), _1(1), _2(2){}
		Index(WORD a, WORD b, WORD c) : _0(a), _1(b), _2(c) {}
	};

	int m_iNumVertex;
	int m_iNumIndex;

	Vertex* m_pVertices;
	Index* m_pIndices;

	LPDIRECT3DTEXTURE9 m_pTigerTex;
	D3DXVECTOR3 m_vTiger;

	LPDIRECT3DTEXTURE9		m_pFloorTex;
	Vertex m_pFloorVertices[4];

	LPD3DXMESH		m_pMeshS;

public:
	CBasic_3dapi_08_03();
	~CBasic_3dapi_08_03();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

