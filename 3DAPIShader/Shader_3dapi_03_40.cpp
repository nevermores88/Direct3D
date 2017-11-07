#include "stdafx.h"
#include "Shader_3dapi_03_40.h"
#include "Mesh.h"

CShader_3dapi_03_40::CShader_3dapi_03_40()
{
}


CShader_3dapi_03_40::~CShader_3dapi_03_40()
{
}

HRESULT CShader_3dapi_03_40::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	HRESULT hr;

	m_pShader = LoadShader("Ex03_40/Shader.fx");
	if (!m_pShader)
		return E_FAIL;

	DWORD dwFVF = Vertex::FVF;
	D3DVERTEXELEMENT9	vertex_decl[MAX_FVF_DECL_SIZE];
	D3DXDeclaratorFromFVF(dwFVF, vertex_decl);
	if (FAILED(m_pdev->CreateVertexDeclaration(vertex_decl, &m_pFVF)))
		return E_FAIL;

	m_pMesh = new CMesh;
	m_pMesh->Create(m_pdev, "Ex03_40/teapot.x");

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

void CShader_3dapi_03_40::Release()
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

void CShader_3dapi_03_40::Render()
{
	if (m_pdev)
	{
		D3DXMATRIX mtWorld = m_mtWorld;
		D3DXMATRIX mtView;
		D3DXMATRIX mtProj;

		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);

		m_pdev->SetVertexDeclaration(m_pFVF);

		m_pShader->SetMatrix("g_mtWorld", &mtWorld);
		m_pShader->SetMatrix("g_mtView", &mtView);
		m_pShader->SetMatrix("g_mtProj", &mtProj);

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

void CShader_3dapi_03_40::Update()
{
	if (m_pdev)
	{
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
