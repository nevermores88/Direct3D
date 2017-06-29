#include "stdafx.h"
#include "Shader_3dapi_03_32.h"


CShader_3dapi_03_32::CShader_3dapi_03_32()
{
}


CShader_3dapi_03_32::~CShader_3dapi_03_32()
{
}

HRESULT CShader_3dapi_03_32::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pShader = LoadShader("Ex03_32/Shader.fx");
	if (!m_pShader)
		return E_FAIL;

	D3DXCreateTextureFromFile(m_pdev, "Ex03_32/concrete.bmp", &m_pDiffuseTex);
	D3DXCreateTextureFromFile(m_pdev, "Ex03_32/saint.tga", &m_pNormalTex);

	DWORD dwFVF = Vertex::FVF;
	D3DVERTEXELEMENT9	vertex_decl[MAX_FVF_DECL_SIZE];
	D3DXDeclaratorFromFVF(dwFVF, vertex_decl);
	if (FAILED(m_pdev->CreateVertexDeclaration(vertex_decl, &m_pFVF)))
		return E_FAIL;

	m_pVertices[0] = Vertex(-40.f, -40.f, 0.0f, 0.0f, 0.0f, -1.0f,  0.0f, 1.0f);
	m_pVertices[1] = Vertex(-40.f, 40.f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	m_pVertices[2] = Vertex(40.f, 40.f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	m_pVertices[3] = Vertex(40.f, -40.f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	return S_OK;
}

void CShader_3dapi_03_32::Release()
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
}

void CShader_3dapi_03_32::Render()
{
	if (m_pdev)
	{
		D3DXMATRIX mtWorldViewProj;
		D3DXMATRIX mtView;
		D3DXMATRIX mtProj;
		D3DXVECTOR4 vLightDir = { -1, 0, 0, 0 };

		Vertex pVertices[4];

		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);

		mtWorldViewProj = mtView * mtProj;

		m_pdev->SetVertexDeclaration(m_pFVF);

		m_pShader->SetMatrix("g_mtWorldViewProj", &mtWorldViewProj);
		m_pShader->SetVector("g_vLightDir", &vLightDir);

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
		m_pShader->SetTexture("g_NormalTex", m_pNormalTex);

		m_pShader->BeginPass(1);
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
		m_pShader->SetTexture("g_DiffuseTex", m_pDiffuseTex);
		m_pShader->SetTexture("g_NormalTex", m_pNormalTex);

		m_pShader->BeginPass(2);
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

		m_pShader->BeginPass(3);
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

		m_pShader->BeginPass(4);
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

void CShader_3dapi_03_32::Update()
{
	if (m_pdev)
	{
		
	}
}
