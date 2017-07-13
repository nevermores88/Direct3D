#include "stdafx.h"
#include "Ex03_Create.h"


CEx03_Create::CEx03_Create()
{
}


CEx03_Create::~CEx03_Create()
{
}

void CEx03_Create::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;

	m_pdev = pdev;
	memset(m_pObjects, 0, sizeof(m_pObjects));
	memset(m_mtObjWorld, 0, sizeof(m_mtObjWorld));

	D3DXCreateTeapot(m_pdev, &m_pObjects[0], 0);
	D3DXCreateBox(m_pdev, 2.0f, 2.0f, 2.0f, &m_pObjects[1], 0);
	D3DXCreateCylinder(m_pdev, 1.0f, 1.0f, 3.0f, 10, 10, &m_pObjects[2], 0);
	D3DXCreateTorus(m_pdev, 1.0f, 3.0f, 10, 10, &m_pObjects[3], 0);
	D3DXCreateSphere(m_pdev, 1.0f, 10, 10, &m_pObjects[4], 0);

	D3DXMatrixTranslation(&m_mtObjWorld[0], 0.0f, 0.0f, 0.0f);
	D3DXMatrixTranslation(&m_mtObjWorld[1], -5.0f, 0.0f, 5.0f);
	D3DXMatrixTranslation(&m_mtObjWorld[2], 5.0f, 0.0f, 5.0f);
	D3DXMatrixTranslation(&m_mtObjWorld[3], -5.0f, 0.0f, -5.0f);
	D3DXMatrixTranslation(&m_mtObjWorld[4], 5.0f, 0.0f, -5.0f);

	m_fAngle = (3.0f*D3DX_PI) / 2.0f;
	m_fCameraHeight = 0.0f;
	m_fCameraHeightDirection = 5.0f;
	memset(&m_mtCamera, 0, sizeof(m_mtCamera));
}

void CEx03_Create::Release()
{

}

void CEx03_Create::OnRender()
{
	if (m_pdev)
	{
		m_pdev->SetTransform(D3DTS_VIEW, &m_mtCamera);
		m_pdev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		for (int i = 0; i < 5; i++)
		{
			m_pdev->SetTransform(D3DTS_WORLD, &m_mtObjWorld[i]);
			m_pObjects[i]->DrawSubset(0);
		}
	}
}

void CEx03_Create::OnUpdate()
{
	if (m_pdev)
	{
		D3DXVECTOR3 vPostion(cosf(m_fAngle)*20.0f, m_fCameraHeight, sinf(m_fAngle)*20.0f);

		D3DXVECTOR3 vTarget(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);

		D3DXMatrixLookAtLH(&m_mtCamera, &vPostion, &vTarget, &vUp);

		m_fAngle += g_pGame->GetManager()->GetTimeManager()->m_fDeltaTime;
		if (m_fAngle >= 6.28f)
			m_fAngle = 0.0f;

		float deltaTime = g_pGame->GetManager()->GetTimeManager()->m_fDeltaTime;
		//deltaTime = 0.064f;
		m_fCameraHeight += m_fCameraHeightDirection*deltaTime;

		if (m_fCameraHeight >= 10.0f)
			m_fCameraHeightDirection = -5.0f;

		if (m_fCameraHeight <= -10.0f)
			m_fCameraHeightDirection = 5.0f;
	}
}
