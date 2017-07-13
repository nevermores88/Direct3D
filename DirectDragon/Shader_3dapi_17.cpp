#include "stdafx.h"
#include "Shader_3dapi_17.h"


CShader_3dapi_17::CShader_3dapi_17()
{
}


CShader_3dapi_17::~CShader_3dapi_17()
{
}

void CShader_3dapi_17::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;

	HRESULT hr = 0;

	LPD3DXBUFFER	pShader = NULL;
	LPD3DXBUFFER	pError = NULL;

	hr = D3DXCompileShaderFromFile(
		L"Shader_3dapi_17/shader.fx",
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

	m_Vertex[0] = Vertex(-1.05f, 1.02f, 0, D3DXCOLOR(1, 0, 0, 1));
	m_Vertex[1] = Vertex(1.05f, 1.02f, 0, D3DXCOLOR(0, 1, 0, 1));
	m_Vertex[2] = Vertex(1.05f, -1.02f, 0, D3DXCOLOR(0, 0, 1, 1));
	m_Vertex[3] = Vertex(-1.05f, -1.02f, 0, D3DXCOLOR(1, 0, 1, 1));

	m_hdType = m_pConstTable->GetConstantByName(NULL, "ProcType");
}

void CShader_3dapi_17::Release()
{

}

void CShader_3dapi_17::OnRender()
{
	if (m_pdev)
	{
		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);

		m_pdev->SetPixelShader(m_pShader);

		m_pConstTable->SetInt(m_pdev, m_hdType, m_nType);

		m_pdev->SetFVF(Vertex::FVF);
		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_Vertex, sizeof(Vertex));

		m_pdev->SetPixelShader(NULL);
	}
}

void CShader_3dapi_17::OnUpdate()
{
	if (m_pdev)
	{
		m_nType = 2;
	}
}
