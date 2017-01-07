#include "stdafx.h"
#include "TestShader.h"


CTestShader::CTestShader()
{
}


CTestShader::~CTestShader()
{
}

HRESULT CTestShader::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pShader = LoadShader("Ex03_10/tmp.fx");
	if (!m_pShader)
		return E_FAIL;

	if (FAILED(D3DXCreateCylinder(m_pdev, 6, 6, 1000, 30, 5, &m_pMesh1, NULL)))
		return E_FAIL;

	if (FAILED(D3DXCreateCylinder(m_pdev, 10, 8, 1000, 30, 10, &m_pMesh2, NULL)))
		return E_FAIL;

	return S_OK;
}

void CTestShader::Release()
{
	if (m_pShader)
	{
		m_pShader->Release();
		m_pShader = NULL;
	}

	if (m_pMesh1)
	{
		m_pMesh1->Release();
		m_pMesh1 = NULL;
	}

	if (m_pMesh2)
	{
		m_pMesh2->Release();
		m_pMesh2 = NULL;
	}

	if (m_pFVF)
	{
		m_pFVF->Release();
		m_pFVF = NULL;
	}
}

void CTestShader::Render()
{
	if (m_pdev)
	{
		D3DXMATRIX mtView;
		D3DXMATRIX mtViewInv;
		D3DXMATRIX mtProj;

		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);

		//D3DXMatrixIdentity(&m_mtWorld);
		HRESULT hr = 0;

		D3DXMATRIX mtAll = m_mtWorld* mtView*mtProj;
		hr = m_pShader->SetMatrix("gWorldViewProjection", &mtAll);
		//hr = m_pShader->SetVector("g_vNormal", &vNormal);

		//m_pdev->SetVertexDeclaration(m_pFVF);

		m_pShader->SetTechnique("Tech0");

		UINT numPasses = 0;
		m_pShader->Begin(&numPasses, NULL);

		{
			m_pShader->BeginPass(0);
			{
				m_pMesh1->DrawSubset(0);
			}
			m_pShader->EndPass();
		}

		/*UINT numPasses = 0;

		m_pShader->Begin(&numPasses, NULL);

		for (UINT i = 0; i < numPasses; ++i)
		{
		m_pShader->BeginPass(i);
		{
		switch (i)
		{
		case 0:
		{
		fBright = (94 + rand() % 7)*0.01;
		vColor = D3DXVECTOR4(0, 0, 1, 1);
		vColor *= fBright;
		m_pShader->SetVector("g_vColor", &vColor);
		m_pMesh2->DrawSubset(0);
		}
		break;
		case 1:
		{
		fBright = (94 + rand() % 7)*0.01;
		vColor = D3DXVECTOR4(0.2f, 1, 0, 0.1f);
		vColor *= fBright;
		m_pShader->SetVector("g_vColor", &vColor);
		m_pMesh1->DrawSubset(0);
		}
		break;
		}
		}
		m_pShader->EndPass();
		}

		m_pShader->End();*/

		m_pdev->SetVertexShader(NULL);
		m_pdev->SetPixelShader(NULL);
		m_pdev->SetVertexDeclaration(NULL);
	}
}

void CTestShader::Update()
{
	if (m_pdev)
	{
		static float c = 0;

		c += 25.f * 0.05f;
		//	c=90;
		if (c > 360.f)
			c -= 360.f;

		D3DXMATRIX	mtY;
		D3DXMATRIX	mtZ;

		// Update World Matrix
		D3DXMatrixRotationY(&mtY, D3DXToRadian(-c));
		D3DXMatrixRotationZ(&mtZ, D3DXToRadian(-23.5f));
		m_mtWorld = mtY * mtZ;
	}
}