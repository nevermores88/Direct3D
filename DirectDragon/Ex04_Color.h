#pragma once

struct ColorVertex
{
	float x, y, z;
	D3DCOLOR color;

	ColorVertex(){}
	ColorVertex(float _x, float _y, float _z, D3DCOLOR _color = 0)
	{
		x = _x; y = _y; z = _z;
		color = _color;
	}

	enum { FVF = (D3DFVF_XYZ|D3DFVF_DIFFUSE) };
};

class CEx04_Color : public CBaseClass
{
private:

	D3DXMATRIX							m_WorldMat;
	LPDIRECT3DVERTEXBUFFER9	VB;
	//LPDIRECT3DINDEXBUFFER9		IB;

public:
	CEx04_Color();
	~CEx04_Color();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

