#pragma once
class CBasic_3dapi_07_04 : public CBaseClass
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

	struct Index
	{
		WORD _0, _1, _2;

		Index() : _0(0), _1(1), _2(2){}
		Index(WORD a, WORD b, WORD c) : _0(a), _1(b), _2(c) {}
	};

	Vertex m_pVertices1[8 + 1];
	Vertex m_pVertices2[8 + 1];

	Index m_pIndicies[8];

public:
	CBasic_3dapi_07_04();
	~CBasic_3dapi_07_04();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

