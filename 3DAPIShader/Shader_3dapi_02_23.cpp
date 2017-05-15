#include "stdafx.h"
#include "Shader_3dapi_02_23.h"


CShader_3dapi_02_23::CShader_3dapi_02_23()
{
}


CShader_3dapi_02_23::~CShader_3dapi_02_23()
{
}

HRESULT CShader_3dapi_02_23::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pShader = LoadPixelShader("Ex02_23/Shader.fx", &m_pConstTbl);
	if (!m_pShader)
		return E_FAIL;

	D3DXCreateTextureFromFile(m_pdev, "Ex02_23/earth.bmp", &m_pTex1);
	D3DXCreateTextureFromFile(m_pdev, "Ex02_23/dx5_logo.bmp", &m_pTex2);
	D3DXCreateTextureFromFile(m_pdev, "Ex02_23/env0.png", &m_pTex3);

	m_pVertices[0] = Vertex(-1.05F, 1.02F, 0, D3DXCOLOR(1, 0, 0, 1), 0, 0);
	m_pVertices[1] = Vertex(1.05F, 1.02F, 0, D3DXCOLOR(0, 1, 0, 1), 1, 0);
	m_pVertices[2] = Vertex(1.05F, -1.02F, 0, D3DXCOLOR(0, 0, 1, 1), 1, 1);
	m_pVertices[3] = Vertex(-1.05F, -1.02F, 0, D3DXCOLOR(1, 0, 1, 1), 0, 1);

	return S_OK;
}

void CShader_3dapi_02_23::Release()
{
	if (m_pShader)
	{
		m_pShader->Release();
		m_pShader = NULL;
	}

	if (m_pConstTbl)
	{
		m_pConstTbl->Release();
		m_pConstTbl = NULL;
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

	if (m_pTex3)
	{
		m_pTex3->Release();
		m_pTex3 = NULL;
	}
}

void CShader_3dapi_02_23::Render()
{
	if (m_pdev)
	{
		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);

		m_pdev->SetPixelShader(m_pShader);

		m_pdev->SetFVF(Vertex::FVF);

		m_pdev->SetTexture(0, m_pTex1);
		m_pdev->SetTexture(1, m_pTex2);
		m_pdev->SetTexture(2, m_pTex3);

		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_pVertices, sizeof(Vertex));


		m_pdev->SetTexture(0, NULL);
		m_pdev->SetTexture(1, NULL);
		m_pdev->SetTexture(2, NULL);

		m_pdev->SetPixelShader(NULL);
	}
}

void CShader_3dapi_02_23::Update()
{
	if (m_pdev)
	{

	}
}
