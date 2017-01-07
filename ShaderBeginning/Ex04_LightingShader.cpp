#include "stdafx.h"
#include "Ex04_LightingShader.h"


CEx04_LightingShader::CEx04_LightingShader()
{
}


CEx04_LightingShader::~CEx04_LightingShader()
{
}

HRESULT CEx04_LightingShader::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pShader = NULL;
	m_pSphere = NULL;
	m_fRotY = NULL;

	m_pShader = LoadShader("Ex04/Shader.fx");
	if (!m_pShader)
		return E_FAIL;

	m_pSphere = LoadModel("Ex04/Sphere.x");
	if (!m_pSphere)
		return E_FAIL;

	return S_OK;
}

void CEx04_LightingShader::Release()
{
	if (m_pShader)
	{
		m_pShader->Release();
		m_pShader = NULL;
	}

	if (m_pSphere)
	{
		m_pSphere->Release();
		m_pSphere = NULL;
	}
}

void CEx04_LightingShader::Render()
{
	if (m_pdev)
	{
		D3DXMATRIX mtWorld;
		D3DXMatrixRotationY(&mtWorld, m_fRotY);

		m_pShader->SetMatrix("gWorldMatrix", &mtWorld);
		m_pShader->SetMatrix("gViewMatrix", &(g_pGame->m_matView));
		m_pShader->SetMatrix("gProjectionMatrix", &(g_pGame->m_matProj));

		D3DXVECTOR4	gWorldLightPosition(500.0f, 500.0f, -500.0f, 1.0f);
		D3DXVECTOR4    gWorldCameraPosition(g_pGame->m_Eye.x, g_pGame->m_Eye.y, g_pGame->m_Eye.z, 0.0f);
		m_pShader->SetVector("gWorldLightPosition", &gWorldLightPosition);
		m_pShader->SetVector("gWorldCameraPosition", &gWorldCameraPosition);

		UINT numPasses = 0;

		m_pShader->Begin(&numPasses, NULL);

		for (UINT i = 0; i < numPasses; ++i)
		{
			m_pShader->BeginPass(i);
			{
				m_pSphere->DrawSubset(0);
			}
			m_pShader->EndPass();
		}
		m_pShader->End();
	}
}

void CEx04_LightingShader::Update()
{
	if (m_pdev)
	{
		m_fRotY += 0.4f * 3.14f / 180.0f;
		if (m_fRotY > 2 * 3.14f)
			m_fRotY -= 2 * 3.14f;
	}
}
