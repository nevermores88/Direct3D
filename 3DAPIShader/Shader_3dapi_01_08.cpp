#include "stdafx.h"
#include "Shader_3dapi_01_08.h"


CShader_3dapi_01_08::CShader_3dapi_01_08()
{
}


CShader_3dapi_01_08::~CShader_3dapi_01_08()
{
}

HRESULT CShader_3dapi_01_08::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	HRESULT hr = 0;
	HWND hwnd;
	D3DDEVICE_CREATION_PARAMETERS ppm;

	m_pdev->GetCreationParameters(&ppm);
	hwnd = ppm.hFocusWindow;

	m_pVertices[0] = Vertex(-80, 60, 0, D3DXCOLOR(1, 1, 1, 1), 0, 0);
	m_pVertices[1] = Vertex(80, 60, 0, D3DXCOLOR(1, 1, 1, 1), 1, 0);
	m_pVertices[2] = Vertex(80, -60, 0, D3DXCOLOR(1, 1, 1, 1), 1, 1);
	m_pVertices[3] = Vertex(-80, -60, 0, D3DXCOLOR(1, 1, 1, 1), 0, 1);

	DWORD dwFlags = 0;

#if defined(_DEBUG) || defined(DEBUG)
	dwFlags |= D3DXSHADER_DEBUG;
#endif

	LPD3DXBUFFER pShader = NULL;
	LPD3DXBUFFER pError = NULL;

	hr = D3DXAssembleShaderFromFile("Ex01_08/Shader.vsh", NULL, NULL, dwFlags, &pShader, &pError);

	if (FAILED(hr))
	{
		int iSize = pError->GetBufferSize();
		void* ack = pError->GetBufferPointer();

		if (ack)
		{
			char* str = new char[iSize];
			sprintf(str, (const char*)ack, iSize);
			OutputDebugString(str);
			delete[] str;
		}
	}

	if (pShader)
	{
		hr = m_pdev->CreateVertexShader((DWORD*)pShader->GetBufferPointer(), &m_pVertexShader);
		pShader->Release();
	}

	if (FAILED(hr))
		return E_FAIL;

	D3DVERTEXELEMENT9 vertex_decl[MAX_FVF_DECL_SIZE] = { 0 };
	D3DXDeclaratorFromFVF(Vertex::FVF, vertex_decl);
	if (FAILED(m_pdev->CreateVertexDeclaration(vertex_decl, &m_pFVF)))
		return E_FAIL;

	D3DXCreateTextureFromFile(m_pdev, "Ex01_08/earth.bmp", &m_pTex);

	return S_OK;
}

void CShader_3dapi_01_08::Release()
{
	if (m_pVertexShader)
	{
		m_pVertexShader->Release();
		m_pVertexShader = NULL;
	}

	if (m_pFVF)
	{
		m_pFVF->Release();
		m_pFVF = NULL;
	}

	if (m_pTex)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
}

void CShader_3dapi_01_08::Render()
{
	if (m_pdev)
	{
		D3DXMATRIX mtWorld;
		D3DXMATRIX mtView;
		D3DXMATRIX mtProj;

		D3DXMatrixIdentity(&mtWorld);

		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);

		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		D3DXMATRIX mtWVP = mtWorld*mtView*mtProj;
		D3DXMatrixTranspose(&mtWVP, &mtWVP);

		m_pdev->SetVertexShader(m_pVertexShader);
		m_pdev->SetVertexDeclaration(m_pFVF);
		
		m_pdev->SetVertexShaderConstantF(0, (FLOAT*)&mtWVP, 4);

		m_pdev->SetTexture(0, m_pTex);
		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_pVertices, sizeof(Vertex));

		m_pdev->SetVertexShader(NULL);
		m_pdev->SetTexture(0, NULL);
	}
}

void CShader_3dapi_01_08::Update()
{
	if (m_pdev)
	{

	}
}
