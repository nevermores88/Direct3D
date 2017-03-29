#include "stdafx.h"
#include "Shader_3dapi_02_18.h"


CShader_3dapi_02_18::CShader_3dapi_02_18()
{
}


CShader_3dapi_02_18::~CShader_3dapi_02_18()
{
}

HRESULT CShader_3dapi_02_18::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pShader = LoadPixelShader("Ex02_18/Shader.fx", &m_pConstTbl);
	if (!m_pShader)
		return E_FAIL;

	/*DWORD dwFlags = 0;
#if defined( _DEBUG ) || defined( DEBUG )
	dwFlags |= D3DXSHADER_DEBUG;
#endif

	HRESULT	hr = 0;

	LPD3DXBUFFER	pShader = NULL;
	LPD3DXBUFFER	pErr = NULL;

	hr = D3DXCompileShaderFromFile("Ex02_18/Shader.fx"
		, NULL
		, NULL
		, "PxlPrc"
		, "ps_2_0"
		, dwFlags
		, &pShader
		, &pErr
		, &m_pConstTbl
		);

	if (FAILED(hr))
	{
		char sErr[2048] = { 0 };
		if (pErr)
		{
			char* s = (char*)pErr->GetBufferPointer();
			sprintf(sErr, s);
		}
		else
		{
			sprintf(sErr, "Cannot Compile Shader.");
		}

		MessageBox(g_pGame->GetHWND(), sErr, "Err", MB_ICONEXCLAMATION);
		return -1;
	}

	hr = m_pdev->CreatePixelShader((DWORD*)pShader->GetBufferPointer(), &m_pShader);
	pShader->Release();
	if (FAILED(hr))
		return -1;*/

	m_pTex = LoadTexture("Ex02_18/earth.bmp");
	if (!m_pTex)
		return E_FAIL;

	m_pVertices[0] = Vertex(-1.05F, 1.02F, 0, D3DXCOLOR(1, 0, 0, 1), 0, 0);
	m_pVertices[1] = Vertex(1.05F, 1.02F, 0, D3DXCOLOR(0, 1, 0, 1), 1, 0);
	m_pVertices[2] = Vertex(1.05F, -1.02F, 0, D3DXCOLOR(0, 0, 1, 1), 1, 1);
	m_pVertices[3] = Vertex(-1.05F, -1.02F, 0, D3DXCOLOR(1, 0, 1, 1), 0, 1);

	for (int i = 0; i < 4; i++)
	{
		m_pVertices[i].x *= 100;
		m_pVertices[i].y *= 100;
	}

	return S_OK;
}

void CShader_3dapi_02_18::Release()
{

}

void CShader_3dapi_02_18::Render()
{
	if (m_pdev)
	{
		//m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pdev->SetPixelShader(m_pShader);

		m_pdev->SetFVF(Vertex::FVF);
		m_pdev->SetTexture(0, m_pTex);
		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_pVertices, sizeof(Vertex));

		m_pdev->SetPixelShader(NULL);
		m_pdev->SetTexture(0, NULL);
	}
}

void CShader_3dapi_02_18::Update()
{

}
