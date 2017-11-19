#include "stdafx.h"
#include "Shader_3dapi_03_42.h"
#include "Mesh.h"

CShader_3dapi_03_42::CShader_3dapi_03_42()
{
}


CShader_3dapi_03_42::~CShader_3dapi_03_42()
{
}

HRESULT CShader_3dapi_03_42::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_iEnvOpt = 0;

	HRESULT hr = 0;

	m_pShader = LoadShader("Ex03_42/Shader.fx");
	if (!m_pShader)
		return E_FAIL;

	DWORD dwFVF = Vertex::FVF;
	D3DVERTEXELEMENT9 vertex_decl[MAX_FVF_DECL_SIZE];
	D3DXDeclaratorFromFVF(dwFVF, vertex_decl);
	if (FAILED(m_pdev->CreateVertexDeclaration(vertex_decl, &m_pFVF)))
		return E_FAIL;

	m_pMesh = new CMesh;
	m_pMesh->Create(m_pdev, "Ex03_42/teapot.x");

	m_pSkyBox = new CMesh;
	m_pSkyBox->Create(m_pdev, "Ex03_42/lobby_skybox.x", "Ex03_41");

	////
	LPDIRECT3DSURFACE9 pSurface;
	D3DSURFACE_DESC dscC;
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

	hr = D3DXCreateRenderToEnvMap(m_pdev, ENVMAP_RESOLUTION, 1, dscC.Format, TRUE, dscD.Format, &m_pRenderEnvMap);

}

void CShader_3dapi_03_42::Release()
{

}

void CShader_3dapi_03_42::Render()
{

}

void CShader_3dapi_03_42::Update()
{

}

void CShader_3dapi_03_42::SetupCubeViewMatrix(D3DXMATRIX* pOut, DWORD dwFace)
{

}

void CShader_3dapi_03_42::RenderScene()
{

}

void CShader_3dapi_03_42::RenderScene(D3DXMATRIX* pView, D3DXMATRIX* pProj)
{

}
