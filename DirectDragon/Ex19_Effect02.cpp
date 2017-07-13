#include "stdafx.h"
#include "Ex19_Effect02.h"


CEx19_Effect02::CEx19_Effect02()
{
}


CEx19_Effect02::~CEx19_Effect02()
{
}

void CEx19_Effect02::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;

	HRESULT hr = 0;

	D3DXVECTOR3 vLightDir(0.0f, 1.0f, 0.0f);
	m_pTerrain = new CTerrain();
	m_pTerrain->Create(m_pdev, "Ex19/coastMountain64.raw", 64, 64, 6, 0.5f);
	m_pTerrain->GenTexture(&vLightDir);

	m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);


	//Load Shader
	LPD3DXBUFFER pErrorBuffer = 0;
	hr = D3DXCreateEffectFromFile(
		m_pdev,
		L"Ex19/fog.fx", 0, 0, D3DXSHADER_DEBUG, 0, &m_pShader, &pErrorBuffer);

	if (pErrorBuffer)
	{
		::MessageBoxA(0, (char*)pErrorBuffer->GetBufferPointer(), 0, 0);
		pErrorBuffer->Release();
	}

	m_hdTech = m_pShader->GetTechniqueByName("Fog");
}

void CEx19_Effect02::Release()
{
	if (m_pShader)
	{
		m_pShader->Release();
		m_pShader = NULL;
	}
}

void CEx19_Effect02::OnRender()
{
	if (m_pdev)
	{
		m_pShader->SetTechnique(m_hdTech);

		D3DXMATRIX mtIdentity;
		D3DXMatrixIdentity(&mtIdentity);

		UINT numPasses = 0;
		m_pShader->Begin(&numPasses, NULL);
		{
			for (UINT i = 0; i < numPasses; ++i)
			{
				m_pShader->BeginPass(i);
				{
					if (m_pTerrain)
						m_pTerrain->Draw(&mtIdentity, false);
				}
				m_pShader->EndPass();
			}
		}
		m_pShader->End();
	}
}

void CEx19_Effect02::OnUpdate()
{
	if (m_pdev)
	{

	}
}
