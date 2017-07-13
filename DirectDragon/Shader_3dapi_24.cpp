#include "stdafx.h"
#include "Shader_3dapi_24.h"


CShader_3dapi_24::CShader_3dapi_24()
{
}


CShader_3dapi_24::~CShader_3dapi_24()
{
}

void CShader_3dapi_24::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;

	HRESULT hr = 0;

	LPD3DXBUFFER	pShader = NULL;
	LPD3DXBUFFER	pError = NULL;

	hr = D3DXCompileShaderFromFile(
		L"Shader_3dapi_24/shader.fx",
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
}

void CShader_3dapi_24::Release()
{

}

void CShader_3dapi_24::OnRender()
{
	if (m_pdev)
	{
		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);

		m_pdev->SetPixelShader(m_pShader);

		m_pdev->SetFVF(Vertex::FVF);

		m_pdev->SetTexture(0, m_pTex0);
		m_pdev->SetTexture(1, m_pTex1);

		Vertex pVtx[4];

		for (int i = 0; i <= 13; ++i)
		{
			float	x = -1.f + float(i % 4) / 2.f;
			float	y = 1.f - float(i / 4) / 2.f;

			pVtx[0] = Vertex(x + 0.f, y - 0.f, 0, 0, 0);
			pVtx[1] = Vertex(x + .5f, y - 0.f, 0, 1, 0);
			pVtx[2] = Vertex(x + .5f, y - .5f, 0, 1, 1);
			pVtx[3] = Vertex(x + 0.f, y - .5f, 0, 0, 1);

			m_pConstTable->SetInt(m_pdev, "m_nMulti", i);
			m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, pVtx, sizeof(Vertex));
		}

		m_pdev->SetPixelShader(NULL);
		//m_pdev->SetVertexDeclaration(NULL);
	}
}

void CShader_3dapi_24::OnUpdate()
{

}
