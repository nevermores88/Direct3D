#include "stdafx.h"
#include "Shader_3dapi_01_06.h"


CShader_3dapi_01_06::CShader_3dapi_01_06()
{
}


CShader_3dapi_01_06::~CShader_3dapi_01_06()
{
}

HRESULT CShader_3dapi_01_06::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	HRESULT hr = 0;
	HWND hwnd;
	D3DDEVICE_CREATION_PARAMETERS ppm;

	m_pdev->GetCreationParameters(&ppm);
	hwnd = ppm.hFocusWindow;

	m_pVertices[0] = Vertex(-50, 0, 0, D3DXCOLOR(1, 0, 0, 1));
	m_pVertices[1] = Vertex(50, 0, 0, D3DXCOLOR(0, 1, 0, 1));
	m_pVertices[2] = Vertex(50, 80, 0, D3DXCOLOR(0, 0, 1, 1));
	m_pVertices[3] = Vertex(-50, 80, 0, D3DXCOLOR(1, 0, 1, 1));

	DWORD dwFlags = 0;

#if defined(_DEBUG) || defined(DEBUG)
	dwFlags |= D3DXSHADER_DEBUG;
#endif

	LPD3DXBUFFER pShader = NULL;
	LPD3DXBUFFER pError = NULL;

	hr = D3DXAssembleShaderFromFile("Ex01_06/Shader.vsh", NULL, NULL, dwFlags, &pShader, &pError);

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

void CShader_3dapi_01_06::Release()
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
}

void CShader_3dapi_01_06::Render()
{
	if (m_pdev)
	{
		D3DXMATRIX mtWorld;
		D3DXMATRIX mtView;
		D3DXMATRIX mtProj;

		D3DXMatrixIdentity(&mtWorld);

		float fAngle = D3DXToRadian(GetTickCount() * 0.1f);
		D3DXMatrixRotationY(&mtWorld, fAngle);

		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);

		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		m_pdev->SetVertexShader(m_pVertexShader);			//정점 쉐이더 사용
		m_pdev->SetVertexDeclaration(m_pFVF);					//정점 형식

		D3DXMATRIX mtTranspose;
		D3DXMatrixTranspose(&mtTranspose, &mtWorld);
		m_pdev->SetVertexShaderConstantF(0, (FLOAT*)&mtTranspose, 4);

		D3DXMatrixTranspose(&mtTranspose, &mtView);
		m_pdev->SetVertexShaderConstantF(4, (FLOAT*)&mtTranspose, 4);

		D3DXMatrixTranspose(&mtTranspose, &mtProj);
		m_pdev->SetVertexShaderConstantF(8, (FLOAT*)&mtTranspose, 4);

		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_pVertices, sizeof(Vertex));

		m_pdev->SetVertexShader(NULL);
		m_pdev->SetVertexDeclaration(NULL);
	}
}

void CShader_3dapi_01_06::Update()
{
	if (m_pdev)
	{

	}
}
