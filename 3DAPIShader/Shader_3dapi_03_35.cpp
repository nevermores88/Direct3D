#include "stdafx.h"
#include "Shader_3dapi_03_35.h"


CShader_3dapi_03_35::CShader_3dapi_03_35()
{
}


CShader_3dapi_03_35::~CShader_3dapi_03_35()
{
}

HRESULT CShader_3dapi_03_35::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pShader = LoadShader("Ex03_35/Shader.fx");
	if (!m_pShader)
		return E_FAIL;

	D3DXCreateTextureFromFile(m_pdev, "Ex03_35/rock_d.tga", &m_pDiffuseTex);
	D3DXCreateTextureFromFile(m_pdev, "Ex03_35/rock_n.tga", &m_pNormalTex);
	D3DXCreateTextureFromFile(m_pdev, "Ex03_35/rock_s.tga", &m_pSpecularTex);

	DWORD dwFVF = Vertex::FVF;
	D3DVERTEXELEMENT9	vertex_decl[MAX_FVF_DECL_SIZE];
	D3DXDeclaratorFromFVF(dwFVF, vertex_decl);
	if (FAILED(m_pdev->CreateVertexDeclaration(vertex_decl, &m_pFVF)))
		return E_FAIL;

	m_pVertices[0] = Vertex(-40.f, -40.f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	m_pVertices[1] = Vertex(-40.f, 40.f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	m_pVertices[2] = Vertex(40.f, 40.f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	m_pVertices[3] = Vertex(40.f, -40.f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	m_vLightDir = D3DXVECTOR4(0.0f, -2.0f, 1.0f, 0.0f);

	return S_OK;
}

void CShader_3dapi_03_35::Release()
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

void CShader_3dapi_03_35::Render()
{
	if (m_pdev)
	{
		D3DXMATRIX mtWorldViewProj;
		D3DXMATRIX mtView;
		D3DXMATRIX mtProj;
		D3DXVECTOR3 vLightDir = { -1.0f, 0.0f, 1.0f };

		Vertex pVertices[4];

		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);
		mtWorldViewProj = mtView * mtProj;

		m_pdev->SetFVF(Vertex::FVF);

		m_pShader->SetMatrix("g_mtWorldViewProj", &mtWorldViewProj);
		m_pShader->SetVector("g_vLightDir", &m_vLightDir);
		m_pShader->SetTexture("g_DiffuseTex", m_pDiffuseTex);
		m_pShader->SetTexture("g_NormalTex", m_pNormalTex);
		m_pShader->SetTexture("g_SpecularTex", m_pSpecularTex);

		m_pShader->Begin(NULL, NULL);


		//ÁÂ»ó´Ü
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


		//¿ì»ó´Ü
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


		//ÁÂÇÏ´Ü
		m_pShader->BeginPass(3);
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
		m_pShader->BeginPass(4);
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
		m_pShader->BeginPass(5);
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

void CShader_3dapi_03_35::Update()
{
	if (m_pdev)
	{
		D3DXVec4Normalize(&m_vLightDir, &m_vLightDir);

		if (::GetAsyncKeyState('E'))
		{
			m_vLightDir.x += 0.01f;
		}
		if (::GetAsyncKeyState('R'))
		{
			m_vLightDir.x -= 0.01f;
		}
	}
}
