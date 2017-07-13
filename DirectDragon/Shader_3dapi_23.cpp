#include "stdafx.h"
#include "Shader_3dapi_23.h"


CShader_3dapi_23::CShader_3dapi_23()
{
}


CShader_3dapi_23::~CShader_3dapi_23()
{
}

void CShader_3dapi_23::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;

	HRESULT hr = 0;

	LPD3DXBUFFER	pShader = NULL;
	LPD3DXBUFFER	pError = NULL;

	hr = D3DXCompileShaderFromFile(
		L"Shader_3dapi_23/shader.fx",
		0, 0,
		"Main",
		"ps_2_0",
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

	hr = m_pdev->CreatePixelShader(
		(DWORD*)pShader->GetBufferPointer(),
		&m_pShader);

	pShader->Release();

	D3DXCreateTextureFromFile(m_pdev, L"Shader_3dapi_23/earth.bmp", &m_pTex0);
	D3DXCreateTextureFromFile(m_pdev, L"Shader_3dapi_23/dx5_logo.bmp", &m_pTex1);
	D3DXCreateTextureFromFile(m_pdev, L"Shader_3dapi_23/env0.png", &m_pTex2);

	m_pVertex[0] = Vertex(-1.05F, 1.02F, 0, 0, 0);
	m_pVertex[1] = Vertex(1.05F, 1.02F, 0, 1, 0);
	m_pVertex[2] = Vertex(1.05F, -1.02F, 0, 1, 1);
	m_pVertex[3] = Vertex(-1.05F, -1.02F, 0, 0, 1);

}

void CShader_3dapi_23::Release()
{

}

void CShader_3dapi_23::OnRender()
{
	if (m_pdev)
	{
		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);

		/*m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);*/

		m_pdev->SetPixelShader(m_pShader);

		m_pdev->SetFVF(Vertex::FVF);
		
		m_pdev->SetTexture(0, m_pTex0);
		m_pdev->SetTexture(1, m_pTex1);
		m_pdev->SetTexture(2, m_pTex2);


		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_pVertex, sizeof(Vertex));

		m_pdev->SetPixelShader(NULL);
		//m_pdev->SetVertexDeclaration(NULL);
	}
}

void CShader_3dapi_23::OnUpdate()
{

}
