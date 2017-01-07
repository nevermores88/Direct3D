#include "stdafx.h"
#include "Ex06_ToonShader.h"


CEx06_ToonShader::CEx06_ToonShader()
{
}


CEx06_ToonShader::~CEx06_ToonShader()
{
}

HRESULT CEx06_ToonShader::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pMesh = NULL;
	m_pShader = NULL;
	m_fRotY = 0.0f;

	m_pShader = LoadShader("Ex06/Shader.fx");
	if (!m_pShader)
		return E_FAIL;

	m_pMesh = LoadModel("Ex06/Teapot.x");
	if (!m_pMesh)
		return E_FAIL;

	return S_OK;
}

void CEx06_ToonShader::Release()
{
	if (m_pShader)
	{
		m_pShader->Release();
		m_pShader = NULL;
	}

	if (m_pMesh)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}
}

void CEx06_ToonShader::Render()
{
	if (m_pdev)
	{
		D3DXMATRIX mtWorld;
		D3DXMatrixRotationY(&mtWorld, m_fRotY);

		D3DXMATRIX mtWorldInv;
		D3DXMatrixTranspose(&mtWorldInv, &mtWorld);

		D3DXMATRIX mtView;
		D3DXMATRIX mtProj;

		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);

		D3DXMATRIX mtWorldView;
		D3DXMATRIX mtWorldViewProj;
		D3DXMatrixMultiply(&mtWorldView, &mtWorld, &mtView);
		D3DXMatrixMultiply(&mtWorldViewProj, &mtWorldView, &mtProj);

		m_pShader->SetMatrix("g_mtWorldViewProj", &mtWorldViewProj);
		m_pShader->SetMatrix("g_mtWorldInv", &mtWorldInv);

		D3DXVECTOR4 vLightPos = D3DXVECTOR4(500.0f, 500.0f, -500.0f, 1.0f);
		D3DXVECTOR4 vSurfaceColor = D3DXVECTOR4(0.5, 0.5, 0.9, 1);
		m_pShader->SetVector("g_vLightPos", &vLightPos);
		m_pShader->SetVector("g_vSurfaceColor", &vSurfaceColor);

		UINT numPasses = 0;

		m_pShader->Begin(&numPasses, NULL);

		for (UINT i = 0; i < numPasses; ++i)
		{
			m_pShader->BeginPass(i);
			{
				m_pMesh->DrawSubset(0);
			}
			m_pShader->EndPass();
		}

		m_pShader->End();
	}
}

void CEx06_ToonShader::Update()
{
	if (m_pdev)
	{
		m_fRotY += 0.4f * 3.14f / 180.0f;
		if (m_fRotY > 2 * 3.14f)
			m_fRotY -= 2 * 3.14f;
	}
}
