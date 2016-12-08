#pragma once
#include "Ex05_02Billboard.h"

class CEx05_03Billboard : public CBaseClass
{
	struct Vertex
	{
		float x, y, z;
		D3DCOLOR color;
		float u, v;

		Vertex(){}
		Vertex(float _x, float _y, float _z, D3DCOLOR _color, float _u, float _v)
		{
			x = _x; y = _y; z = _z;
			color = _color;
			u = _u; v = _v;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1) };
	};

	CEx05_02Water* m_pWater;
	CEx05_02Camera* m_pCam;

	LPDIRECT3DTEXTURE9		m_pTexBillboard[4];

	D3DXMATRIX m_mtAni;
	D3DXMATRIX m_mtWorld;
	D3DXMATRIX m_mtView;
	D3DXMATRIX m_mtProj;

	DWORD m_dwMouseX;
	DWORD m_dwMouseY;
	BOOL m_bBillboard;
	BOOL	 m_bWireframe;

	DWORD m_dwMousePos[2];

public:
	CEx05_03Billboard();
	~CEx05_03Billboard();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

