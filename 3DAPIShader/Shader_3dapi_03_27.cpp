#include "stdafx.h"
#include "Shader_3dapi_03_27.h"


CShader_3dapi_03_27::CShader_3dapi_03_27()
{
}


CShader_3dapi_03_27::~CShader_3dapi_03_27()
{
}

HRESULT CShader_3dapi_03_27::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pShader = LoadShader("Ex03_27/Shader.fx");
	if (!m_pShader)
		return E_FAIL;

	D3DXCreateTextureFromFile(m_pdev, "Ex03_27/stones.bmp", &m_pDiffuseTex);
	D3DXCreateTextureFromFile(m_pdev, "Ex03_27/lighting.tga", &m_pLightTex);

	m_pVertices[0] = Vertex(-40.0f, 40.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	m_pVertices[1] = Vertex(40.0f, 40.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	m_pVertices[2] = Vertex(40.0f, -40.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	m_pVertices[3] = Vertex(-40.0f, -40.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	return S_OK;
}

void CShader_3dapi_03_27::Release()
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

void CShader_3dapi_03_27::Render()
{
	if (m_pdev)
	{
		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pdev->SetFVF(Vertex::FVF);

		D3DXMATRIX mtView, mtProj;

		D3DXMatrixIdentity(&m_mtWorld);
		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);
		
		m_mtWorld = m_mtWorld * mtView * mtProj;

		Vertex pVertices[4];

		m_pShader->SetTexture("g_DiffuseTex", m_pDiffuseTex);
		m_pShader->SetTexture("g_LightTex", m_pLightTex);
		m_pShader->SetMatrix("g_mtWorldViewProj", &m_mtWorld);

		m_pShader->SetTechnique("Tech0");

		m_pShader->Begin(NULL, 0);

		m_pShader->BeginPass(0);
		{
			//왼쪽 사각형1
			memcpy(pVertices, m_pVertices, 4 * sizeof(Vertex));

			for (int i = 0; i < 4; i++)
			{
				pVertices[i].p += D3DXVECTOR3(-55, 41, 0);
			}

			m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, pVertices, sizeof(Vertex));
		}
		m_pShader->EndPass();


		m_pShader->BeginPass(1);
		{
			//왼쪽 사각형2
			memcpy(pVertices, m_pVertices, 4 * sizeof(Vertex));

			for (int i = 0; i < 4; i++)
			{
				pVertices[i].p += D3DXVECTOR3(-55, -41, 0);
			}

			m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, pVertices, sizeof(Vertex));
		}
		m_pShader->EndPass();


		m_pShader->BeginPass(2);
		{
			//오른쪽 사각형1
			memcpy(pVertices, m_pVertices, 4 * sizeof(Vertex));

			for (int i = 0; i < 4; i++)
			{
				pVertices[i].p += D3DXVECTOR3(55, 41, 0);
			}

			m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, pVertices, sizeof(Vertex));
		}
		m_pShader->EndPass();


		m_pShader->BeginPass(3);
		{
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

void CShader_3dapi_03_27::Update()
{
	if (m_pdev)
	{

	}
}
