#include "stdafx.h"
#include "Shader_3dapi_27.h"


CShader_3dapi_27::CShader_3dapi_27()
{
}


CShader_3dapi_27::~CShader_3dapi_27()
{
}

void CShader_3dapi_27::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;

	HRESULT hr = 0;

	LPD3DXBUFFER	pShader = NULL;
	LPD3DXBUFFER	pError = NULL;

	hr = D3DXCompileShaderFromFile(
		L"Shader_3dapi_27/shader.fx",
		0, 0,
		"Main",
		"tx_1_0",
		D3DXSHADER_DEBUG,
		&pShader,
		&pError,
		NULL);

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

	// 절차적 텍스처(procedural texture) 생성
	D3DXCreateTexture(m_pdev, 128, 128, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, &m_pTex);

	LPD3DXTEXTURESHADER pTs = NULL;
	D3DXCreateTextureShader((DWORD*)pShader->GetBufferPointer(), &pTs);
	pShader->Release();

	D3DXFillTextureTX(m_pTex, pTs);

	pTs->Release();

	m_pVertex[0] = Vertex(-1.05F, 1.02F, 0, 0, 0);//, D3DXCOLOR(0, 0, 0, 0));
	m_pVertex[1] = Vertex(1.05F, 1.02F, 0, 1, 0);//, D3DXCOLOR(0, 0, 0, 0));
	m_pVertex[2] = Vertex(1.05F, -1.02F, 0, 1, 1);//, D3DXCOLOR(0, 0, 0, 0));
	m_pVertex[3] = Vertex(-1.05F, -1.02F, 0, 0, 1);//, D3DXCOLOR(0, 0, 0, 0));
}

void CShader_3dapi_27::Release()
{

}

void CShader_3dapi_27::OnRender()
{
	if (m_pdev)
	{
		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);

		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);

		m_pdev->SetTexture(0, m_pTex);
		m_pdev->SetFVF(Vertex::FVF);
		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_pVertex, sizeof(Vertex));
	}
}

void CShader_3dapi_27::OnUpdate()
{

}
