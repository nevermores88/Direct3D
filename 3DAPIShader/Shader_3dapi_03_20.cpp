#include "stdafx.h"
#include "Shader_3dapi_03_20.h"
#include "Mesh.h"

CShader_3dapi_03_20::CShader_3dapi_03_20()
{
}


CShader_3dapi_03_20::~CShader_3dapi_03_20()
{
}

HRESULT CShader_3dapi_03_20::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pShader = LoadShader("Ex03_20/Shader.fx");
	if (!m_pShader)
		return E_FAIL;

	DWORD dwFVF = Vertex::FVF;
	D3DVERTEXELEMENT9	vertex_decl[MAX_FVF_DECL_SIZE];
	D3DXDeclaratorFromFVF(dwFVF, vertex_decl);
	if (FAILED(m_pdev->CreateVertexDeclaration(vertex_decl, &m_pFVF)))
		return E_FAIL;

	D3DXCreateTextureFromFile(m_pdev, "Ex03_20/wood.jpg", &m_pTex);

	m_pMesh = NULL;
	m_pMesh = new CMesh;
	m_pMesh->Create(m_pdev, "Ex03_20/teapot.x");

	return S_OK;
}

void CShader_3dapi_03_20::Release()
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
}

void CShader_3dapi_03_20::Render()
{
	if (m_pdev)
	{
		static int bTexture = 0;
		if (::GetAsyncKeyState('T') & 0x8000)
			bTexture = 1;

		if (::GetAsyncKeyState('R') & 0x8000)
			bTexture = 0;

		D3DXMATRIX mtView;
		D3DXMATRIX mtViewInv;
		D3DXMATRIX mtProj;
		D3DXVECTOR4 vLightDir[4];
		D3DXVECTOR4 vLightDif[4];

		vLightDir[0] = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0);
		vLightDir[1] = D3DXVECTOR4(-1.0f, 1.0f, 1.0f, 0);
		vLightDir[2] = D3DXVECTOR4(0.0f, 1.0f, 1.0f, 0);
		vLightDir[3] = D3DXVECTOR4(0.0f, -1.0f, 0.0f, 0);

		for (int i = 0; i < 4; ++i)
			D3DXVec4Normalize(&vLightDir[i], &vLightDir[i]);

		vLightDif[0] = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
		vLightDif[1] = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
		vLightDif[2] = D3DXVECTOR4(1.0f, 0.0f, 1.0f, 1.0f);
		vLightDif[3] = D3DXVECTOR4(0.0f, 0.4f, 0.8f, 1.0f);

		D3DXVECTOR4 vCamPos;
		float fSharpness;

		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);

		D3DXMatrixInverse(&mtViewInv, NULL, &mtView);
		vCamPos = D3DXVECTOR4(mtViewInv._41, mtViewInv._42, mtViewInv._43, 0);
		fSharpness = 10.0f;

		//Render
		m_pdev->SetVertexDeclaration(m_pFVF);

		m_pShader->SetMatrix("g_mtWorld", &m_mtWorld);
		m_pShader->SetMatrix("g_mtView", &mtView);
		m_pShader->SetMatrix("g_mtProj", &mtProj);
		m_pShader->SetMatrix("g_mtRot", &m_mtRot);
		m_pShader->SetVectorArray("g_vLightDir", vLightDir, 4);
		m_pShader->SetVectorArray("g_vLightDif", vLightDif, 4);
		m_pShader->SetVector("g_vCamPos", &vCamPos);
		m_pShader->SetFloat("g_fSharpness", fSharpness);
		m_pShader->SetTexture("g_Tex", m_pTex);
		m_pShader->SetInt("g_bTexture", bTexture);

		UINT numPasses = 0;
		m_pShader->Begin(&numPasses, NULL);
		{
			for (UINT i = 0; i < numPasses; ++i)
			{
				m_pShader->BeginPass(i);
				{
					// 구체를 그린다.
					m_pMesh->Render(false);
				}
				m_pShader->EndPass();
			}
		}
		m_pShader->End();

		m_pdev->SetTexture(0, NULL);

		m_pdev->SetVertexShader(NULL);
		m_pdev->SetPixelShader(NULL);
		m_pdev->SetVertexDeclaration(NULL);
	}
}

void CShader_3dapi_03_20::Update()
{
	if (m_pdev)
	{
		D3DXMATRIX mtScale;
		D3DXMATRIX mtRotY;
		D3DXMATRIX mtRotZ;

		static float c = 0.0f;
		c += 0.1f;
		if (c > 360.0f)
			c = 0.0f;

		D3DXMatrixScaling(&mtScale, 40, 40, 40);
		D3DXMatrixRotationY(&mtRotY, D3DXToRadian(c));
		D3DXMatrixRotationZ(&mtRotZ, D3DXToRadian(-23.5f));

		m_mtRot = mtRotY * mtRotZ;
		m_mtWorld = mtScale * m_mtRot;
	}
}
