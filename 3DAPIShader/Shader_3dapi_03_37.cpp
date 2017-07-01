#include "stdafx.h"
#include "Shader_3dapi_03_37.h"
#include "Mesh.h"

CShader_3dapi_03_37::CShader_3dapi_03_37()
{
}


CShader_3dapi_03_37::~CShader_3dapi_03_37()
{
}

HRESULT CShader_3dapi_03_37::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pShader = LoadShader("Ex03_37/Shader.fx");
	if (!m_pShader)
		return E_FAIL;

	D3DXCreateTextureFromFile(m_pdev, "Ex03_37/spheremap.bmp", &m_pEnvTex);


	DWORD dwFVF = Vertex::FVF;
	D3DVERTEXELEMENT9	vertex_decl[MAX_FVF_DECL_SIZE];
	D3DXDeclaratorFromFVF(dwFVF, vertex_decl);
	if (FAILED(m_pdev->CreateVertexDeclaration(vertex_decl, &m_pFVF)))
		return E_FAIL;

	m_pMesh = new CMesh;
	m_pMesh->Create(m_pdev, "Ex03_37/teapot.x");

	return S_OK;
}

void CShader_3dapi_03_37::Release()
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

	if (m_pMesh)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	if (m_pEnvTex)
	{
		m_pEnvTex->Release();
		m_pEnvTex = NULL;
	}
}

void CShader_3dapi_03_37::Render()
{
	if (m_pdev)
	{
		D3DXMATRIX mtWorld = m_mtWorld;
		D3DXMATRIX mtView;
		D3DXMATRIX mtProj;

		D3DXMATRIX	mtRot = m_mtRot;

		D3DXMATRIX	mtViewInv;
		D3DXVECTOR4	vCamPos;

		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);

		D3DXMatrixInverse(&mtViewInv, NULL, &mtView);

		vCamPos = D3DXVECTOR4(mtViewInv._41, mtViewInv._42, mtViewInv._43, 0);

		m_pdev->SetVertexDeclaration(m_pFVF);

		m_pShader->SetMatrix("g_mtWorld", &mtWorld);
		m_pShader->SetMatrix("g_mtView", &mtView);
		m_pShader->SetMatrix("g_mtProj", &mtProj);
		m_pShader->SetMatrix("g_mtRot", &mtRot);

		m_pShader->SetVector("g_vCamPos", &vCamPos);

		m_pShader->SetTexture("g_EnvTex", m_pEnvTex);


		UINT numPasses = 0;
		m_pShader->Begin(&numPasses, NULL);
		{
			for (UINT i = 0; i < numPasses; ++i)
			{
				m_pShader->BeginPass(i);
				{
					m_pMesh->Render(false, false);
				}
				m_pShader->EndPass();
			}
		}
		m_pShader->End();

		m_pdev->SetVertexShader(NULL);
		m_pdev->SetPixelShader(NULL);
		m_pdev->SetVertexDeclaration(NULL);
	}
}

void CShader_3dapi_03_37::Update()
{
	if (m_pdev)
	{
		D3DXMATRIX	mtScale;
		D3DXMATRIX	mtRotY;
		D3DXMATRIX	mtRotZ;

		static float fRot = 0.0f;
		fRot += 0.3f;
		FLOAT	fAngle = D3DXToRadian(fRot);
		D3DXMatrixScaling(&mtScale, 50.0f, 50.0f, 50.0f);
		D3DXMatrixRotationY(&mtRotY, D3DXToRadian(fRot));
		D3DXMatrixRotationX(&mtRotZ, D3DXToRadian(-23.5f));

		m_mtRot = mtRotY * mtRotZ;
		m_mtWorld = mtScale * m_mtRot;
		m_mtWorld._41 = 0;
		m_mtWorld._42 = 0;
		m_mtWorld._43 = 0;
	}
}
