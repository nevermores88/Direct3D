#pragma once
class CGrid
{
	struct Vertex
	{
		D3DXVECTOR3 p;
		DWORD color;

		Vertex(){}
		Vertex(float _x, float _y, float _z, DWORD _color = 0xffffffff)
		{
			p.x = _x; p.y = _y; p.z = _z;
			color = _color;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE) };
	};

	LPDIRECT3DDEVICE9 m_pdev;

	int m_iVertexNum;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;

public:
	CGrid();
	~CGrid();

	HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	void Release();

	void Render();
	void Update();
};

