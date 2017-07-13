#pragma once
#include <vector>

class CEx11_PMesh : public CBaseClass
{
	LPD3DXMESH							m_pSourceMesh;
	LPD3DXPMESH							m_pPMesh;
	std::vector<D3DMATERIAL9>	m_vMtrls;
	std::vector<LPDIRECT3DTEXTURE9>	m_vTextures;

	float										m_fYRot;
	D3DXMATRIX							m_mtWorld;

public:
	CEx11_PMesh();
	~CEx11_PMesh();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

