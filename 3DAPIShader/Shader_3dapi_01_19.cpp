#include "stdafx.h"
#include "Shader_3dapi_01_19.h"


CShader_3dapi_01_19::CShader_3dapi_01_19()
{
}


CShader_3dapi_01_19::~CShader_3dapi_01_19()
{
}

HRESULT CShader_3dapi_01_19::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	HRESULT hr = 0;
	HWND hwnd;
	D3DDEVICE_CREATION_PARAMETERS ppm;

	m_pdev->GetCreationParameters(&ppm);
	hwnd = ppm.hFocusWindow;

	DWORD dwFlags = 0;

#if defined(_DEBUG) || defined(DEBUG)
	dwFlags |= D3DXSHADER_DEBUG;
#endif

	LPD3DXBUFFER pShader = NULL;
	LPD3DXBUFFER pError = NULL;

	hr = D3DXAssembleShaderFromFile("Ex01_19/Shader.vsh", NULL, NULL, dwFlags, &pShader, &pError);

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

	D3DXCreateTextureFromFile(m_pdev, "Ex01_19/dx5_logo.bmp", &m_pTex);

	for (int i = 0; i < 21; ++i)
	{
		m_pVertices[2 * i + 0].p = D3DXVECTOR3(i - 10.F, -6, 0) * 5;
		m_pVertices[2 * i + 0].u = i / 20.f;
		m_pVertices[2 * i + 0].v = 1;
		m_pVertices[2 * i + 0].color = 0xFFFFFFFF;

		m_pVertices[2 * i + 1].p = D3DXVECTOR3(i - 10.F, 6, 0) * 5;
		m_pVertices[2 * i + 1].u = i / 20.f;
		m_pVertices[2 * i + 1].v = 0;
		m_pVertices[2 * i + 1].color = 0xFFFFFFFF;

		FLOAT t = FLOAT(i) / 20.f;

		t *= D3DX_PI / 2.F;
		m_pVertices[2 * i + 0].g = 1.f - sinf(t);
		m_pVertices[2 * i + 1].g = 1.f - sinf(t);
	}

	return S_OK;
}

void CShader_3dapi_01_19::Release()
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

void CShader_3dapi_01_19::Render()
{
	if (m_pdev)
	{
		D3DXMATRIX mtView;
		D3DXMATRIX mtProj;

		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);

		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		//m_pdev->SetSoftwareVertexProcessing(TRUE);
		D3DXMATRIX mtWorldViewProj = m_mtWorld0 * mtView * mtProj;
		D3DXMatrixTranspose(&mtWorldViewProj, &mtWorldViewProj);
		D3DXMatrixTranspose(&mtView, &mtView);
		D3DXMatrixTranspose(&mtProj, &mtProj);
		D3DXMatrixTranspose(&m_mtWorld0, &m_mtWorld0);
		D3DXMatrixTranspose(&m_mtWorld1, &m_mtWorld1);

		m_pdev->SetVertexShader(m_pVertexShader);
		m_pdev->SetVertexDeclaration(m_pFVF);

		m_pdev->SetVertexShaderConstantF(20, (FLOAT*)&mtWorldViewProj, 4);
		m_pdev->SetVertexShaderConstantF(4, (FLOAT*)&mtView, 4);
		m_pdev->SetVertexShaderConstantF(8, (FLOAT*)&mtProj, 4);
		m_pdev->SetVertexShaderConstantF(12, (FLOAT*)&m_mtWorld0, 4);
		m_pdev->SetVertexShaderConstantF(16, (FLOAT*)&m_mtWorld1, 4);

		m_pdev->SetTexture(0, m_pTex);
		//m_pdev->SetFVF(Vertex::FVF);
		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 40, m_pVertices, sizeof(Vertex));

		m_pdev->SetVertexShader(NULL);
		m_pdev->SetVertexDeclaration(NULL);
		m_pdev->SetTexture(0, NULL);

		//m_pdev->SetSoftwareVertexProcessing(FALSE);
	}
}

void CShader_3dapi_01_19::Update()
{
	if (m_pdev)
	{
		D3DXMatrixIdentity(&m_mtWorld0);
		D3DXMatrixIdentity(&m_mtWorld1);

		m_fRandom += 0.005f;

		D3DXMATRIX mtX;
		D3DXMATRIX mtY;
		D3DXMATRIX mtZ;
		D3DXMatrixIdentity(&mtX);
		D3DXMatrixIdentity(&mtY);
		D3DXMatrixIdentity(&mtZ);

		float fRandomX = m_fRandom;
		float fRandomY = m_fRandom * 2.0f;
		float fRandomZ = m_fRandom * 3.0f;

		D3DXMatrixRotationX(&mtX, fRandomX);
		D3DXMatrixRotationY(&mtY, fRandomY);
		D3DXMatrixRotationZ(&mtZ, fRandomZ);

		m_mtWorld1 = mtZ * mtY * mtX;
	}
}
