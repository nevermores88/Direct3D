#include "stdafx.h"
#include "Ex19_Effect01.h"


CEx19_Effect01::CEx19_Effect01()
{
}


CEx19_Effect01::~CEx19_Effect01()
{
}

void CEx19_Effect01::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;
	
	HRESULT hr = 0;


	//Load Mesh
	LPD3DXBUFFER	pMtrlBuffer;
	DWORD	dwMtrlsNum;

	hr = D3DXLoadMeshFromX(
		L"Ex19/mountain.x",
		D3DXMESH_MANAGED,
		m_pdev,
		0, &pMtrlBuffer, 0, &dwMtrlsNum, &m_pMesh);

	D3DXMATERIAL* pMtrls = (D3DXMATERIAL*)pMtrlBuffer->GetBufferPointer();

	for (DWORD i = 0; i < dwMtrlsNum; i++)
	{
		pMtrls[i].MatD3D.Ambient = pMtrls[i].MatD3D.Diffuse;
		m_vMtrls.push_back(pMtrls[i].MatD3D);

		if (pMtrls[i].pTextureFilename != 0)
		{
			LPDIRECT3DTEXTURE9		pTex = NULL;
			D3DXCreateTextureFromFileA(
				pdev,
				pMtrls[i].pTextureFilename,
				&pTex);

			m_vTextures.push_back(pTex);
		}
		else
		{
			m_vTextures.push_back(0);
		}
	}

	pMtrlBuffer->Release();


	//Load Shader
	LPD3DXBUFFER pErrorBuffer = 0;
	hr = D3DXCreateEffectFromFile(
		m_pdev,
		L"Ex19/light_tex.fx", 0, 0, D3DXSHADER_DEBUG, 0, &m_pShader, &pErrorBuffer);

	if (pErrorBuffer)
	{
		::MessageBoxA(0, (char*)pErrorBuffer->GetBufferPointer(), 0, 0);
		pErrorBuffer->Release();
	}

	m_hdWorldMatrix = m_pShader->GetParameterByName(0, "g_mtWorld");
	m_hdViewMatrix = m_pShader->GetParameterByName(0, "g_mtView");
	m_hdProjMatrix = m_pShader->GetParameterByName(0, "g_mtProj");
	m_hdTex = m_pShader->GetParameterByName(0, "g_Tex");

	m_hdTech = m_pShader->GetTechniqueByName("LightAndTexture");


	//Load Texture
	m_pTex = NULL;
	D3DXCreateTextureFromFile(m_pdev, L"Ex19/Terrain_3x_diffcol.jpg", &m_pTex);

	
}

void CEx19_Effect01::Release()
{
	if (m_pShader)
	{
		m_pShader->Release();
		m_pShader = NULL;
	}
	
	if (m_pMesh)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	if (m_pTex)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
		
	for (size_t i = 0; i < m_vTextures.size(); i++)
		m_vTextures[i]->Release();
}

void CEx19_Effect01::OnRender()
{
	if (m_pdev)
	{
		D3DXMATRIX mtWorld;
		D3DXMATRIX mtView;
		D3DXMATRIX mtProj;

		D3DXMatrixIdentity(&mtWorld);
		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);

		m_pShader->SetMatrix(m_hdWorldMatrix, &mtWorld);
		m_pShader->SetMatrix(m_hdViewMatrix, &mtView);
		m_pShader->SetMatrix(m_hdProjMatrix, &mtProj);
		m_pShader->SetTexture(m_hdTex, m_pTex);

		m_pShader->SetTechnique(m_hdTech);

		UINT numPasses = 0;
		m_pShader->Begin(&numPasses, NULL);
		{
			for (UINT i = 0; i < numPasses; ++i)
			{
				m_pShader->BeginPass(i);
				{
					for (size_t j = 0; j < m_vMtrls.size(); ++j)
						m_pMesh->DrawSubset(j);
				}
				m_pShader->EndPass();
			}
		}
		m_pShader->End();
	}
}

void CEx19_Effect01::OnUpdate()
{
	if (m_pdev)
	{

	}
}
