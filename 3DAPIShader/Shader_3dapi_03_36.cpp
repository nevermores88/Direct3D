#include "stdafx.h"
#include "Shader_3dapi_03_36.h"


CShader_3dapi_03_36::CShader_3dapi_03_36()
{
}


CShader_3dapi_03_36::~CShader_3dapi_03_36()
{
}

HRESULT CShader_3dapi_03_36::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pShader = LoadShader("Ex03_36/Shader.fx");
	if (!m_pShader)
		return E_FAIL;

	D3DXCreateTextureFromFile(m_pdev, "Ex03_36/rock_d.tga", &m_pDiffuseTex);
	D3DXCreateTextureFromFile(m_pdev, "Ex03_36/rock_n.tga", &m_pNormalTex);
	D3DXCreateTextureFromFile(m_pdev, "Ex03_36/rock_s.tga", &m_pSpecularTex);

	DWORD dwFVF = Vertex::FVF;
	D3DVERTEXELEMENT9	vertex_decl[MAX_FVF_DECL_SIZE];
	D3DXDeclaratorFromFVF(dwFVF, vertex_decl);
	if (FAILED(m_pdev->CreateVertexDeclaration(vertex_decl, &m_pFVF)))
		return E_FAIL;

	LPD3DXMESH	pMeshOrg = NULL;
	LPD3DXBUFFER	pAdjBuffer = NULL;

	D3DXLoadMeshFromX("Ex03_36/teapot.x", D3DXMESH_SYSTEMMEM, m_pdev, &pAdjBuffer, NULL, NULL, NULL, &pMeshOrg);
	pMeshOrg->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)pAdjBuffer->GetBufferPointer(), NULL, NULL, NULL);

	pMeshOrg->CloneMeshFVF(D3DXMESH_MANAGED, Vertex::FVF, m_pdev, &m_pMesh);
	D3DXComputeNormals(m_pMesh, NULL);

	if (pMeshOrg)
		pMeshOrg->Release();

	if (pAdjBuffer)
		pAdjBuffer->Release();

	return S_OK;
}

void CShader_3dapi_03_36::Release()
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

void CShader_3dapi_03_36::Render()
{
	if (m_pdev)
	{
		D3DXMATRIX mtWorld;
		D3DXMATRIX mtView;
		D3DXMATRIX mtProj;

		D3DXMATRIX	mtRot;
		D3DXMATRIX	mtScale;

		D3DXMATRIX	mtViewInv;
		D3DXVECTOR4	vCamPos;

		D3DXVECTOR4	vLightDir;
		float fSharpness = 6.0f;

		static float fRot = 0.0f;
		fRot += 0.002f;
		float fTime = -D3DXToRadian(fRot);

		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);

		D3DXMatrixInverse(&mtViewInv, NULL, &mtView);

		vCamPos = D3DXVECTOR4(mtViewInv._41, mtViewInv._42, mtViewInv._43, 0);
		vLightDir = D3DXVECTOR4(m_vLightDir.x, m_vLightDir.y, m_vLightDir.z, 0);


		D3DXMatrixRotationY(&mtRot, fRot);
		D3DXMatrixScaling(&mtScale, 50.0f, 50.0f, 50.0f);

		mtWorld = mtRot*mtScale;

		m_pdev->SetVertexDeclaration(m_pFVF);

		m_pShader->SetMatrix("g_mtWorld", &mtWorld);
		m_pShader->SetMatrix("g_mtView", &mtView);
		m_pShader->SetMatrix("g_mtProj", &mtProj);
		m_pShader->SetMatrix("g_mtRot", &mtRot);

		m_pShader->SetVector("g_vCamPos", &vCamPos);
		m_pShader->SetVector("g_vLightDir", &vLightDir);
		m_pShader->SetFloat("g_fSharpness", fSharpness);

		m_pShader->SetTexture("g_DiffuseTex", m_pDiffuseTex);
		m_pShader->SetTexture("g_NormalTex", m_pNormalTex);
		m_pShader->SetTexture("g_SpecularTex", m_pSpecularTex);

		D3DXVECTOR4 vColor(1.3f, 1.0f, 0.5f, 1.0f);
		D3DXVECTOR4 vColor1(0.5f, 1.3f, 1.0f, 1.0f);
		D3DXVECTOR4 vColor2(1.0f, 0.5f, 1.3f, 1.0f);

		m_pShader->SetVector("g_vColor", &vColor);

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

		m_pdev->SetVertexShader(NULL);
		m_pdev->SetPixelShader(NULL);
		m_pdev->SetVertexDeclaration(NULL);
	}
}

void CShader_3dapi_03_36::Update()
{
	if (m_pdev)
	{
		D3DXMATRIX	mtTM;
		D3DXMATRIX	mtRotX;
		D3DXMATRIX	mtRotZ;

		static float fRot = 0.0f;
		fRot += 0.3f;
		FLOAT	fAngle = D3DXToRadian(fRot);
		D3DXMatrixRotationY(&mtTM, fAngle*3.f);
		D3DXMatrixRotationZ(&mtRotZ, fAngle*2.f);
		D3DXMatrixRotationX(&mtRotX, fAngle*1.f);

		mtTM *= mtRotZ;
		mtTM *= mtRotX;

		D3DXVec3TransformCoord(&m_vLightDir, &D3DXVECTOR3(-1, -1, 0), &mtTM);
	}
}
