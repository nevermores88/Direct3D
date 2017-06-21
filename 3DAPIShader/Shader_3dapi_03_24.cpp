#include "stdafx.h"
#include "Shader_3dapi_03_24.h"


CShader_3dapi_03_24::CShader_3dapi_03_24()
{
}


CShader_3dapi_03_24::~CShader_3dapi_03_24()
{
}

HRESULT CShader_3dapi_03_24::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pShader = LoadShader("Ex03_24/Shader.fx");
	if (!m_pShader)
		return E_FAIL;

	DWORD dwFVF = Vertex::FVF;
	D3DVERTEXELEMENT9	vertex_decl[MAX_FVF_DECL_SIZE];
	D3DXDeclaratorFromFVF(dwFVF, vertex_decl);
	if (FAILED(m_pdev->CreateVertexDeclaration(vertex_decl, &m_pFVF)))
		return E_FAIL;

	for (int i = 0; i < 6; i++)
	{
		char szPath[128];
		sprintf(szPath, "Ex03_24/stroke%d.bmp", i);

		D3DXCreateTextureFromFile(m_pdev, szPath, &m_pTex[i]);
	}

	LPD3DXMESH	pOrgMesh = NULL;
	LPD3DXBUFFER pAdj = NULL;

	D3DXLoadMeshFromX("Ex03_24/teapot.x", D3DXMESH_SYSTEMMEM, m_pdev, &pAdj, NULL, NULL, NULL, &pOrgMesh);

	pOrgMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE
		, (DWORD*)pAdj->GetBufferPointer(), NULL, NULL, NULL);

	pOrgMesh->CloneMeshFVF(D3DXMESH_MANAGED, Vertex::FVF, m_pdev, &m_pMesh);
	D3DXComputeNormals(m_pMesh, NULL);
	/*m_pMesh = NULL;
	m_pMesh = new CMesh;
	m_pMesh->Create(m_pdev, "Ex03_23/teapot.x");*/

	pOrgMesh->Release();
	pAdj->Release();

	return S_OK;
}

void CShader_3dapi_03_24::Release()
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

	for (int i = 0; i < 6; i++)
	{
		m_pTex[i]->Release();
		m_pTex[i] = NULL;
	}

	if (m_pMesh)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}
}

void CShader_3dapi_03_24::Render()
{
	if (m_pdev)
	{
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

void CShader_3dapi_03_24::Update()
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
