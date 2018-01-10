#include "stdafx.h"
#include "Shader_3dapi_01_02.h"


CShader_3dapi_01_02::CShader_3dapi_01_02()
{
}


CShader_3dapi_01_02::~CShader_3dapi_01_02()
{
}

HRESULT CShader_3dapi_01_02::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	HRESULT hr = 0;
	HWND hwnd;
	D3DDEVICE_CREATION_PARAMETERS ppm;

	m_pdev->GetCreationParameters(&ppm);
	hwnd = ppm.hFocusWindow;

	m_pVertices[0] = Vertex(-1, -1, 0);
	m_pVertices[1] = Vertex(0, 1, 0);
	m_pVertices[2] = Vertex(1, -1, 0);

	const char szShader[] =
		"vs_1_1;				정점 쉐이더 버전 1.1	\n"
		"def c10, 1, 1, 0, 1; 상수 레지스터 c10에 xyzw 또는 rgba 값을 설정		\n"
		"dcl_position v0;		정점 위치를 입력 레지스터 v0에 선언	\n"
		"mov oPos, v0;		v0에 적재된 위치 값을 출력 위치 레지스터에 복사	\n"
		"mov oD0, c10;		c10에 설정된 상수 값을 디퓨즈 출력 레지스터에 복사	\n"
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

	D3DVERTEXELEMENT9 vertex_decl[MAX_FVF_DECL_SIZE] = { 0 };

	D3DXDeclaratorFromFVF(Vertex::FVF, vertex_decl);
	if (FAILED(m_pdev->CreateVertexDeclaration(vertex_decl, &m_pFVF)))
		return E_FAIL;

	return S_OK;
}

void CShader_3dapi_01_02::Release()
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

void CShader_3dapi_01_02::Render()
{
	if (m_pdev)
	{
		m_pdev->SetVertexShader(m_pVertexShader);			//정점 쉐이더 사용
		m_pdev->SetVertexDeclaration(m_pFVF);					//정점 형식

		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, m_pVertices, sizeof(Vertex));

		m_pdev->SetVertexShader(NULL);
		m_pdev->SetVertexDeclaration(NULL);
	}
}

void CShader_3dapi_01_02::Update()
{
	if (m_pdev)
	{

	}
}
