#pragma once
class CTemp
{
	struct Vertex
	{
		float x, y, z;
		D3DCOLOR color;

		Vertex(){}
		Vertex(float _x, float _y, float _z, D3DCOLOR _color = 0)
		{
			x = _x;  y = _y; z = _z;
			color = _color;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE) };
	};

	LPDIRECT3DDEVICE9		m_pdev;

	Vertex	m_Vertex[3];
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
public:
	CTemp();
	~CTemp();

	void Create(LPDIRECT3DDEVICE9 pdev);
	void Release();

	void OnRender();
	void OnUpdate();
};

