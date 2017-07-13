#pragma once
#include <vector>

class CEx11_XFile : public CBaseClass
{
	LPD3DXMESH							m_pMesh;
	std::vector<D3DMATERIAL9>	m_vMtrls;
	std::vector<LPDIRECT3DTEXTURE9>	m_vTextures;

	D3DLIGHT9							m_Light;

	float										m_fYRot;
	D3DXMATRIX							m_mtWorld;

public:
	CEx11_XFile();
	~CEx11_XFile();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

