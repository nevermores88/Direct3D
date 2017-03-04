#include "stdafx.h"
#include "Shader_3dapi_03_12.h"


CShader_3dapi_03_12::CShader_3dapi_03_12()
{
}


CShader_3dapi_03_12::~CShader_3dapi_03_12()
{
}

HRESULT CShader_3dapi_03_12::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pShader = LoadShader("Ex03_12/Shader.fx");
	if (!m_pShader)
		return E_FAIL;

	DWORD dwFVF = Vertex::FVF;
	D3DVERTEXELEMENT9 vertex_decl[MAX_FVF_DECL_SIZE];
	D3DXDeclaratorFromFVF(dwFVF, vertex_decl);
	if (FAILED(m_pdev->CreateVertexDeclaration(vertex_decl, &m_pFVF)))
		return E_FAIL;

	return S_OK;
}

void CShader_3dapi_03_12::Release()
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
}

void CShader_3dapi_03_12::Render()
{
	if (m_pdev)
	{

	}
}

void CShader_3dapi_03_12::Update()
{
	if (m_pdev)
	{

	}
}
