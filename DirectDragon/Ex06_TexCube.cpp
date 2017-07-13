#include "stdafx.h"
#include "Ex06_TexCube.h"


CEx06_TexCube::CEx06_TexCube()
{
}


CEx06_TexCube::~CEx06_TexCube()
{
}

void CEx06_TexCube::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;

	m_pBox = new CCube();
	m_pBox->Create(m_pdev);

	D3DXCreateTextureFromFile(m_pdev, L"Ex06/crate.jpg", &m_pTex);

	//m_fAngle = (3.0f * D3DX_PI) / 2.0f;
	//m_fHeight = 2.0f;
}

void CEx06_TexCube::Release()
{
	if (m_pBox)
	{
		m_pBox->Release();
		delete m_pBox;
		m_pBox = NULL;
	}
}

void CEx06_TexCube::OnRender()
{
	if (m_pBox)
	{
		m_pdev->SetLight(0, &m_Light);
		m_pdev->LightEnable(0, true);

		m_pdev->SetRenderState(D3DRS_NORMALIZENORMALS, true);
		m_pdev->SetRenderState(D3DRS_SPECULARENABLE, true);

		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		if (m_pBox)
		{
			m_pdev->SetMaterial(&d3d::WHITE_MTRL);
			m_pBox->Draw(&m_mtWorld, &(d3d::WHITE_MTRL), m_pTex);
		}
	}
}

void CEx06_TexCube::OnUpdate()
{
	if (m_pBox)
	{
		::ZeroMemory(&m_Light, sizeof(m_Light));
		m_Light.Type = D3DLIGHT_DIRECTIONAL;
		m_Light.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
		m_Light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_Light.Specular = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
		m_Light.Direction = D3DXVECTOR3(1.0f, -1.0f, 0.0f);

		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
			m_fAngle += 0.5f*g_pGame->GetManager()->GetTimeManager()->GetTimeDelta();
		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
			m_fAngle -= 0.5f*g_pGame->GetManager()->GetTimeManager()->GetTimeDelta();
		if (::GetAsyncKeyState(VK_UP) & 0x8000f)
			m_fHeight += 0.5f*g_pGame->GetManager()->GetTimeManager()->GetTimeDelta();
		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
			m_fHeight -= 0.5f*g_pGame->GetManager()->GetTimeManager()->GetTimeDelta();

		
		D3DXMatrixIdentity(&m_mtWorld);
		//D3DXMatrixRotationY(&m_mtWorld, m_fAngle);
		//이거 주석해제
		//D3DXMatrixRotationYawPitchRoll(&m_mtWorld, m_fAngle, m_fHeight, 0);
		//D3DXMatrixRotationX(&m_mtWorld, m_fHeight);
	}
}
