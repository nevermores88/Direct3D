#include "stdafx.h"
#include "Game.h"

CGame* CGame::m_pGame = NULL;

CGame::CGame()
{
}


CGame::~CGame()
{
}

void CGame::OnInit()
{
	RECT rect;
	D3DVIEWPORT9 vp;
	GetClientRect(m_hWnd, &rect);

	vp.X = 0;
	vp.Y = 0;
	vp.Width = rect.right - rect.left;
	vp.Height = rect.bottom - rect.top;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;

	m_Eye.x = 0.0f;
	m_Eye.y = 0.0f;
	m_Eye.z = 0.0f;

	m_At.x = 0.0f;
	m_At.y = 0.0f;
	m_At.z = 0.0f;

	m_Up.x = 0.0f;
	m_Up.y = 1.0f;
	m_Up.z = 0.0f;

	/*D3DXMatrixLookAtLH(&m_matView, &m_Eye, &m_At, &m_Up);
	m_pdev->SetTransform(D3DTS_VIEW, &m_matView);*/

	g_pCamManager->CreateCamera(CCamera::CameraType::AIRCRAFT, m_pdev);
	g_pCamManager->GetCamera()->SetPosition(&m_Eye);
	g_pCamManager->GetCamera()->GetViewMatrix(&m_matView);
	m_pdev->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4.0f, (float)vp.Width / (float)vp.Height, 1.0f, 2000.0f);
	m_pdev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pdev->SetViewport(&vp);

	m_dwExType = en_Shader_3dapi_18;
	m_pClasses = NULL;
	m_pClasses = new	CEx15_Picking();
	m_pClasses->Create(m_pdev, 0);
}

void CGame::OnRender()
{
	m_pClasses->OnRender();
}

void CGame::OnUpdate()
{
	g_pInputManager->OnCamInputUpdate(g_pCamManager->GetCamera());
	if( g_pCamManager->GetCamera())
		g_pCamManager->GetCamera()->OnUpdate();

	m_pClasses->OnUpdate();
}

void CGame::OnRelease()
{
	//m_Ex09_Text02.Release();
}

CGame* CGame::GetGame()
{
	if (!m_pGame)
	{
		m_pGame = new CGame();
	}

	return m_pGame;
}
