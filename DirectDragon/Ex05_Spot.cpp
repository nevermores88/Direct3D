#include "stdafx.h"
#include "Ex05_Spot.h"


CEx05_Spot::CEx05_Spot()
{
}


CEx05_Spot::~CEx05_Spot()
{
}

void CEx05_Spot::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;

	D3DXCreateTeapot(m_pdev, &m_pObjects[0], 0);
	D3DXCreateSphere(m_pdev, 1.0f, 20, 20, &m_pObjects[1], 0);
	D3DXCreateTorus(m_pdev, 0.5f, 1.0f, 20, 20, &m_pObjects[2], 0);
	D3DXCreateCylinder(m_pdev, 0.5f, 0.5f, 2.0f, 20, 20, &m_pObjects[3], 0);

	D3DXMatrixTranslation(&m_mtWorlds[0], 0.0f, 2.0f, 0.0f);
	D3DXMatrixTranslation(&m_mtWorlds[1], 0.0f, -2.0f, 0.0f);
	D3DXMatrixTranslation(&m_mtWorlds[2], -3.0f, 0.0f, 0.0f);
	D3DXMatrixTranslation(&m_mtWorlds[3], 3.0f, 0.0f, 0.0f);

	m_Mtrls[0] = d3d::RED_MTRL;
	m_Mtrls[1] = d3d::BLUE_MTRL;
	m_Mtrls[2] = d3d::GREEN_MTRL;
	m_Mtrls[3] = d3d::YELLOW_MTRL;

	for (int i = 0; i < 4; i++)
	{
		m_Mtrls[i].Power = 20.0f;
	}
}

void CEx05_Spot::Release()
{

}

void CEx05_Spot::OnRender()
{
	if (m_pdev)
	{
		m_pdev->SetLight(0, &m_SpotLight);
		m_pdev->LightEnable(0, true);

		m_pdev->SetRenderState(D3DRS_NORMALIZENORMALS, true);
		m_pdev->SetRenderState(D3DRS_SPECULARENABLE, true);

		for (int i = 0; i < 4; ++i)
		{
			m_pdev->SetMaterial(&m_Mtrls[i]);
			m_pdev->SetTransform(D3DTS_WORLD, &m_mtWorlds[i]);
			m_pObjects[i]->DrawSubset(0);
		}
	}
}

void CEx05_Spot::OnUpdate()
{
	if (m_pdev)
	{
		memset(m_vSpotLightPos, 0, sizeof(m_vSpotLightPos));
		m_vSpotLightPos.z = -5.0f;
		memset(m_vSpotLightDir, 0, sizeof(m_vSpotLightDir));
		m_vSpotLightDir.x = -0.3f;
		m_vSpotLightDir.z = 1.0f;

		D3DXCOLOR color = d3d::WHITE;
		m_SpotLight = d3d::InitSpotLight(&m_vSpotLightPos, &m_vSpotLightDir, &color);
	}
}
