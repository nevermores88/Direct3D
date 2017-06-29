#include "stdafx.h"
#include "Shader_3dapi_03_34.h"


CShader_3dapi_03_34::CShader_3dapi_03_34()
{
}


CShader_3dapi_03_34::~CShader_3dapi_03_34()
{
}

HRESULT CShader_3dapi_03_34::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pShader = LoadShader("Ex03_34/Shader.fx");
	if (!m_pShader)
		return E_FAIL;

	D3DXCreateTextureFromFile(m_pdev, "Ex03_34/rock_d.tga", &m_pDiffuseTex);
	D3DXCreateTextureFromFile(m_pdev, "Ex03_34/rock_n.tga", &m_pNormalTex);
	D3DXCreateTextureFromFile(m_pdev, "Ex03_34/rock_s.tga", &m_pSpecularTex);

	m_pVertices[0] = Vertex(-40.f, -40.f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	m_pVertices[1] = Vertex(-40.f, 40.f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	m_pVertices[2] = Vertex(40.f, 40.f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	m_pVertices[3] = Vertex(40.f, -40.f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	return S_OK;
}

void CShader_3dapi_03_34::Release()
{
	if (m_pShader)
	{
		m_pShader->Release();
		m_pShader = NULL;
	}

	if (m_pFVF)
	{
		m_pFVF->Release();
		m_pFVF = NULL;
	}

	if (m_pDiffuseTex)
	{
		m_pDiffuseTex->Release();
		m_pDiffuseTex = NULL;
	}

	if (m_pNormalTex)
	{
		m_pNormalTex->Release();
		m_pNormalTex = NULL;
	}

	if (m_pSpecularTex)
	{
		m_pSpecularTex->Release();
		m_pSpecularTex = NULL;
	}
}

void CShader_3dapi_03_34::Render()
{
	if (m_pdev)
	{
		D3DXMATRIX mtWorldViewProj;
		D3DXMATRIX mtView;
		D3DXMATRIX mtProj;
		D3DXVECTOR3 vLightDir = { -1.0f, 0.0f, 1.0f};
		m_dwTFactor = CD3DUtility::VectorToRGB(&vLightDir);

		D3DXCOLOR dwTFactor = m_dwTFactor;

		Vertex pVertices[4];

		mtWorldViewProj = mtView * mtProj;

		m_pdev->SetFVF(Vertex::FVF);

		m_pShader->SetVector("g_dwTFactor", (D3DXVECTOR4*)&dwTFactor);

		m_pShader->Begin(NULL, NULL);


		//ÁÂ»ó´Ü
		m_pShader->SetTexture("g_DiffuseTex", m_pDiffuseTex);

		m_pShader->BeginPass(0);
		{
			memcpy(pVertices, m_pVertices, 4 * sizeof(Vertex));

			for (int i = 0; i < 4; i++)
			{
				pVertices[i].p += D3DXVECTOR3(-90.0f, 45.0f, 0);
			}

			m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, pVertices, sizeof(Vertex));
		}
		m_pShader->EndPass();


		//Áß¾Ó»ó´Ü
		m_pShader->SetTexture("g_DiffuseTex", m_pNormalTex);

		m_pShader->BeginPass(0);
		{
			memcpy(pVertices, m_pVertices, 4 * sizeof(Vertex));

			for (int i = 0; i < 4; i++)
			{
				pVertices[i].p += D3DXVECTOR3(0.0f, 45.0f, 0);
			}

			m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, pVertices, sizeof(Vertex));
		}
		m_pShader->EndPass();


		//¿ì»ó´Ü
		m_pShader->SetTexture("g_DiffuseTex", m_pSpecularTex);

		m_pShader->BeginPass(0);
		{
			memcpy(pVertices, m_pVertices, 4 * sizeof(Vertex));

			for (int i = 0; i < 4; i++)
			{
				pVertices[i].p += D3DXVECTOR3(90.0f, 45.0f, 0);
			}

			m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, pVertices, sizeof(Vertex));
		}
		m_pShader->EndPass();


		//ÁÂÇÏ´Ü
		m_pShader->SetTexture("g_NormalTex", m_pNormalTex);

		m_pShader->BeginPass(1);
		{
			memcpy(pVertices, m_pVertices, 4 * sizeof(Vertex));

			for (int i = 0; i < 4; i++)
			{
				pVertices[i].p += D3DXVECTOR3(-90.0f, -45.0f, 0);
			}

			m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, pVertices, sizeof(Vertex));
		}
		m_pShader->EndPass();


		//Áß¾ÓÇÏ´Ü
		m_pShader->SetTexture("g_DiffuseTex", m_pDiffuseTex);
		m_pShader->SetTexture("g_NormalTex", m_pNormalTex);
		m_pShader->SetTexture("g_SpecularTex", m_pSpecularTex);

		m_pShader->BeginPass(2);
		{
			memcpy(pVertices, m_pVertices, 4 * sizeof(Vertex));

			for (int i = 0; i < 4; i++)
			{
				pVertices[i].p += D3DXVECTOR3(0.0f, -45.0f, 0);
			}

			m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, pVertices, sizeof(Vertex));
		}
		m_pShader->EndPass();


		//¿ìÇÏ´Ü
		m_pShader->SetTexture("g_DiffuseTex", m_pDiffuseTex);
		m_pShader->SetTexture("g_NormalTex", m_pNormalTex);
		m_pShader->SetTexture("g_SpecularTex", m_pSpecularTex);

		m_pShader->BeginPass(3);
		{
			memcpy(pVertices, m_pVertices, 4 * sizeof(Vertex));

			for (int i = 0; i < 4; i++)
			{
				pVertices[i].p += D3DXVECTOR3(90.0f, -45.0f, 0);
			}

			m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, pVertices, sizeof(Vertex));
		}
		m_pShader->EndPass();


		m_pShader->End();

		m_pdev->SetVertexShader(NULL);
		m_pdev->SetPixelShader(NULL);
		m_pdev->SetVertexDeclaration(NULL);
	}
}

void CShader_3dapi_03_34::Update()
{
	if (m_pdev)
	{

	}
}
