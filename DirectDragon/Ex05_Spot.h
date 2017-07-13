#pragma once
class CEx05_Spot : public CBaseClass
{
private:

	LPD3DXMESH				m_pObjects[4];
	D3DXMATRIX				m_mtWorlds[4];
	D3DMATERIAL9			m_Mtrls[4];

	D3DXVECTOR3			m_vSpotLightPos;
	D3DXVECTOR3			m_vSpotLightDir;

	D3DLIGHT9				m_SpotLight;

public:
	CEx05_Spot();
	~CEx05_Spot();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

