#pragma once
class CEx03_Create : public CBaseClass
{
private:
	LPD3DXMESH					m_pObjects[5];
	D3DXMATRIX					m_mtObjWorld[5];
	D3DXMATRIX					m_mtCamera;

	float				m_fAngle;
	float				m_fCameraHeight;
	float				m_fCameraHeightDirection;

public:
	CEx03_Create();
	~CEx03_Create();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

