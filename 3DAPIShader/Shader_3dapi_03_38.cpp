#include "stdafx.h"
#include "Shader_3dapi_03_38.h"
#include "Mesh.h"

const int ENVMAP_RESOULTION = 512;

CShader_3dapi_03_38::CShader_3dapi_03_38()
{
}


CShader_3dapi_03_38::~CShader_3dapi_03_38()
{
}

HRESULT CShader_3dapi_03_38::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pShader = LoadShader("Ex03_38/Shader.fx");
	if (!m_pShader)
		return E_FAIL;

	DWORD dwFVF = Vertex::FVF;
	D3DVERTEXELEMENT9	vertex_decl[MAX_FVF_DECL_SIZE];
	D3DXDeclaratorFromFVF(dwFVF, vertex_decl);
	if (FAILED(m_pdev->CreateVertexDeclaration(vertex_decl, &m_pFVF)))
		return E_FAIL;

	m_pMesh = new CMesh;
	m_pMesh->Create(m_pdev, "Ex03_38/teapot.x");

	m_pSkyBox = new CMesh;
	m_pSkyBox->Create(m_pdev, "Ex03_38/lobby_skybox.x", "Ex03_38");

	Init3DEnvrionment();

	D3DXCreateSprite(m_pdev, &m_pSprite);

	return S_OK;
}

void CShader_3dapi_03_38::Release()
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

	if (m_pSkyBox)
	{
		m_pSkyBox->Release();
		m_pSkyBox = NULL;
	}

	if (m_pSphereTex)
	{
		m_pSphereTex->Release();
		m_pSphereTex = NULL;
	}

	if (m_pSprite)
	{
		m_pSprite->Release();
		m_pSprite = NULL;
	}
}

void CShader_3dapi_03_38::Render()
{
	RenderScene();

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
		
		m_pShader->SetTexture("g_EnvTex", m_pSphereTex);


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

	RenderSphereTex();
}

void CShader_3dapi_03_38::Update()
{
	if (m_pdev)
	{
		//환경 관련 업데이트
		D3DXMATRIX mtCurView;
		D3DXMATRIX mtCurProj;

		m_pdev->GetTransform(D3DTS_VIEW, &mtCurView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtCurProj);

		D3DXMATRIX	mtView[6];
		D3DXMATRIX	mtProj;

		// Set the projection matrix for a field of view of 90 degrees
		D3DXMatrixPerspectiveFovLH(&mtProj, D3DX_PI / 2.0f, 1.0f, 1.0f, 5000.0f);

		// Set the view transform for this cubemap surface
		for (int i = 0; i < 6; ++i)
		{
			T_SetupCubeViewMatrix(&mtView[i], (D3DCUBEMAP_FACES)i);
			mtView[i] = mtCurView * mtView[i];
		}

		// Rendering to Sphere surface
		m_pRenderEnv->BeginSphere(m_pSphereTex);
		{
			for (int i = 0; i < 6; ++i)
			{
				m_pRenderEnv->Face((D3DCUBEMAP_FACES)i, 0);
				RenderScene(&mtView[i], &mtProj);
			}
		}
		m_pRenderEnv->End(0);

		m_pdev->SetTransform(D3DTS_VIEW, &mtCurView);
		m_pdev->SetTransform(D3DTS_PROJECTION, &mtCurProj);


		//Teapot Update
		D3DXMATRIX	mtScale;
		D3DXMATRIX	mtRotY;
		D3DXMATRIX	mtRotZ;

		static float fRot = 0.0f;
		//fRot += 0.3f;
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

void CShader_3dapi_03_38::RenderScene()
{
	if (m_pdev)
	{
		D3DXMATRIX mtWorld;
		D3DXMatrixScaling(&mtWorld, 100.0f, 100.0f, 100.0f);

		//장면을 회전시켜도 제대로 환경매핑이 구현되는지 확인
		D3DXMATRIX	mtRotY;
		D3DXMatrixRotationY(&mtRotY, D3DXToRadian(90.0f));
		mtWorld = mtWorld * mtRotY;
		//

		m_pdev->SetTransform(D3DTS_WORLD, &mtWorld);
		m_pSkyBox->Render();

		D3DXMatrixIdentity(&mtWorld);
		m_pdev->SetTransform(D3DTS_WORLD, &mtWorld);
	}
}

void CShader_3dapi_03_38::RenderScene(D3DXMATRIX* pView, D3DXMATRIX* pProj)
{
	if (m_pdev)
	{
		m_pdev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFF006699, 1.0f, 0);
		m_pdev->SetTransform(D3DTS_VIEW, pView);
		m_pdev->SetTransform(D3DTS_PROJECTION, pProj);

		RenderScene();
	}
}

void CShader_3dapi_03_38::RenderSphereTex()
{
	if(m_pdev)
	{
		m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

		D3DXMATRIX mtScale;
		D3DXMatrixScaling(&mtScale, 0.4f, 0.4f, 1.0f);

		RECT rc = { 0, 0, ENVMAP_RESOULTION, ENVMAP_RESOULTION };
		m_pSprite->SetTransform(&mtScale);
		m_pSprite->Draw(m_pSphereTex, &rc, NULL, NULL, 0xFFFFFFFF);
		m_pSprite->End();
	}
}

void CShader_3dapi_03_38::Init3DEnvrionment()
{
	if (m_pdev)
	{
		HRESULT	hr = 0;

		LPDIRECT3DSURFACE9		pSurf;
		D3DSURFACE_DESC		descC;
		D3DSURFACE_DESC		descD;

		if (FAILED(m_pdev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pSurf)))
			return;

		pSurf->GetDesc(&descC);
		pSurf->Release();

		if (FAILED(m_pdev->GetDepthStencilSurface(&pSurf)))
			return;

		pSurf->GetDesc(&descD);
		pSurf->Release();

		// Create Rendering Environment Mapping Object
		hr = D3DXCreateRenderToEnvMap(m_pdev, ENVMAP_RESOULTION, 1, descC.Format, true, descD.Format, &m_pRenderEnv);

		if (FAILED(hr))
			return;

		// Create Spheremap
		hr = D3DXCreateTexture(m_pdev, ENVMAP_RESOULTION, ENVMAP_RESOULTION,
			1, D3DUSAGE_RENDERTARGET, descC.Format, D3DPOOL_DEFAULT, &m_pSphereTex);

		if (FAILED(hr))
			hr = D3DXCreateTexture(m_pdev, ENVMAP_RESOULTION, ENVMAP_RESOULTION,
				1, 0, descC.Format, D3DPOOL_DEFAULT, &m_pSphereTex);

		if (FAILED(hr))
			return;
	}
}

void CShader_3dapi_03_38::T_SetupCubeViewMatrix(D3DXMATRIX* pViewMatrix, DWORD dwFace)
{
	D3DXVECTOR3 vEye = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vLook;
	D3DXVECTOR3 vUp;

	switch (dwFace)
	{
	case D3DCUBEMAP_FACE_POSITIVE_X:
		vLook = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		break;
	case D3DCUBEMAP_FACE_NEGATIVE_X:
		vLook = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		break;
	case D3DCUBEMAP_FACE_POSITIVE_Y:
		vLook = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vUp = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		break;
	case D3DCUBEMAP_FACE_NEGATIVE_Y:
		vLook = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		vUp = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		break;
	case D3DCUBEMAP_FACE_POSITIVE_Z:
		vLook = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		break;
	case D3DCUBEMAP_FACE_NEGATIVE_Z:
		vLook = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		break;
	}

	D3DXMatrixLookAtLH(pViewMatrix, &vEye, &vLook, &vUp);
}
