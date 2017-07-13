#pragma once
#include "BoundingBox.h"
#include "BoundingSphere.h"

class CEx11_BoundingVolumes : public CBaseClass
{
	LPD3DXMESH							m_pMesh;
	//LPD3DXPMESH							m_pPMesh;
	std::vector<D3DMATERIAL9>	m_vMtrls;
	std::vector<LPDIRECT3DTEXTURE9>	m_vTextures;

	LPD3DXMESH							m_pSphereMesh;
	LPD3DXMESH							m_pBoxMesh;

	float										m_fYRot;
	D3DXMATRIX							m_mtWorld;

	bool										m_bRenderBoundingSphere;

public:
	CEx11_BoundingVolumes();
	~CEx11_BoundingVolumes();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

