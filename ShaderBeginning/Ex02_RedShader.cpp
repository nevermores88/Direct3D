#include "stdafx.h"
#include "Ex02_RedShader.h"

CEx02_RedShader::CEx02_RedShader()
{
}


CEx02_RedShader::~CEx02_RedShader()
{
}

HRESULT CEx02_RedShader::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pShader = NULL;
	m_pSphere = NULL;

	m_pShader = LoadShader("Ex02/Shader.fx");
	if (!m_pShader)
		return E_FAIL;

	m_pSphere = LoadModel("Ex02/Sphere.x");
	if (!m_pSphere)
		return E_FAIL;

	return S_OK;
}

void CEx02_RedShader::Release()
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

void CEx02_RedShader::Render()
{
	if (m_pdev)
	{
		D3DXMATRIX mtWorld;
		D3DXMatrixIdentity(&mtWorld);

		m_pShader->SetMatrix("gWorldMatrix", &mtWorld);
		m_pShader->SetMatrix("gViewMatrix", &(g_pGame->m_matView));
		m_pShader->SetMatrix("gProjectionMatrix", &(g_pGame->m_matProj));

		UINT numPasses = 0;

		//LPD3DXMESH m_pObjects;
		//D3DXCreateTeapot(m_pdev, &m_pObjects, 0);

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

void CEx02_RedShader::Update()
{
	if (m_pdev)
	{

	}
}
