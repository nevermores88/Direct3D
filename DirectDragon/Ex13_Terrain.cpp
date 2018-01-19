#include "stdafx.h"
#include "Ex13_Terrain.h"


CEx13_Terrain::CEx13_Terrain()
{
}


CEx13_Terrain::~CEx13_Terrain()
{
}

void CEx13_Terrain::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;

	D3DXVECTOR3 lightDir(0.0f, 1.0f, 0.0f);

	m_pTerrain = new CTerrain();
	if (!m_pTerrain)
		return;

	m_pTerrain->Create(m_pdev, "Ex13/coastMountain64.raw", 64, 64, 10, 0.5f);
	m_pTerrain->GenTexture(&lightDir);

	m_pCamera = new CCamera();
	if (!m_pCamera)
		return;

	m_pCamera->Create(CCamera::LANDOBJECT);
}

void CEx13_Terrain::Release()
{

}

void CEx13_Terrain::OnRender()
{
	if (m_pdev)
	{
		m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		D3DXMATRIX mtIdentity;
		D3DXMatrixIdentity(&mtIdentity);

		if (m_pTerrain)
			m_pTerrain->Draw(&mtIdentity, true);
	}
}

void CEx13_Terrain::OnUpdate()
{
	if (m_pdev)
	{
		float fTimeDelta = g_pGame->GetManager()->GetTimeManager()->GetTimeDelta();

		if (::GetAsyncKeyState('W') & 0x8000f)
			m_pCamera->Walk(30.0f * fTimeDelta);

		if (::GetAsyncKeyState('S') & 0x8000f)
			m_pCamera->Walk(-30.0f * fTimeDelta);

		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
			m_pCamera->Yaw(-1.0f * fTimeDelta);

		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
			m_pCamera->Yaw(1.0f * fTimeDelta);

		if (::GetAsyncKeyState('A') & 0x8000f)
			m_pCamera->Strafe(-30.0f * fTimeDelta);

		if (::GetAsyncKeyState('D') & 0x8000f)
			m_pCamera->Strafe(30.0f * fTimeDelta);

		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
			m_pCamera->Pitch(1.0f * fTimeDelta);

		if (::GetAsyncKeyState(VK_UP) & 0x8000f)
			m_pCamera->Pitch(-1.0f * fTimeDelta);

		D3DXVECTOR3 pos;
		m_pCamera->GetPosition(&pos);

		float fHeight = m_pTerrain->GetHeight(pos.x, pos.z);
		pos.y = fHeight + 5.0f;
		m_pCamera->SetPosition(&pos);

		D3DXMATRIX	mtView;
		m_pCamera->GetViewMatrix(&mtView);
		m_pdev->SetTransform(D3DTS_VIEW, &mtView);
	}
}
