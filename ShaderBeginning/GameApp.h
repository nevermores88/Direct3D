#pragma once
#include "GameCore.h"

#include "../D3DUtility/InputManager.h"
#include "../D3DUtility/CameraManager.h"

#define g_pGame CGameApp::GetInstance()

class CGameApp : public CGameCore
{
	static CGameApp* m_pGameApp;

	CBaseClass*	 m_pGameInstance;

	CInputManager*	m_pInputManager;
	CCameraManager* m_pCameraManager;

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

	void GetWidthHeight(int& iWidth, int& iHeight);
};