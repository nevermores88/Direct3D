#pragma once
class CEx05_Point : public CBaseClass
{
private:

	LPD3DXMESH				m_pObjects[4];
	D3DXMATRIX				m_mtWorlds[4];
	D3DMATERIAL9			m_Mtrls[4];

	D3DXVECTOR3			m_vPointLightPos;

	D3DLIGHT9				m_PointLight;

public:
	CEx05_Point();
	~CEx05_Point();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

