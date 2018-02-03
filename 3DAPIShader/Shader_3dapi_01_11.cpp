#include "stdafx.h"
#include "Shader_3dapi_01_11.h"


CShader_3dapi_01_11::CShader_3dapi_01_11()
{
}


CShader_3dapi_01_11::~CShader_3dapi_01_11()
{
}

HRESULT CShader_3dapi_01_11::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	HRESULT hr = 0;
	HWND hwnd;
	D3DDEVICE_CREATION_PARAMETERS ppm;

	m_pdev->GetCreationParameters(&ppm);
	hwnd = ppm.hFocusWindow;

	LPD3DXMESH pMeshS = NULL;
	hr = D3DXLoadMeshFromX("Ex01_11/tiger.x", D3DXMESH_SYSTEMMEM, m_pdev, NULL, NULL, NULL, NULL, &pMeshS);
	if (FAILED(hr))
		return E_FAIL;

	LPD3DXMESH	pMeshD = NULL;
	hr = pMeshS->CloneMeshFVF(D3DXMESH_SYSTEMMEM, Vertex::FVF, m_pdev, &pMeshD);

	m_iVertexNum = pMeshD->GetNumVertices();
	m_iIndexNum = pMeshD->GetNumFaces();

	m_pVertices = new Vertex[m_iVertexNum];
	m_pIndices = new Index[m_iIndexNum];

	void* pVertices = NULL;
	hr = pMeshD->LockVertexBuffer(0, &pVertices);

	memcpy(m_pVertices, pVertices, m_iVertexNum * sizeof(Vertex));

	hr = pMeshD->UnlockVertexBuffer();

	void* pIndices = NULL;
	hr = pMeshD->LockIndexBuffer(0, &pIndices);

	memcpy(m_pIndices, pIndices, m_iIndexNum * sizeof(Index));

	hr = pMeshD->UnlockIndexBuffer();
	pMeshD->Release();

	float fScale = 15;
	for (int i = 0; i < m_iVertexNum; i++)
	{
		m_pVertices[i].p *= fScale;
	}

	D3DXCreateTextureFromFile(m_pdev, "Ex01_11/dx5_logo.bmp", &m_pTex);

	DWORD dwFlags = 0;

#if defined(_DEBUG) || defined(DEBUG)
	dwFlags |= D3DXSHADER_DEBUG;
#endif

	LPD3DXBUFFER pShader = NULL;
	LPD3DXBUFFER pError = NULL;

	hr = D3DXAssembleShaderFromFile("Ex01_11/Shader.vsh", NULL, NULL, dwFlags, &pShader, &pError);

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

	return S_OK;
}

void CShader_3dapi_01_11::Release()
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

	if (m_pVertices)
	{
		delete[] m_pVertices;
		m_pVertices = NULL;
	}

	if (m_pIndices)
	{
		delete[] m_pIndices;
		m_pIndices = NULL;
	}

	if (m_pTex)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
}

void CShader_3dapi_01_11::Render()
{
	if (m_pdev)
	{
		D3DXMATRIX		mtWorld;
		D3DXMATRIX		mtView;
		D3DXMATRIX		mtProj;

		D3DXMatrixIdentity(&mtWorld);

		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);

		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

		D3DXMATRIX mtWVP = mtWorld*mtView*mtProj;
		D3DXMatrixTranspose(&mtWVP, &mtWVP);

		D3DXMATRIX mtTex = D3DXMATRIX(
			0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, -0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f
			);

		m_pdev->SetVertexShader(m_pVertexShader);
		m_pdev->SetVertexDeclaration(m_pFVF);

		m_pdev->SetVertexShaderConstantF(0, (FLOAT*)&mtWVP, 4);

		D3DXMatrixTranspose(&mtTex, &mtTex);
		m_pdev->SetVertexShaderConstantF(8, (FLOAT*)&mtTex, 4);

		m_pdev->SetTexture(0, m_pTex);
		m_pdev->DrawIndexedPrimitiveUP(
			D3DPT_TRIANGLELIST, 0, m_iVertexNum, m_iIndexNum, m_pIndices, D3DFMT_INDEX16, m_pVertices, sizeof(Vertex)
		);

		m_pdev->SetVertexShader(NULL);
		m_pdev->SetVertexDeclaration(NULL);
		m_pdev->SetTexture(0, NULL);
	}
}

void CShader_3dapi_01_11::Update()
{
	if (m_pdev)
	{

	}
}
