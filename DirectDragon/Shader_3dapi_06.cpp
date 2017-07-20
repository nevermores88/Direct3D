#include "stdafx.h"
#include "Shader_3dapi_06.h"


CShader_3dapi_06::CShader_3dapi_06()
{
}


CShader_3dapi_06::~CShader_3dapi_06()
{
}

void CShader_3dapi_06::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;

	HRESULT hr = 0;

	LPD3DXBUFFER	pShader = NULL;
	LPD3DXBUFFER	pError = NULL;

	hr = D3DXCompileShaderFromFile(
		L"Shader_3dapi_06/shader.fx",
		0, 0,
		"Main",
		"vs_1_1",
		D3DXSHADER_DEBUG,
		&pShader,
		&pError,
		&m_pConstTable);

	if (pError)
	{
		wchar_t errorMsg[1024];
		MultiByteToWideChar(CP_ACP, MB_COMPOSITE, (LPCCH)pError->GetBufferPointer(), -1, errorMsg, 1024);
		::MessageBox(0, errorMsg, 0, 0);
	}

	if (FAILED(hr))
	{
		::MessageBox(0, L"D3DXCompileShaderFromFile() - FAILED", 0, 0);
		return;
	}

	hr = m_pdev->CreateVertexShader(
		(DWORD*)pShader->GetBufferPointer(),
		&m_pShader);

	pShader->Release();

	D3DVERTEXELEMENT9	vertexDecl[MAX_FVF_DECL_SIZE] = { 0 };
	D3DXDeclaratorFromFVF(Vertex::FVF, vertexDecl);

	if (FAILED(m_pdev->CreateVertexDeclaration(vertexDecl, &m_pFVF)))
		return;

	m_Vertex[0] = Vertex(-1, -1, 0, D3DXCOLOR(1, 0, 0, 1));
	m_Vertex[1] = Vertex(0, 1, 0, D3DXCOLOR(0, 1, 0, 1));
	m_Vertex[2] = Vertex(1, -1, 0, D3DXCOLOR(0, 0, 1, 1));

	
	m_hdDiffuse = m_pConstTable->GetConstantByName(NULL, "diffuse");
	
}

void CShader_3dapi_06::Release()
{

}

void CShader_3dapi_06::OnRender()
{
	if (m_pdev)
	{
		m_pdev->SetVertexShader(m_pShader);
		m_pdev->SetVertexDeclaration(m_pFVF);

		D3DXCOLOR color(0, 1, 1, 1);
		m_pConstTable->SetVector(m_pdev, m_hdDiffuse, (D3DXVECTOR4*)&color);

		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, m_Vertex, sizeof(Vertex));

		m_pdev->SetVertexShader(NULL);
	}
}

void CShader_3dapi_06::OnUpdate()
{

}