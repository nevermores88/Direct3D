#pragma once

struct NomalVertex01
{
	float x, y, z;
	float nx, ny, nz;

	NomalVertex01(){}
	NomalVertex01(float _x, float _y, float _z, float _nx, float _ny, float _nz)
	{
		x = _x; y = _y; z = _z;
		nx = _nx; ny = _ny; nz = _nz;
	}

	enum { FVF = (D3DFVF_XYZ|D3DFVF_NORMAL) };
};

class CEx05_Light01 : public CBaseClass
{
private:

	D3DXMATRIX							m_WorldMat;
	LPDIRECT3DVERTEXBUFFER9	VB;

	D3DMATERIAL9 m_Mtrl;
	D3DLIGHT9	m_Light;
	//LPDIRECT3DINDEXBUFFER9		IB;

public:
	CEx05_Light01();
	~CEx05_Light01();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

