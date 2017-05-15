#include "stdafx.h"
#include "Shader_3dapi_02_26.h"


CShader_3dapi_02_26::CShader_3dapi_02_26()
{
}


CShader_3dapi_02_26::~CShader_3dapi_02_26()
{
}

HRESULT CShader_3dapi_02_26::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	//Compile Shader
	DWORD dwShaderFlags = 0;

#if _DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

	HRESULT	hr = 0;

	LPD3DXBUFFER	pShader = NULL;
	LPD3DXBUFFER	pErr = NULL;

	hr = D3DXCompileShaderFromFile("Ex02_26/Shader.fx"
		, NULL
		, NULL
		, "TxlPrc"
		, "tx_1_0"
		, dwShaderFlags
		, &pShader
		, &pErr
		, NULL
		);

	if (FAILED(hr) && pErr)
	{
		int size = pErr->GetBufferSize();
		void* ack = pErr->GetBufferPointer();

		if (ack)
		{
			char* str = new char[size];
			sprintf(str, (const char*)ack, size);
			OutputDebugString(str);
			delete[] str;
		}
	}

	//Create Precedural Texture
	if (FAILED(D3DXCreateTexture(m_pdev, 128, 128, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, &m_pTex)))
		return E_FAIL;

	// 텍스처 픽셀 채우기
	//if( FAILED(D3DXFillTextureTX(m_pTx, (DWORD*)pShd->GetBufferPointer(), NULL, 0)))
	//	return -1;

	//pShd->Release();

	LPD3DXTEXTURESHADER pTexShader = NULL;
	D3DXCreateTextureShader((DWORD*)pShader->GetBufferPointer(), &pTexShader);
	pShader->Release();

	if (FAILED(D3DXFillTextureTX(m_pTex, pTexShader)))
		return E_FAIL;

	pTexShader->Release();

	// 정점 생성
	m_pVertices[0] = Vertex(-1.05F, 1.02F, 0, D3DXCOLOR(1, 1, 1, 1), 0, 0);
	m_pVertices[1] = Vertex(1.05F, 1.02F, 0, D3DXCOLOR(1, 1, 1, 1), 1, 0);
	m_pVertices[2] = Vertex(1.05F, -1.02F, 0, D3DXCOLOR(1, 1, 1, 1), 1, 1);
	m_pVertices[3] = Vertex(-1.05F, -1.02F, 0, D3DXCOLOR(1, 1, 1, 1), 0, 1);

	return S_OK;
}

void CShader_3dapi_02_26::Release()
{
	if (m_pTex)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
}

void CShader_3dapi_02_26::Render()
{
	if (m_pdev)
	{
		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);

		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		m_pdev->SetTexture(0, m_pTex);
		m_pdev->SetFVF(Vertex::FVF);
		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_pVertices, sizeof(Vertex));

		m_pdev->SetTexture(0, NULL);
	}
}

void CShader_3dapi_02_26::Update()
{
	if (m_pdev)
	{

	}
}
