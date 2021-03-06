#include "stdafx.h"
#include "Shader_3dapi_01_04.h"


CShader_3dapi_01_04::CShader_3dapi_01_04()
{
}


CShader_3dapi_01_04::~CShader_3dapi_01_04()
{
}

HRESULT CShader_3dapi_01_04::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	HRESULT hr = 0;
	HWND hwnd;
	D3DDEVICE_CREATION_PARAMETERS ppm;

	m_pdev->GetCreationParameters(&ppm);
	hwnd = ppm.hFocusWindow;

	m_pVertices[0] = Vertex(-50, 0, 0, D3DXCOLOR(1, 0, 0, 1));
	m_pVertices[1] = Vertex(50, 0, 0, D3DXCOLOR(0, 1, 0, 1));
	m_pVertices[2] = Vertex(0, 80, 0, D3DXCOLOR(0, 0, 1, 1));

	const char szShader[] =
		"vs_1_1;					정점 쉐이더 버전 1.1	\n"
		"dcl_position v0;			정점 위치를 입력 레지스터 v0에 선언	\n"
		"dcl_color v1;				정점 색상을 입력 레지스터 v1에 선언	\n"
		"m4x4 oPos, v0, c0;		정점의 변환: 출력 위치 = 입력 위치 * c0에 입력된 행렬	\n"
		"mov oD0, v1;				정점의 색상 설정: 입력된 정점의 색상을 그대로 복사	\n"
	;

	DWORD dwFlags = 0;

#if defined(_DEBUG) || defined(DEBUG)
	dwFlags |= D3DXSHADER_DEBUG;
#endif

	LPD3DXBUFFER pShader = NULL;
	LPD3DXBUFFER pError = NULL;
	int iLen = strlen(szShader);

	hr = D3DXAssembleShader(
		szShader, iLen, NULL, NULL, dwFlags, &pShader, &pError);

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


	/*	D3DVERTEXELEMENT9 vertex_decl[MAX_FVF_DECL_SIZE] =
		{
		{ 0, 0				, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 3*sizeof(float), D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		D3DDECL_END()
		};*/

	D3DVERTEXELEMENT9 vertex_decl[MAX_FVF_DECL_SIZE] = { 0 };
	D3DXDeclaratorFromFVF(Vertex::FVF, vertex_decl);
	if (FAILED(m_pdev->CreateVertexDeclaration(vertex_decl, &m_pFVF)))
		return E_FAIL;

	return S_OK;
}

void CShader_3dapi_01_04::Release()
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

void CShader_3dapi_01_04::Render()
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

		D3DXMATRIX mtWVP = mtWorld*mtView*mtProj;
		D3DXMatrixTranspose(&mtWVP, &mtWVP);

		m_pdev->SetVertexShader(m_pVertexShader);			//정점 쉐이더 사용
		m_pdev->SetVertexDeclaration(m_pFVF);					//정점 형식

		// 상수 연결:	c0,, 행렬이므로 float4 가 4개
		m_pdev->SetVertexShaderConstantF(0, (FLOAT*)&mtWVP, 4);

		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, m_pVertices, sizeof(Vertex));

		m_pdev->SetVertexShader(NULL);
		m_pdev->SetVertexDeclaration(NULL);
	}
}

void CShader_3dapi_01_04::Update()
{
	if (m_pdev)
	{

	}
}
