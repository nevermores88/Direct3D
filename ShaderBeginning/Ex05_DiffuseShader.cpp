#include "stdafx.h"
#include "Ex05_DiffuseShader.h"


CEx05_DiffuseShader::CEx05_DiffuseShader()
{
}


CEx05_DiffuseShader::~CEx05_DiffuseShader()
{
}

HRESULT CEx05_DiffuseShader::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pSphere = NULL;
	m_pShader = NULL;
	m_pTexDM = NULL;

	m_pShader = LoadShader("Ex05/Shader.fx");
	if (!m_pShader)
		return E_FAIL;

	m_pSphere = LoadModel("Ex05/Sphere.x");
	if (!m_pSphere)
		return E_FAIL;

	m_pTexDM = LoadTexture("Ex05/Fieldstone_DM.tga");
	if (!m_pTexDM)
		return E_FAIL;

	m_pTexSM = LoadTexture("Ex05/Fieldstone_SM.tga");
	if (!m_pTexSM)
		return E_FAIL;

	return S_OK;
}

void CEx05_DiffuseShader::Release()
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

	if (m_pTexDM)
	{
		m_pTexDM->Release();
		m_pTexDM = NULL;
	}

	if (m_pTexSM)
	{
		m_pTexSM->Release();
		m_pTexSM = NULL;
	}
}

void CEx05_DiffuseShader::Render()
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
		D3DXVECTOR4	gLightColor(0.7f, 0.7f, 1.0f, 0.0f);

		m_pShader->SetVector("gWorldLightPosition", &gWorldLightPosition);
		m_pShader->SetVector("gWorldCameraPosition", &gWorldCameraPosition);

		m_pShader->SetVector("gLightColor", &gLightColor);
		m_pShader->SetTexture("DiffuseMap_Tex", m_pTexDM);
		m_pShader->SetTexture("SpecularMap_Tex", m_pTexSM);

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

void CEx05_DiffuseShader::Update()
{
	if (m_pdev)
	{
		m_fRotY += 0.4f * 3.14f / 180.0f;
		if (m_fRotY > 2 * 3.14f)
			m_fRotY -= 2 * 3.14f;
	}
}
