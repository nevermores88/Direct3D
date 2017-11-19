#include "stdafx.h"
#include "Shader_3dapi_03_41.h"
#include "Mesh.h"

CShader_3dapi_03_41::CShader_3dapi_03_41()
{
}


CShader_3dapi_03_41::~CShader_3dapi_03_41()
{
}

HRESULT CShader_3dapi_03_41::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_iEnvOpt = 0;

	HRESULT hr = 0;

	m_pShader = LoadShader("Ex03_41/Shader.fx");
	if (!m_pShader)
		return E_FAIL;

	DWORD dwFVF = Vertex::FVF;
	D3DVERTEXELEMENT9	vertex_decl[MAX_FVF_DECL_SIZE];
	D3DXDeclaratorFromFVF(dwFVF, vertex_decl);
	if (FAILED(m_pdev->CreateVertexDeclaration(vertex_decl, &m_pFVF)))
		return E_FAIL;

	m_pMesh = new CMesh;
	m_pMesh->Create(m_pdev, "Ex03_41/teapot.x");

	m_pSkyBox = new CMesh;
	m_pSkyBox->Create(m_pdev, "Ex03_41/lobby_skybox.x", "Ex03_41");

	////
	LPDIRECT3DSURFACE9 pSurface;
	D3DSURFACE_DESC	dscC;
	D3DSURFACE_DESC dscD;

	hr = m_pdev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pSurface);

	if (FAILED(hr))
		return E_FAIL;

	pSurface->GetDesc(&dscC);
	pSurface->Release();

	hr = m_pdev->GetDepthStencilSurface(&pSurface);

	if (FAILED(hr))
		return E_FAIL;

	pSurface->GetDesc(&dscD);
	pSurface->Release();

	const	int	ENVMAP_RESOLUTION = 256;

	// Create Rendering Environment Mapping Object
	hr = D3DXCreateRenderToEnvMap(m_pdev, ENVMAP_RESOLUTION, 1, dscC.Format, TRUE, dscD.Format, &m_pRenderEnvMap);
	if (FAILED(hr))
		return E_FAIL;

	// Create Cube Map
	hr = D3DXCreateCubeTexture(m_pdev, ENVMAP_RESOLUTION, 1, D3DUSAGE_RENDERTARGET, dscC.Format, D3DPOOL_DEFAULT, &m_pCubeTex);
	if (FAILED(hr))
	{
		hr = D3DXCreateCubeTexture(m_pdev, ENVMAP_RESOLUTION, 1, 0, dscC.Format, D3DPOOL_DEFAULT, &m_pCubeTex);
		if (FAILED(hr))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

void CShader_3dapi_03_41::Release()
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

	if (m_pCubeTex)
	{
		m_pCubeTex->Release();
		m_pCubeTex = NULL;
	}

	if (m_pRenderEnvMap)
	{
		m_pRenderEnvMap->Release();
		m_pRenderEnvMap = NULL;
	}
}

void CShader_3dapi_03_41::Render()
{
	if (m_pdev)
	{
		RenderScene();

		D3DXMATRIX mtWorld = m_mtWorld;
		D3DXMATRIX mtView;
		D3DXMATRIX mtProj;
		D3DXMATRIX mtViewI;
		D3DXVECTOR4 vCam;

		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);

		D3DXMatrixInverse(&mtViewI, NULL, &mtView);
		vCam = D3DXVECTOR4(mtViewI._41, mtViewI._42, mtViewI._43, 0);

		m_pdev->SetVertexDeclaration(m_pFVF);

		m_pShader->SetMatrix("g_mtWorld", &mtWorld);
		m_pShader->SetMatrix("g_mtView", &mtView);
		m_pShader->SetMatrix("g_mtProj", &mtProj);
		m_pShader->SetVector("g_vCam", &vCam);
		m_pShader->SetInt("g_iEnvOpt", m_iEnvOpt);

		m_pShader->SetTexture("g_CubeTex", m_pCubeTex);

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

void CShader_3dapi_03_41::Update()
{
	if (m_pdev)
	{
		/*Render CubeMap*/
		if (GetAsyncKeyState('1') & 0X8000)
			m_iEnvOpt = 1;
		if (GetAsyncKeyState('2') & 0X8000)
			m_iEnvOpt = 0;

		D3DXMATRIX mtCurView;
		D3DXMATRIX mtCurProj;

		m_pdev->GetTransform(D3DTS_VIEW, &mtCurView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtCurProj);

		D3DXMATRIX mtView[6];
		D3DXMATRIX mtProj;

		D3DXMatrixPerspectiveFovLH(&mtProj, D3DX_PI / 2.0f, 1.0f, 1.0f, 5000.0f);

		// Set the view transform for this cubemap surface
		for (int i = 0; i < 6; i++)
		{
			SetupCubeViewMatrix(&mtView[i], (D3DCUBEMAP_FACES)i);
			mtView[i] = mtCurView * mtView[i];
		}

		// Rendering to cubemap surface
		m_pRenderEnvMap->BeginCube(m_pCubeTex);
		{
			for (int i = 0; i < 6; i++)
			{
				m_pRenderEnvMap->Face((D3DCUBEMAP_FACES)i, 0);
				RenderScene(&mtView[i], &mtProj);
			}
		}
		m_pRenderEnvMap->End(0);

		m_pdev->SetTransform(D3DTS_VIEW, &mtCurView);
		m_pdev->SetTransform(D3DTS_PROJECTION, &mtCurProj);



		/*Teapot Update*/
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

void CShader_3dapi_03_41::SetupCubeViewMatrix(D3DXMATRIX* pOut, DWORD dwFace)
{
	D3DXVECTOR3 vEye = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vLookDir;
	D3DXVECTOR3 vUpDir;

	switch (dwFace)
	{
	case D3DCUBEMAP_FACE_POSITIVE_X:
		vLookDir = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		vUpDir = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		break;
	case D3DCUBEMAP_FACE_NEGATIVE_X:
		vLookDir = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		vUpDir = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		break;
	case D3DCUBEMAP_FACE_POSITIVE_Y:
		vLookDir = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vUpDir = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		break;
	case D3DCUBEMAP_FACE_NEGATIVE_Y:
		vLookDir = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		vUpDir = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		break;
	case D3DCUBEMAP_FACE_POSITIVE_Z:
		vLookDir = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		vUpDir = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		break;
	case D3DCUBEMAP_FACE_NEGATIVE_Z:
		vLookDir = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vUpDir = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		break;
	}

	D3DXMatrixLookAtLH(pOut, &vEye, &vLookDir, &vUpDir);
}

void CShader_3dapi_03_41::RenderScene()
{
	m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	D3DXMATRIX mtWorld;
	D3DXMatrixScaling(&mtWorld, 100.0f, 100.0f, 100.0f);

	m_pdev->SetTransform(D3DTS_WORLD, &mtWorld);
	m_pSkyBox->Render();

	D3DXMatrixIdentity(&mtWorld);
	m_pdev->SetTransform(D3DTS_WORLD, &mtWorld);
}

void CShader_3dapi_03_41::RenderScene(D3DXMATRIX* pView, D3DXMATRIX* pProj)
{
	m_pdev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFF006699, 1.0f, 0);
	m_pdev->SetTransform(D3DTS_VIEW, pView);
	m_pdev->SetTransform(D3DTS_PROJECTION, pProj);

	RenderScene();
}
