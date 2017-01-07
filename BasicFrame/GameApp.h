#pragma once
#include "GameCore.h"

#define g_pGame CGameApp::GetInstance()

class CGameApp : public CGameCore
{
	static CGameApp* m_pGameApp;

	CBaseClass*	 pGameInstance;

public:
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
	D3DXVECTOR3 m_Eye, m_At, m_Up;
	D3DVIEWPORT9 m_VP;

public:
	CGameApp();
	~CGameApp();

	virtual HRESULT  Create(HWND hWnd);
	virtual void Release();

	virtual void Render();
	virtual void Update();

	static CGameApp* GetInstance();

	void SetCamera();
	HWND GetHWND();
	const D3DVIEWPORT9* const GetViewPort();
};