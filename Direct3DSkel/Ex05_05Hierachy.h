#pragma once
class CEx05_05Hierachy : public CBaseClass
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

		enum { FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE ) };
	};

	struct Index
	{
		WORD _0, _1, _2;
	};

	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;

	D3DXMATRIX m_mtTMParent;	//부모의 TM
	D3DXMATRIX m_mtRParent;		//부모의 회전행렬

	D3DXMATRIX m_mtTMChild;		//자식의 TM
	D3DXMATRIX m_mtRChild;		//자식의 회전행렬

public:
	CEx05_05Hierachy();
	~CEx05_05Hierachy();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();

	void DrawMesh(D3DXMATRIX* pMat);
};

