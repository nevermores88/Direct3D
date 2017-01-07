#include "stdafx.h"
#include "Ex03_TextureMapping.h"


CEx03_TextureMapping::CEx03_TextureMapping()
{
}


CEx03_TextureMapping::~CEx03_TextureMapping()
{
}

HRESULT CEx03_TextureMapping::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pShader = NULL;
	m_pSphere = NULL;
	m_pTex = NULL;
	m_fRotY = 0.0f;

	m_pShader = LoadShader("Ex03/Shader.fx");
	if (!m_pShader)
		return E_FAIL;

	m_pSphere = LoadModel("Ex03/Sphere.x");
	if (!m_pSphere)
		return E_FAIL;

	m_pTex = LoadTexture("Ex03/Earth.jpg");
	if (!m_pTex)
		return E_FAIL;

	D3DXCreateFont(m_pdev, 20, 10, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, (DEFAULT_PITCH | FF_DONTCARE),
		"Arial", &gpFont);

	return S_OK;
}

void CEx03_TextureMapping::Release()
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

	if (m_pTex)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
}

void CEx03_TextureMapping::Render()
{
	if (m_pdev)
	{
		D3DXMATRIX mtWorld;
		D3DXMatrixRotationY(&mtWorld, m_fRotY);

		D3DCOLOR fontColor = D3DCOLOR_ARGB(255, 255, 255, 255);

		// 텍스트를 출력할 위치
		RECT rct;
		rct.left = 5;
		rct.right = 300;
		rct.top = 5;
		rct.bottom = 300;

		char str[256];
		sprintf(str, "%f", m_fRotY);

		// 키 입력 정보를 출력
		gpFont->DrawText(NULL, str, -1, &rct, 0, fontColor);



		m_pShader->SetMatrix("gWorldMatrix", &mtWorld);
		m_pShader->SetMatrix("gViewMatrix", &(g_pGame->m_matView));
		m_pShader->SetMatrix("gProjectionMatrix", &(g_pGame->m_matProj));

		m_pShader->SetTexture("DiffuseMap_Tex", m_pTex);

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

void CEx03_TextureMapping::Update()
{
	if (m_pdev)
	{
		m_fRotY += 0.4f * 3.14f / 180.0f;
		if (m_fRotY > 2 * 3.14f)
			m_fRotY -= 2 * 3.14f;
	}
}
