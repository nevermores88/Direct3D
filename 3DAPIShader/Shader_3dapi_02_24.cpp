#include "stdafx.h"
#include "Shader_3dapi_02_24.h"


CShader_3dapi_02_24::CShader_3dapi_02_24()
{
}


CShader_3dapi_02_24::~CShader_3dapi_02_24()
{
}

HRESULT CShader_3dapi_02_24::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pShader = LoadPixelShader("Ex02_24/Shader.fx", &m_pConstTbl);
	if (!m_pShader)
		return E_FAIL;

	D3DXCreateTextureFromFile(m_pdev, "Ex02_24/earth.bmp", &m_pTex1);
	D3DXCreateTextureFromFile(m_pdev, "Ex02_24/dx5_logo.bmp", &m_pTex2);
}

void CShader_3dapi_02_24::Release()
{
	if (m_pShader)
	{
		m_pShader->Release();
		m_pShader = NULL;
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

void CShader_3dapi_02_24::Render()
{
	if (m_pdev)
	{
		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);

		m_pdev->SetPixelShader(m_pShader);

		m_pdev->SetFVF(Vertex::FVF);
		m_pdev->SetTexture(0, m_pTex1);
		m_pdev->SetTexture(1, m_pTex2);

		Vertex pVertices[4];

		for (int i = 0; i <= 13; ++i)
		{
			float	x = -1.f + float(i % 4) / 2.f;
			float	y = 1.f - float(i / 4) / 2.f;

			pVertices[0] = Vertex(x + 0.f, y - 0.f, 0, D3DXCOLOR(1, 0, 0, 1), 0, 0);
			pVertices[1] = Vertex(x + .5f, y - 0.f, 0, D3DXCOLOR(0, 1, 0, 1), 1, 0);
			pVertices[2] = Vertex(x + .5f, y - .5f, 0, D3DXCOLOR(0, 0, 1, 1), 1, 1);
			pVertices[3] = Vertex(x + 0.f, y - .5f, 0, D3DXCOLOR(0, 0, 1, 1), 0, 1);

			m_pConstTbl->SetInt(m_pdev, "m_nMulti", i);
			m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, pVertices, sizeof(Vertex));
		}

		m_pdev->SetPixelShader(NULL);
		m_pdev->SetTexture(0, NULL);
		m_pdev->SetTexture(1, NULL);
	}
}

void CShader_3dapi_02_24::Update()
{
	if (m_pdev)
	{

	}
}
