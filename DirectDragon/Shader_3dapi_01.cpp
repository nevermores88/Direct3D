#include "stdafx.h"
#include "Shader_3dapi_01.h"


CShader_3dapi_01::CShader_3dapi_01()
{
}


CShader_3dapi_01::~CShader_3dapi_01()
{
}

void CShader_3dapi_01::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;

	HRESULT hr = 0;

	LPD3DXBUFFER pShader = NULL;
	LPD3DXBUFFER pError = NULL;

	hr = D3DXCompileShaderFromFile(
		L"Shader_3dapi_01/shader.txt",
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

	//m_hdViewProjMatrix = m_pConstTable->GetConstantByName(0, "ViewProjMatrix");

	pShader->Release();

	D3DVERTEXELEMENT9 vertexDecl[MAX_FVF_DECL_SIZE] = { 0 };
	D3DXDeclaratorFromFVF(Vertex::FVF, vertexDecl);

	if (FAILED(m_pdev->CreateVertexDeclaration(vertexDecl, &m_pFVF)))
		return;

	m_Vertex[0] = Vertex(-1, -1, 0);
	m_Vertex[1] = Vertex(0, 1, 0);
	m_Vertex[2] = Vertex(1, -1, 0);
}

void CShader_3dapi_01::Release()
{

}

void CShader_3dapi_01::OnRender()
{
	if (m_pdev)
	{
		D3DXMATRIX view = g_pGame->GetViewMatrix();
		D3DXMATRIX viewProj = view * g_pGame->GetProjMatrix();

		//m_pConstTable->SetMatrix(m_pdev, m_hdViewProjMatrix, &viewProj);

		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);

		m_pdev->SetVertexShader(m_pShader);
		m_pdev->SetVertexDeclaration(m_pFVF);

		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, m_Vertex, sizeof(Vertex));

		m_pdev->SetVertexShader(NULL);
		m_pdev->SetVertexDeclaration(NULL);
	}
}

void CShader_3dapi_01::OnUpdate()
{

}
