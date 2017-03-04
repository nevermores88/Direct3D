#include "stdafx.h"
#include "Ex10_ShadowMapping.h"


CEx10_ShadowMapping::CEx10_ShadowMapping()
{
}


CEx10_ShadowMapping::~CEx10_ShadowMapping()
{
}

HRESULT CEx10_ShadowMapping::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_fRotY = 0.0f;

	m_pCreateShadowShader = LoadShader("Ex10/CreateShadowShader.fx");
	if (!m_pCreateShadowShader)
		return E_FAIL;

	m_pApplyShadowShader = LoadShader("Ex10/ApplyShadowShader.fx");
	if (!m_pApplyShadowShader)
		return E_FAIL;

	m_pTorusMesh = LoadModel("Ex10/Torus.x");
	if (!m_pTorusMesh)
		return E_FAIL;

	m_pDiscMesh = LoadModel("Ex10/Disc.x");
	if (!m_pDiscMesh)
		return E_FAIL;

	//����Ÿ�� ����
	const int iShadowMapSize = 2048;
	if (FAILED(m_pdev->CreateTexture(iShadowMapSize, iShadowMapSize, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &m_pShadowRenderTarget, NULL)))
		return E_FAIL;

	//�׸��ڸʰ� ������ ũ���� ���� ���� ����
	if (FAILED(m_pdev->CreateDepthStencilSurface(iShadowMapSize, iShadowMapSize, D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pShadowDepthStencil, NULL)))
		return E_FAIL;

	return S_OK;
}

void CEx10_ShadowMapping::Release()
{

}

void CEx10_ShadowMapping::Render()
{
	if (m_pdev)
	{
		D3DXVECTOR4 vLightPos = D3DXVECTOR4(500.0f, 500.0f, -500.0f, 1.0f);
		D3DXVECTOR4 vCameraPos(g_pGame->m_Eye.x, g_pGame->m_Eye.y, g_pGame->m_Eye.z, 0.0f);
		D3DXVECTOR4 vLightColor(0.7f, 0.7f, 1.0f, 1.0f);

		//����-�� ��� ����
		D3DXMATRIX mtLightView;
		{
			D3DXVECTOR3 vEyePt(vLightPos.x, vLightPos.y, vLightPos.z);
			D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
			D3DXMatrixLookAtLH(&mtLightView, &vEyePt, &vLookatPt, &vUpVec);
		}

		//����-���� ��� ����
		D3DXMATRIX mtLightProj;
		{
			D3DXMatrixPerspectiveFovLH(&mtLightProj, D3DX_PI / 4.0f, 1, 1, 3000);
		}

		//Torus ���� ��� ����
		D3DXMATRIX mtTorusWorld;
		D3DXMatrixIdentity(&mtTorusWorld);
		//D3DXMatrixRotationY(&mtWorld, m_fRotY);

		//Disc ���� ��� ����
		D3DXMATRIX mtDiscWorld;
		{
			D3DXMATRIX mtScale;
			D3DXMatrixScaling(&mtScale, 2, 2, 2);

			D3DXMATRIX mtTrans;
			D3DXMatrixTranslation(&mtTrans, 0, -40, 0);

			D3DXMatrixMultiply(&mtDiscWorld, &mtScale, &mtTrans);
		}

		//View, Proj ��� ��������
		D3DXMATRIX mtView;
		D3DXMATRIX mtProj;
		D3DXMATRIX mtViewProj;

		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);

		D3DXMatrixMultiply(&mtViewProj, &mtView, &mtProj);

		//���� �ϵ���� ���ۿ� ���̹��� ��������
		LPDIRECT3DSURFACE9 pHWBackBuffer = NULL;
		LPDIRECT3DSURFACE9 pHWDepthStencilBuffer = NULL;

		m_pdev->GetRenderTarget(0, &pHWBackBuffer);
		m_pdev->GetDepthStencilSurface(&pHWDepthStencilBuffer);

		////////////////////////
		// 1. �׸��� �� ����
		////////////////////////

		//�׸��� ���� ����Ÿ��� ���̹��� ���
		LPDIRECT3DSURFACE9 pShadowSurface = NULL;
		if (SUCCEEDED(m_pShadowRenderTarget->GetSurfaceLevel(0, &pShadowSurface)))
		{
			m_pdev->SetRenderTarget(0, pShadowSurface);
			pShadowSurface->Release();
			pShadowSurface = NULL;
		}
		m_pdev->SetDepthStencilSurface(m_pShadowDepthStencil);

		//���� �����ӿ��� �׷ȴ� �׸��� ������ ����
		m_pdev->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), 0xFFFFFFFF, 1.0f, 0);

		m_pCreateShadowShader->SetMatrix("gWorldMatrix", &mtTorusWorld);
		m_pCreateShadowShader->SetMatrix("gLightViewMatrix", &mtLightView);
		m_pCreateShadowShader->SetMatrix("gLightProjMatrix", &mtLightProj);

		//�׸��� ����� ���̴��� ����
		{
			UINT numPasses = 0;

			m_pCreateShadowShader->Begin(&numPasses, NULL);

			for (UINT i = 0; i < numPasses; ++i)
			{
				m_pCreateShadowShader->BeginPass(i);
				{
					m_pTorusMesh->DrawSubset(0);
				}
				m_pCreateShadowShader->EndPass();
			}

			m_pCreateShadowShader->End();
		}
		
		////////////////////////
		// 2. �׸��� ������
		////////////////////////

		D3DXVECTOR4 vTorusColor(1, 1, 0, 1);
		D3DXVECTOR4 vDiscColor(0, 1, 1, 1);

		//�ϵ���� ����� / ���̹��۸� ����Ѵ�.
		m_pdev->SetRenderTarget(0, pHWBackBuffer);
		m_pdev->SetDepthStencilSurface(pHWDepthStencilBuffer);

		pHWBackBuffer->Release();
		pHWBackBuffer = NULL;
		pHWDepthStencilBuffer->Release();
		pHWDepthStencilBuffer = NULL;

		m_pApplyShadowShader->SetMatrix("gWorldMatrix", &mtTorusWorld);
		m_pApplyShadowShader->SetMatrix("gViewProjMatrix", &mtViewProj);
		m_pApplyShadowShader->SetMatrix("gLightViewMatrix", &mtLightView);
		m_pApplyShadowShader->SetMatrix("gLightProjMatrix", &mtLightProj);

		m_pApplyShadowShader->SetVector("gWorldLightPosition", &vLightPos);
		m_pApplyShadowShader->SetVector("gObjectColor", &vTorusColor);

		m_pApplyShadowShader->SetTexture("ShadowMap_Tex", m_pShadowRenderTarget);

		{
			UINT numPasses = 0;

			m_pApplyShadowShader->Begin(&numPasses, NULL);

			for (UINT i = 0; i < numPasses; ++i)
			{
				m_pApplyShadowShader->BeginPass(i);
				{
					m_pTorusMesh->DrawSubset(0);

					m_pApplyShadowShader->SetMatrix("gWorldMatrix", &mtDiscWorld);
					m_pApplyShadowShader->SetVector("gObjectColor", &vDiscColor);
					m_pApplyShadowShader->CommitChanges();
					m_pDiscMesh->DrawSubset(0);
				}
				m_pApplyShadowShader->EndPass();
			}

			m_pApplyShadowShader->End();
		}
	}
}

void CEx10_ShadowMapping::Update()
{
	if (m_pdev)
	{
		m_fRotY += 0.4f * 3.14f / 180.0f;
		if (m_fRotY > 2 * 3.14f)
			m_fRotY -= 2 * 3.14f;
	}
}
