#pragma once
class CEx03_Teapot : public CBaseClass
{
private:

	LPD3DXMESH				m_pTeapot;

	float							m_fY;

public:
	CEx03_Teapot();
	~CEx03_Teapot();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

