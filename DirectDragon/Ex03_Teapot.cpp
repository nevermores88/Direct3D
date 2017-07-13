#include "stdafx.h"
#include "Ex03_Teapot.h"


CEx03_Teapot::CEx03_Teapot()
{
}


CEx03_Teapot::~CEx03_Teapot()
{
}

void CEx03_Teapot::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;
	m_pTeapot = NULL;
	m_fY = 0.f;

	D3DXCreateTeapot(m_pdev, &m_pTeapot, 0);
}

void CEx03_Teapot::Release()
{

}

void CEx03_Teapot::OnRender()
{
	if (m_pdev)
	{
		D3DXMATRIX mtY;
		D3DXMatrixRotationY(&mtY, m_fY);

		m_pdev->SetTransform(D3DTS_WORLD, &mtY);
		m_pdev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		m_pTeapot->DrawSubset(0);

		m_pdev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}

void CEx03_Teapot::OnUpdate()
{
	if (m_pdev)
	{
		m_fY += g_pGame->GetManager()->GetTimeManager()->m_fDeltaTime;

		if (m_fY >= 6.28f)
			m_fY = 0.f;
	}
}
