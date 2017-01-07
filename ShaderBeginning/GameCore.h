#pragma once
class CGameCore
{
protected:
	LPDIRECT3D9 m_pd3d;
	LPDIRECT3DDEVICE9 m_pdev;
	HWND m_hWnd;

public:
	CGameCore();
	~CGameCore();

protected:
	virtual HRESULT  Create(HWND hWnd);
	virtual void Release();

	virtual void Render() = 0;
	virtual void Update() = 0;
};

