#include "stdafx.h"
#include "Shader_3dapi_01_20.h"


CShader_3dapi_01_20::CShader_3dapi_01_20()
{
}


CShader_3dapi_01_20::~CShader_3dapi_01_20()
{
}

HRESULT CShader_3dapi_01_20::Create(LPDIRECT3DDEVICE9 pdev)
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

	hr = D3DXAssembleShaderFromFile("Ex01_20/Shader.vsh", NULL, NULL, dwFlags, &pShader, &pError);

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

	D3DXCreateTextureFromFile(m_pdev, "Ex01_20/tiger.png", &m_pTex);

	for (int i = 0; i < 21; ++i)
	{
		m_pVertices[2 * i + 0].p = D3DXVECTOR3(i - 10.F, -7, 0) * 5;
		m_pVertices[2 * i + 0].u = i / 20.f;
		m_pVertices[2 * i + 0].v = 1;
		m_pVertices[2 * i + 0].color = 0xFFFFFFFF;

		m_pVertices[2 * i + 1].p = D3DXVECTOR3(i - 10.F, 7, 0) * 5;
		m_pVertices[2 * i + 1].u = i / 20.f;
		m_pVertices[2 * i + 1].v = 0;
		m_pVertices[2 * i + 1].color = 0xFFFFFFFF;

		m_pVertices[2 * i + 0].g[0] = 0.25F;		// 비중0
		m_pVertices[2 * i + 0].g[1] = 0.25F;		// 비중1
		m_pVertices[2 * i + 0].g[2] = 0.25F;		// 비중2

		m_pVertices[2 * i + 0].m[0] = i + 0;			// 행렬 인덱스0
		m_pVertices[2 * i + 0].m[1] = i + 1;			// 행렬 인덱스1
		m_pVertices[2 * i + 0].m[2] = i + 2;			// 행렬 인덱스2
		m_pVertices[2 * i + 0].m[3] = i + 3;			// 행렬 인덱스3

		m_pVertices[2 * i + 1].g[0] = 0.25F;		// 비중0
		m_pVertices[2 * i + 1].g[1] = 0.25F;		// 비중1
		m_pVertices[2 * i + 1].g[2] = 0.25F;		// 비중2

		m_pVertices[2 * i + 1].m[0] = i + 0;			// 행렬 인데스0
		m_pVertices[2 * i + 1].m[1] = i + 1;			// 행렬 인데스1
		m_pVertices[2 * i + 1].m[2] = i + 2;			// 행렬 인데스2
		m_pVertices[2 * i + 1].m[3] = i + 3;			// 행렬 인데스3
	}

	for (int i = 0; i < 40; ++i)
		D3DXMatrixIdentity(&m_mtWorld[i]);

	return S_OK;
}

void CShader_3dapi_01_20::Release()
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

void CShader_3dapi_01_20::Render()
{
	if (m_pdev)
	{
		D3DXMATRIX mtView;
		D3DXMATRIX mtProj;

		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);

		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		//m_pdev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		m_pdev->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE);	//enables indexed vertex blending
		//m_pdev->SetSoftwareVertexProcessing(TRUE);
		
		D3DXMatrixTranspose(&mtView, &mtView);
		D3DXMatrixTranspose(&mtProj, &mtProj);

		for (int i = 0; i < 40; i++)
		{
			D3DXMatrixTranspose(&m_mtWorld[i], &m_mtWorld[i]);
		}

		m_pdev->SetVertexShader(m_pVertexShader);
		m_pdev->SetVertexDeclaration(m_pFVF);

		m_pdev->SetVertexShaderConstantF(4, (FLOAT*)&mtView, 4);
		m_pdev->SetVertexShaderConstantF(8, (FLOAT*)&mtProj, 4);

		for (int i = 0; i < 40; i++)
		{
			m_pdev->SetVertexShaderConstantF(12 + i*4, (FLOAT*)&m_mtWorld[i], 4);
		}
		
		m_pdev->SetTexture(0, m_pTex);
		//m_pdev->SetFVF(Vertex::FVF);
		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 40, m_pVertices, sizeof(Vertex));

		m_pdev->SetVertexShader(NULL);
		m_pdev->SetVertexDeclaration(NULL);
		m_pdev->SetTexture(0, NULL);

		m_pdev->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		//m_pdev->SetSoftwareVertexProcessing(FALSE);
	}
}

void CShader_3dapi_01_20::Update()
{
	if (m_pdev)
	{
		static float fAngle = 0;
		fAngle += 1.0f;

		for (int i = 1 /*0*/; i < 40; i++)
		{
			D3DXMatrixIdentity(&m_mtWorld[i]);
			float fTheta = fAngle + i * (360.0f / 15.0f);
			m_mtWorld[i]._41 = cosf(D3DXToRadian(fTheta))*1.5f;
			m_mtWorld[i]._42 = sinf(D3DXToRadian(fTheta))*2.0f;
		}
	}
}
