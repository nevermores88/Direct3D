#include "stdafx.h"
#include "Shader_3dapi_03_26.h"


CShader_3dapi_03_26::CShader_3dapi_03_26()
{
}


CShader_3dapi_03_26::~CShader_3dapi_03_26()
{
}

HRESULT CShader_3dapi_03_26::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pShader = LoadShader("Ex03_26/Shader.fx");
	if (!m_pShader)
		return E_FAIL;

	D3DXCreateTextureFromFile(m_pdev, "Ex03_26/stones.bmp", &m_pDiffuseTex);
	D3DXCreateTextureFromFile(m_pdev, "Ex03_26/lighting.tga", &m_pLightTex);

	m_pVertices[0] = Vertex(-40.0f, 40.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	m_pVertices[1] = Vertex(40.0f, 40.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	m_pVertices[2] = Vertex(40.0f, -40.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	m_pVertices[3] = Vertex(-40.0f, -40.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	return S_OK;
}

void CShader_3dapi_03_26::Release()
{
	if (m_pShader)
	{
		m_pShader->Release();
		m_pShader = NULL;
	}

	if (m_pDiffuseTex)
	{
		m_pDiffuseTex->Release();
		m_pDiffuseTex = NULL;
	}

	if (m_pLightTex)
	{
		m_pLightTex->Release();
		m_pLightTex = NULL;
	}
}

void CShader_3dapi_03_26::Render()
{
	if (m_pdev)
	{
		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pdev->SetFVF(Vertex::FVF);

		m_pShader->SetTechnique("Tech0");

		Vertex pVertices[4];

		m_pShader->Begin(NULL, 0);

		m_pShader->BeginPass(0);
		{
			//왼쪽 사각형1
			memcpy(pVertices, m_pVertices, 4 * sizeof(Vertex));

			for (int i = 0; i < 4; i++)
			{
				pVertices[i].p += D3DXVECTOR3(-55, 41, 0);
			}

			m_pShader->SetTexture("g_DiffuseTex", m_pDiffuseTex);
			m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, pVertices, sizeof(Vertex));
		}
		m_pShader->EndPass();


		m_pShader->BeginPass(0);
		{
			//왼쪽 사각형2
			memcpy(pVertices, m_pVertices, 4 * sizeof(Vertex));

			for (int i = 0; i < 4; i++)
			{
				pVertices[i].p += D3DXVECTOR3(-55, -41, 0);
			}

			m_pShader->SetTexture("g_DiffuseTex", m_pLightTex);
			m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, pVertices, sizeof(Vertex));
		}
		m_pShader->EndPass();


		m_pShader->BeginPass(1);
		{
			m_pShader->SetTexture("g_DiffuseTex", m_pDiffuseTex);
			m_pShader->SetTexture("g_LightTex", m_pLightTex);

			//오른쪽 사각형1
			memcpy(pVertices, m_pVertices, 4 * sizeof(Vertex));

			for (int i = 0; i < 4; i++)
			{
				pVertices[i].p += D3DXVECTOR3(55, 41, 0);
			}

			m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, pVertices, sizeof(Vertex));
		}
		m_pShader->EndPass();


		m_pShader->BeginPass(2);
		{
			m_pShader->SetTexture("g_DiffuseTex", m_pDiffuseTex);
			m_pShader->SetTexture("g_LightTex", m_pLightTex);

			//오른쪽 사각형2
			memcpy(pVertices, m_pVertices, 4 * sizeof(Vertex));

			for (int i = 0; i < 4; i++)
			{
				pVertices[i].p += D3DXVECTOR3(55, -41, 0);
			}

			m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, pVertices, sizeof(Vertex));
		}
		m_pShader->EndPass();

		m_pShader->End();
	}
}

void CShader_3dapi_03_26::Update()
{
	if (m_pdev)
	{

	}
}
