#include "stdafx.h"
#include "Ex19_Effect03.h"


CEx19_Effect03::CEx19_Effect03()
{
}


CEx19_Effect03::~CEx19_Effect03()
{
}

void CEx19_Effect03::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;

	HRESULT hr = 0;

	D3DXCreateTeapot(m_pdev, &m_pMeshes[0], 0);
	D3DXCreateSphere(m_pdev, 1.0f, 20, 20, &m_pMeshes[1], 0);
	D3DXCreateTorus(m_pdev, 0.5f, 1.0f, 20, 20, &m_pMeshes[2], 0);
	D3DXCreateCylinder(m_pdev, 0.5f, 0.5f, 2.0f, 20, 20, &m_pMeshes[3], 0);

	D3DXMatrixTranslation(&m_mtWorlds[0], 0.0f, 2.0f, 0.0f);
	D3DXMatrixTranslation(&m_mtWorlds[1], 0.0f, -2.0f, 0.0f);
	D3DXMatrixTranslation(&m_mtWorlds[2], -3.0f, 0.0f, 0.0f);
	D3DXMatrixTranslation(&m_mtWorlds[3], 3.0f, 0.0f, 0.0f);

	m_vMeshColors[0] = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	m_vMeshColors[1] = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_vMeshColors[2] = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_vMeshColors[3] = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);


	//Load Shader
	LPD3DXBUFFER pErrorBuffer = 0;
	hr = D3DXCreateEffectFromFile(
		m_pdev,
		L"Ex19/tooneffect.fx", 0, 0, D3DXSHADER_DEBUG, 0, &m_pShader, &pErrorBuffer);

	if (pErrorBuffer)
	{
		::MessageBoxA(0, (char*)pErrorBuffer->GetBufferPointer(), 0, 0);
		pErrorBuffer->Release();
	}

	m_hdWorldViewMatrix = m_pShader->GetParameterByName(0, "g_mtWorldView");
	m_hdWorldViewProjMatrix = m_pShader->GetParameterByName(0, "g_mtWorldViewProj");
	m_hdColor = m_pShader->GetParameterByName(0, "g_Color");
	m_hdLightDir = m_pShader->GetParameterByName(0, "g_vLightDir");
	m_hdTex = m_pShader->GetParameterByName(0, "g_Tex");

	m_hdTech = m_pShader->GetTechniqueByName("Toon");

	m_pTex = NULL;
	D3DXCreateTextureFromFile(m_pdev, L"Ex19/toonshade.bmp", &m_pTex);
}

void CEx19_Effect03::Release()
{
	if (m_pShader)
	{
		m_pShader->Release();
		m_pShader = NULL;
	}

	if (m_pTex)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}

	for (int i = 0; i < 4; i++)
	{
		if (m_pMeshes[i])
		{
			m_pMeshes[i]->Release();
			m_pMeshes[i] = NULL;
		}
	}
}

void CEx19_Effect03::OnRender()
{
	if (m_pdev)
	{
		D3DXMATRIX	mtWorldView[4];
		D3DXMATRIX	mtWorldViewProj[4];

		D3DXVECTOR4 vLightDir(-0.57f, 0.57f, -0.57f, 0.0f);

		D3DXMATRIX mtView;
		D3DXMATRIX mtProj;

		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);


		m_pShader->SetVector(m_hdLightDir, &vLightDir);
		m_pShader->SetTexture(m_hdTex, m_pTex);

		m_pShader->SetTechnique(m_hdTech);

		UINT numPasses = 0;
		m_pShader->Begin(&numPasses, NULL);
		{
			for (UINT i = 0; i < numPasses; ++i)
			{
				//MEMO : 이상하다 검색해서 수정
				//이렇게 하면 물체가 겹쳐서 출력됨. 다이렉트x9 문제인듯.
				m_pShader->BeginPass(i);
				{
					for (int j = 0; j < 4; j++)
					{
						//m_pShader->BeginPass(i);

						mtWorldView[j] = m_mtWorlds[j] * mtView;
						mtWorldViewProj[j] = m_mtWorlds[j] * mtView * mtProj;

						m_pShader->SetMatrix(m_hdWorldViewMatrix, &mtWorldView[j]);
						m_pShader->SetMatrix(m_hdWorldViewProjMatrix, &mtWorldViewProj[j]);
						m_pShader->SetVector(m_hdColor, &m_vMeshColors[j]);

						//BeginPass, EndPass 안에서 Set~ 함수를 호출하면 제대로 적용이 안된다.
						//CommitChanges 함수를 호출해서 제대로 적용하게 해줘야함!!!
						m_pShader->CommitChanges();

						m_pMeshes[j]->DrawSubset(0);

						//m_pShader->EndPass();
					}
				}
				m_pShader->EndPass();
			}
		}
		m_pShader->End();
	}
}

void CEx19_Effect03::OnUpdate()
{
	if (m_pdev)
	{

	}
}
