#pragma once
class CEx19_Effect02 : public CBaseClass
{
	LPD3DXEFFECT m_pShader;

	CTerrain* m_pTerrain;

	D3DXHANDLE m_hdTech;

public:
	CEx19_Effect02();
	~CEx19_Effect02();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

