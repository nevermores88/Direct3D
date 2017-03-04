#include "stdafx.h"
#include "Ex09_UVAnimation.h"


CEx09_UVAnimation::CEx09_UVAnimation()
{
}


CEx09_UVAnimation::~CEx09_UVAnimation()
{
}

HRESULT CEx09_UVAnimation::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_fRotY = 0.0f;

	m_pShader = LoadShader("Ex09/Shader.fx");
	if (!m_pShader)
		return E_FAIL;

	m_pMesh = LoadModel("Ex09/torus.x");
	if (!m_pMesh)
		return E_FAIL;

	m_pTexDM = LoadTexture("Ex09/Fieldstone_DM.tga");
	if (!m_pTexDM)
		return E_FAIL;

	m_pTexSM = LoadTexture("Ex09/Fieldstone_SM.tga");
	if (!m_pTexSM)
		return E_FAIL;

	return S_OK;
}

void CEx09_UVAnimation::Release()
{
	if (m_pMesh)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}
	if (m_pShader)
	{
		m_pShader->Release();
		m_pShader = NULL;
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

void CEx09_UVAnimation::Render()
{
	if (m_pdev)
	{
		D3DXMATRIX mtWorld;
		D3DXMatrixIdentity(&mtWorld);
		//D3DXMatrixRotationY(&mtWorld, m_fRotY);

		D3DXMATRIX mtView;
		D3DXMATRIX mtProj;

		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);

		/*D3DXMATRIX mtWorldView;
		D3DXMATRIX mtWorldViewProj;
		D3DXMatrixMultiply(&mtWorldView, &mtWorld, &mtView);
		D3DXMatrixMultiply(&mtWorldViewProj, &mtWorldView, &mtProj);*/

		D3DXVECTOR4 vLightPos = D3DXVECTOR4(500.0f, 500.0f, -500.0f, 1.0f);
		D3DXVECTOR4 vCameraPos(g_pGame->m_Eye.x, g_pGame->m_Eye.y, g_pGame->m_Eye.z, 0.0f);
		D3DXVECTOR4 vLightColor(0.7f, 0.7f, 1.0f, 1.0f);

		m_pShader->SetMatrix("g_mtWorld", &mtWorld);
		m_pShader->SetMatrix("g_mtView", &mtView);
		m_pShader->SetMatrix("g_mtProj", &mtProj);

		m_pShader->SetVector("g_vLightPos", &vLightPos);
		m_pShader->SetVector("g_vCameraPos", &vCameraPos);
		m_pShader->SetVector("g_vLightColor", &vLightColor);

		m_pShader->SetTexture("g_DiffuseTex", m_pTexDM);
		m_pShader->SetTexture("g_SpecularTex", m_pTexSM);

		m_pShader->SetFloat("g_fWaveHeight", 3);
		m_pShader->SetFloat("g_fSpeed", 2);
		m_pShader->SetFloat("g_fWaveFrequency", 10);
		m_pShader->SetFloat("g_fUVSpeed", 0.25f);

		//ULONGLONG tick = GetTickCount64();

		static ULONGLONG tick = 1000.0f;
		tick += 20.0f;
		m_pShader->SetFloat("g_fTime", tick / 1000.0f);

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

void CEx09_UVAnimation::Update()
{
	if (m_pdev)
	{
		m_fRotY += 0.4f * 3.14f / 180.0f;
		if (m_fRotY > 2 * 3.14f)
			m_fRotY -= 2 * 3.14f;
	}
}
