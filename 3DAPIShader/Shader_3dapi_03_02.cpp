#include "stdafx.h"
#include "Shader_3dapi_03_02.h"


CShader_3dapi_03_02::CShader_3dapi_03_02()
{
}


CShader_3dapi_03_02::~CShader_3dapi_03_02()
{
}

HRESULT CShader_3dapi_03_02::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pShader = NULL;
	m_pMesh = NULL;
	m_pTex1 = NULL;
	m_pTex2 = NULL;

	m_nPassIdx = 0;

	m_pShader = LoadShader("Ex03_02/Shader.fx");
	if (!m_pShader)
		return E_FAIL;

	m_pMesh = LoadModel("Ex03_02/teapot.x");
	if (!m_pMesh)
		return E_FAIL;

	m_pTex1 = LoadTexture("Ex03_02/earth.bmp");
	if (!m_pTex1)
		return E_FAIL;

	m_pTex2 = LoadTexture("Ex03_02/wood.jpg");
	if (!m_pTex2)
		return E_FAIL;

	return S_OK;
}

void CShader_3dapi_03_02::Release()
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

	if (m_pTex1)
	{
		m_pTex1->Release();
		m_pTex1 = NULL;
	}

	if (m_pTex2)
	{
		m_pTex2->Release();
		m_pTex2 = NULL;
	}
}

void CShader_3dapi_03_02::Render()
{
	if (m_pdev)
	{
		m_pdev->SetTransform(D3DTS_WORLD, &m_mtWorld);
		m_pdev->SetTexture(1, m_pTex2);

		m_pShader->SetTexture("gTex01", m_pTex1);
		m_pShader->SetTexture("gTex02", m_pTex2);

		m_pShader->SetTechnique("Tech0");

		m_pShader->Begin(NULL, 0);

		m_pShader->BeginPass(m_nPassIdx);
		{
			m_pMesh->DrawSubset(0);
		}
		m_pShader->EndPass();

		m_pShader->End();

		m_pdev->SetTexture(1, NULL);
		m_pdev->SetVertexShader(NULL);
		m_pdev->SetPixelShader(NULL);
		D3DXMATRIX	mtIdentity;
		D3DXMatrixIdentity(&mtIdentity);
		m_pdev->SetTransform(D3DTS_WORLD, &mtIdentity);
	}
}

void CShader_3dapi_03_02::Update()
{
	if (m_pdev)
	{
		static float c = 0;

		c += 50.f * 0.05f;
		//	c=90;
		if (c > 360.f)
			c -= 360.f;

		D3DXMATRIX	mtS;
		D3DXMATRIX	mtY;
		D3DXMATRIX	mtZ;


		// Update World Matrix
		D3DXMatrixScaling(&mtS, 40, 40, 40);
		D3DXMatrixRotationY(&mtY, D3DXToRadian(-c));
		D3DXMatrixRotationZ(&mtZ, D3DXToRadian(-23.5f));

		m_mtRot = mtY * mtZ;
		m_mtWorld = mtS * m_mtRot;

		if (::GetAsyncKeyState('1') & 0x8000)
			m_nPassIdx = 0;

		if (::GetAsyncKeyState('2') & 0x8000)
			m_nPassIdx = 1;

		if (::GetAsyncKeyState('3') & 0x8000)
			m_nPassIdx = 2;
	}
}
