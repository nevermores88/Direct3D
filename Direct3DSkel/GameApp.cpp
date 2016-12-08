#include "stdafx.h"
#include "GameApp.h"

#include "Ex06_Textures.h"
#include "Ex07_Mesh.h"
#include "Ex08_IndexBuffer.h"

#include "Ex05_01LightMapping.h"
#include "Ex05_02Billboard.h"
#include "Ex05_03Billboard.h"
#include "Ex05_04NormalMapping.h"

CGameApp* CGameApp::m_pGameApp = NULL;

CGameApp::CGameApp()
{
}


CGameApp::~CGameApp()
{
}

HRESULT CGameApp::Create(HWND hWnd)
{
	HRESULT ret = CGameCore::Create(hWnd);
	SetCamera();

	pGameInstance = new CEx05_03Billboard();
	pGameInstance->Create(m_pdev);

	return ret;
}

void CGameApp::Release()
{
	CGameCore::Release();

	if (pGameInstance)
	{
		pGameInstance->Release();
		delete pGameInstance;
		pGameInstance = NULL;
	}

	if (!m_pGameApp)
	{
		delete m_pGameApp;
		m_pGameApp = NULL;
	}
}

void CGameApp::Render()
{
	if (NULL == m_pdev)
		return;

	m_pdev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 128, 200), 1.0f, 0);

	if (SUCCEEDED(m_pdev->BeginScene()))
	{
		if (pGameInstance)
			pGameInstance->Render();

		m_pdev->EndScene();
	}

	m_pdev->Present(NULL, NULL, NULL, NULL);
}

void CGameApp::Update()
{
	if (pGameInstance)
		pGameInstance->Update();
}

void CGameApp::SetCamera()
{
	if (m_pdev)
	{
		RECT rect;
		
		GetClientRect(m_hWnd, &rect);

		m_VP.X = 0;
		m_VP.Y = 0;
		m_VP.Width = rect.right - rect.left;
		m_VP.Height = rect.bottom - rect.top;
		m_VP.MinZ = 0.0f;
		m_VP.MaxZ = 1.0f;

		m_Eye.x = 0.0f;
		m_Eye.y = 0.0f;
		m_Eye.z = -4.0f;

		m_At.x = 0.0f;
		m_At.y = 0.0f;
		m_At.z = 0.0f;

		m_Up.x = 0.0f;
		m_Up.y = 1.0f;
		m_Up.z = 0.0f;

		D3DXMatrixLookAtLH(&m_matView, &m_Eye, &m_At, &m_Up);
		m_pdev->SetTransform(D3DTS_VIEW, &m_matView);

		D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4.0f, (float)m_VP.Width / (float)m_VP.Height, 1.0f, 2000.0f);
		m_pdev->SetTransform(D3DTS_PROJECTION, &m_matProj);

		m_pdev->SetViewport(&m_VP);
	}
}

CGameApp* CGameApp::GetInstance()
{
	if (!m_pGameApp)
	{
		m_pGameApp = new CGameApp();
	}

	return m_pGameApp;
}

HWND CGameApp::GetHWND()
{
	return m_hWnd;
}

const D3DVIEWPORT9* const CGameApp::GetViewPort()
{
	return &m_VP;
}
