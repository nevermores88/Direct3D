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
	D3DXCreateTextureFromFile(m_pdev, "Ex03_26/lighting.tga", &m_pDiffuseTex);

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





		float fHatchingWidth = 2.0f;

		D3DXMATRIX mtView;
		D3DXMATRIX mtViewInv;
		D3DXMATRIX mtProj;
		D3DXVECTOR4 vLightDir(-1.0f, -1.0f, 0.5f, 0.0f);
		D3DXVec4Normalize(&vLightDir, &vLightDir);

		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);

		//Render
		m_pdev->SetVertexDeclaration(m_pFVF);

		m_pShader->SetMatrix("g_mtWorld", &m_mtWorld);
		m_pShader->SetMatrix("g_mtView", &mtView);
		m_pShader->SetMatrix("g_mtProj", &mtProj);
		m_pShader->SetMatrix("g_mtRot", &m_mtRot);
		m_pShader->SetVector("g_vLightDir", &vLightDir);
		m_pShader->SetFloat("g_fHatchingWidth", fHatchingWidth);
		m_pShader->SetTexture("g_DiffuseTex", m_pDiffuseTex);

		for (int i = 0; i < 6; i++)
			m_pdev->SetTexture(i, m_pTex[i]);

		UINT numPasses = 0;
		m_pShader->Begin(&numPasses, NULL);
		{
			for (UINT i = 0; i < numPasses; ++i)
			{
				m_pShader->BeginPass(i);
				{
					m_pMesh->DrawSubset(0);
				}
				m_pShader->EndPass();
			}
		}
		m_pShader->End();

		for (int i = 0; i < 6; i++)
			m_pdev->SetTexture(i, NULL);

		m_pdev->SetVertexShader(NULL);
		m_pdev->SetPixelShader(NULL);
		m_pdev->SetVertexDeclaration(NULL);
	}
}

void CShader_3dapi_03_26::Update()
{
	if (m_pdev)
	{
		D3DXMATRIX mtScale;
		D3DXMATRIX mtRotY;
		D3DXMATRIX mtRotZ;

		static float c = 0.0f;
		c += 1.0f;
		if (c > 360.0f)
			c = 0.0f;

		D3DXMatrixScaling(&mtScale, 1, 1, 1);
		D3DXMatrixRotationY(&mtRotY, D3DXToRadian(c));
		D3DXMatrixRotationZ(&mtRotZ, D3DXToRadian(-23.5f));

		m_mtRot = mtRotY * mtRotZ;
		m_mtWorld = mtScale * m_mtRot;
	}
}
