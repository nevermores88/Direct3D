#include "stdafx.h"
#include "Shader_3dapi_01_07.h"


CShader_3dapi_01_07::CShader_3dapi_01_07()
{
}


CShader_3dapi_01_07::~CShader_3dapi_01_07()
{
}

HRESULT CShader_3dapi_01_07::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	HRESULT hr = 0;
	HWND hwnd;
	D3DDEVICE_CREATION_PARAMETERS ppm;

	m_pdev->GetCreationParameters(&ppm);
	hwnd = ppm.hFocusWindow;

	float fSize = 30.0f;

	// front
	m_pVertices[0] = Vertex(-1.f, -1.f, -1.f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pVertices[1] = Vertex(-1.f, 1.f, -1.f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	m_pVertices[2] = Vertex(1.f, 1.f, -1.f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	m_pVertices[3] = Vertex(1.f, -1.f, -1.f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));

	// back
	m_pVertices[4] = Vertex(-1.f, -1.f, 1.f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	m_pVertices[5] = Vertex(1.f, -1.f, 1.f, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
	m_pVertices[6] = Vertex(1.f, 1.f, 1.f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pVertices[7] = Vertex(-1.f, 1.f, 1.f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));

	// top
	m_pVertices[8] = Vertex(-1.f, 1.f, -1.f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	m_pVertices[9] = Vertex(-1.f, 1.f, 1.f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	m_pVertices[10] = Vertex(1.f, 1.f, 1.f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
	m_pVertices[11] = Vertex(1.f, 1.f, -1.f, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));

	// bottom
	m_pVertices[12] = Vertex(-1.f, -1.f, -1.f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	m_pVertices[13] = Vertex(1.f, -1.f, -1.f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	m_pVertices[14] = Vertex(1.f, -1.f, 1.f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	m_pVertices[15] = Vertex(-1.f, -1.f, 1.f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	// left
	m_pVertices[16] = Vertex(-1.f, -1.f, 1.f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	m_pVertices[17] = Vertex(-1.f, 1.f, 1.f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
	m_pVertices[18] = Vertex(-1.f, 1.f, -1.f, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
	m_pVertices[19] = Vertex(-1.f, -1.f, -1.f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));

	// right
	m_pVertices[20] = Vertex(1.f, -1.f, -1.f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	m_pVertices[21] = Vertex(1.f, 1.f, -1.f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	m_pVertices[22] = Vertex(1.f, 1.f, 1.f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
	m_pVertices[23] = Vertex(1.f, -1.f, 1.f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));


	for (int i = 0; i < 24; ++i)
	{
		m_pVertices[i].p *= fSize;
	}

	// front
	m_pIndices[0] = Index(0, 1, 2);
	m_pIndices[1] = Index(0, 2, 3);

	// back
	m_pIndices[2] = Index(4, 5, 6);
	m_pIndices[3] = Index(4, 6, 7);

	// top
	m_pIndices[4] = Index(8, 9, 10);
	m_pIndices[5] = Index(8, 10, 11);

	// bottom
	m_pIndices[6] = Index(12, 13, 14);
	m_pIndices[7] = Index(12, 14, 15);

	// left
	m_pIndices[8] = Index(16, 17, 18);
	m_pIndices[9] = Index(16, 18, 19);

	// right
	m_pIndices[10] = Index(20, 21, 22);
	m_pIndices[11] = Index(20, 22, 23);

	DWORD dwFlags = 0;

#if defined(_DEBUG) || defined(DEBUG)
	dwFlags |= D3DXSHADER_DEBUG;
#endif

	LPD3DXBUFFER pShader = NULL;
	LPD3DXBUFFER pError = NULL;

	hr = D3DXAssembleShaderFromFile("Ex01_07/Shader.vsh", NULL, NULL, dwFlags, &pShader, &pError);

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

void CShader_3dapi_01_07::Release()
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

void CShader_3dapi_01_07::Render()
{
	if (m_pdev)
	{
		D3DXMATRIX mtWorld;
		D3DXMATRIX mtView;
		D3DXMATRIX mtProj;

		D3DXMATRIX mtRotX;
		D3DXMATRIX mtRotY;
		D3DXMATRIX mtRotZ;

		float fAngle = D3DXToRadian(GetTickCount() * 0.1f);
		D3DXMatrixRotationY(&mtRotY, fAngle*3.f);
		D3DXMatrixRotationZ(&mtRotZ, fAngle*2.f);
		D3DXMatrixRotationX(&mtRotX, fAngle*1.f);

		mtWorld = mtRotY * mtRotZ * mtRotX;

		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);

		D3DXMATRIX mtWVP = mtWorld*mtView*mtProj;
		D3DXMatrixTranspose(&mtWVP, &mtWVP);
		
		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		m_pdev->SetVertexShader(m_pVertexShader);			//정점 쉐이더 사용
		m_pdev->SetVertexDeclaration(m_pFVF);					//정점 형식

		D3DXCOLOR color(1, 1, 1, 1);

		m_pdev->SetVertexShaderConstantF(0, (FLOAT*)&mtWVP, 4);	//c0 레지스터에 월드 * 뷰 * 투영 행렬 설정
		m_pdev->SetVertexShaderConstantF(10, (FLOAT*)&color, 1);	//c10 레지스터에 색상 설정
		m_pdev->SetVertexShaderConstantF(11, (FLOAT*)&color, 1);	//c11 레지스터에 색상 설정

		m_pdev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 24, 12, m_pIndices, D3DFMT_INDEX16, m_pVertices, sizeof(Vertex));

		m_pdev->SetVertexShader(NULL);
	}
}

void CShader_3dapi_01_07::Update()
{
	if (m_pdev)
	{

	}
}
