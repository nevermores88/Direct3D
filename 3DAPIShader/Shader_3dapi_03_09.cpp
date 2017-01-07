#include "stdafx.h"
#include "Shader_3dapi_03_09.h"


CShader_3dapi_03_09::CShader_3dapi_03_09()
{
}


CShader_3dapi_03_09::~CShader_3dapi_03_09()
{
}

HRESULT CShader_3dapi_03_09::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pShader = LoadShader("Ex03_09/Shader.fx");
	if (!m_pShader)
		return E_FAIL;

	m_pTex = LoadTexture("Ex03_09/wood.jpg");
	if (!m_pTex)
		return E_FAIL;

	m_pMesh = LoadModel("Ex03_09/teapot.x");
	if (!m_pMesh)
		return E_FAIL;

	DWORD dwFVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);
	D3DVERTEXELEMENT9 vertex_decl[MAX_FVF_DECL_SIZE];
	D3DXDeclaratorFromFVF(dwFVF, vertex_decl);
	if (FAILED(m_pdev->CreateVertexDeclaration(vertex_decl, &m_pFVF)))
		return E_FAIL;

	return S_OK;
}

void CShader_3dapi_03_09::Release()
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

	if (m_pMesh)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	if (m_pFVF)
	{
		m_pFVF->Release();
		m_pFVF = NULL;
	}
}

void CShader_3dapi_03_09::Render()
{
	if (m_pdev)
	{
		D3DXMATRIX mtView;
		D3DXMATRIX mtProj;
		
		D3DXVECTOR4 vLightDir(1.0f, 0.0f, 0.0f, 0.0f);				//±¤¿ø¹æÇâ
		D3DXVECTOR4 vCamZ(0, 0, 0, 0);

		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);

		D3DXVec4Normalize(&vLightDir, &vLightDir);

		D3DXMATRIX mtViewInv;
		D3DXMatrixInverse(&mtViewInv, NULL, &mtViewInv);
		vCamZ = D3DXVECTOR4(mtViewInv._41, mtViewInv._42, mtViewInv._43, 0);

		m_pdev->SetVertexDeclaration(m_pFVF);

		m_pShader->SetMatrix("g_mtWorld0", &m_mtWorld0);		// World Transform Matrix
		m_pShader->SetMatrix("g_mtView", &mtView);		// View Transform Matrix
		m_pShader->SetMatrix("g_mtProj", &mtProj);		// Projection Transform Matrix
		m_pShader->SetMatrix("g_mtRot", &m_mtRot);		// Rotation Matrix
		m_pShader->SetVector("g_vLightDir", &vLightDir);		// Lighting Direction Vector
		m_pShader->SetVector("g_vCamZ", &vCamZ);	// Camera Z Axis Vector
		m_pShader->SetTexture("g_DiffuseTex", m_pTex);		// Diffuse Map Texure

		m_pShader->SetTechnique("Tech0");

		UINT numPasses = 0;

		m_pShader->Begin(&numPasses, NULL);

		for (UINT i = 0; i < numPasses; ++i)
		{
			m_pShader->BeginPass(i);
			{
				m_pMesh->DrawSubset(0);
			}
			m_pShader->EndPass();
		}

		m_pShader->End();

		m_pdev->SetVertexShader(NULL);
		m_pdev->SetPixelShader(NULL);
		m_pdev->SetVertexDeclaration(NULL);
	}
}

void CShader_3dapi_03_09::Update()
{
	if (m_pdev)
	{
		static float c = 0;

		c += 25.f * 0.05f;
		//	c=90;
		if (c > 360.f)
			c -= 360.f;

		D3DXMATRIX	mtY;
		D3DXMATRIX	mtZ;
		D3DXMATRIX mtS;
		D3DXMATRIX mtT;

		// Update World Matrix
		D3DXMatrixScaling(&mtS, 40, 40, 40);
		D3DXMatrixRotationY(&mtY, D3DXToRadian(-c));
		D3DXMatrixRotationZ(&mtZ, D3DXToRadian(-23.5f));
		D3DXMatrixTranslation(&mtT, 0.0f, 0.0f, 0.0f);
		m_mtRot = mtY * mtZ;
		m_mtWorld0 = mtS * mtY * mtZ * mtT;
		//m_mtWorld0._41 = 10; m_mtWorld0._42 = 30;
		m_mtWorld1 = mtT;
	}
}
