#pragma once
class CEx05_Directional : public CBaseClass
{
private:

	LPD3DXMESH				m_pObjects[4];
	D3DXMATRIX				m_mtWorlds[4];
	D3DMATERIAL9			m_Mtrls[4];

	D3DXVECTOR3			m_vDir;

	D3DLIGHT9				m_DirLight;

public:
	CEx05_Directional();
	~CEx05_Directional();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

