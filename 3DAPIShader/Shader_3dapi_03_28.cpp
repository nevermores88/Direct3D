#include "stdafx.h"
#include "Shader_3dapi_03_28.h"

CShader_3dapi_03_28::CShader_3dapi_03_28()
{
}


CShader_3dapi_03_28::~CShader_3dapi_03_28()
{
}

HRESULT CShader_3dapi_03_28::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pShader = LoadShader("Ex03_28/Shader.fx");
	if (!m_pShader)
		return E_FAIL;

	D3DXCreateTextureFromFile(m_pdev, "Ex03_28/rock_d.tga", &m_pDiffuseTex);
	D3DXCreateTextureFromFile(m_pdev, "Ex03_28/rock_n.tga", &m_pNormalTex);

	m_pVertices[0] = Vertex(-40.f, -40.f, 0.0f, 0.0f, 1.0f);
	m_pVertices[1] = Vertex(-40.f, 40.f, 0.0f, 0.0f, 0.0f);
	m_pVertices[2] = Vertex(40.f, 40.f, 0.0f, 1.0f, 0.0f);
	m_pVertices[3] = Vertex(40.f, -40.f, 0.0f, 1.0f, 1.0f);

	return S_OK;
}

void CShader_3dapi_03_28::Release()
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

	if (m_pNormalTex)
	{
		m_pNormalTex->Release();
		m_pNormalTex = NULL;
	}
}

void CShader_3dapi_03_28::Render()
{
	if (m_pdev)
	{
		D3DXVECTOR3 vLight(-1.0f, 0.0f, 1.0f);
		m_dwTFactor = CD3DUtility::VectorToRGB(&vLight);

		D3DXCOLOR colorFactor = m_dwTFactor;

		Vertex pVertices[4];

		m_pdev->SetFVF(Vertex::FVF);

		m_pShader->SetTexture("g_DiffuseTex", m_pDiffuseTex);
		m_pShader->SetTexture("g_NormalTex", m_pNormalTex);
		m_pShader->SetVector("g_dwTFactor", (D3DXVECTOR4*)&colorFactor);

		m_pShader->SetTechnique("Tech0");

		m_pShader->Begin(NULL, 0);

		//좌상단
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


		////중앙상단
		m_pShader->BeginPass(1);
		{
			memcpy(pVertices, m_pVertices, 4 * sizeof(Vertex));

			for (int i = 0; i < 4; i++)
			{
				pVertices[i].p += D3DXVECTOR3(0.0f, 45.0f, 0);
			}

			m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, pVertices, sizeof(Vertex));
		}
		m_pShader->EndPass();


		//우측상단
		m_pShader->BeginPass(2);
		{
			memcpy(pVertices, m_pVertices, 4 * sizeof(Vertex));

			for (int i = 0; i < 4; i++)
			{
				pVertices[i].p += D3DXVECTOR3(90.0f, 45.0f, 0);
			}

			m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, pVertices, sizeof(Vertex));
		}
		m_pShader->EndPass();


		//좌측하단
		m_pShader->BeginPass(3);
		{
			//오른쪽 사각형2
			memcpy(pVertices, m_pVertices, 4 * sizeof(Vertex));

			for (int i = 0; i < 4; i++)
			{
				pVertices[i].p += D3DXVECTOR3(-90.0f, -45.0f, 0);
			}

			m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, pVertices, sizeof(Vertex));
		}
		m_pShader->EndPass();


		//중앙하단
		m_pShader->BeginPass(4);
		{
			//오른쪽 사각형2
			memcpy(pVertices, m_pVertices, 4 * sizeof(Vertex));

			for (int i = 0; i < 4; i++)
			{
				pVertices[i].p += D3DXVECTOR3(0.0f, -45.0f, 0);
			}

			m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, pVertices, sizeof(Vertex));
		}
		m_pShader->EndPass();


		//우측하단
		m_pShader->BeginPass(5);
		{
			//오른쪽 사각형2
			memcpy(pVertices, m_pVertices, 4 * sizeof(Vertex));

			for (int i = 0; i < 4; i++)
			{
				pVertices[i].p += D3DXVECTOR3(90.0f, -45.0f, 0);
			}

			m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, pVertices, sizeof(Vertex));
		}
		m_pShader->EndPass();

		m_pShader->End();
	}
}

void CShader_3dapi_03_28::Update()
{

}
