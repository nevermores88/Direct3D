#pragma once

#include "Terrain.h"
#include "Camera.h"

class CEx13_Terrain : public CBaseClass
{

	CTerrain*			m_pTerrain;
	CCamera*			m_pCamera;
public:
	CEx13_Terrain();
	~CEx13_Terrain();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

