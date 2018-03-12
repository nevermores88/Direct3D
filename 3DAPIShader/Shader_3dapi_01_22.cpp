#include "stdafx.h"
#include "Shader_3dapi_01_22.h"

#include "Filed_01_22.h"

CShader_3dapi_01_22::CShader_3dapi_01_22()
{
}


CShader_3dapi_01_22::~CShader_3dapi_01_22()
{
}

HRESULT CShader_3dapi_01_22::Create(LPDIRECT3DDEVICE9 pdev)
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

	hr = D3DXAssembleShaderFromFile("Ex01_22/Shader.vsh", NULL, NULL, dwFlags, &pShader, &pError);

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

	D3DVERTEXELEMENT9 vertex_decl[MAX_FVF_DECL_SIZE] = { 0 };
	D3DXDeclaratorFromFVF(CFiled_01_22::VtxDUV1::FVF, vertex_decl);
	if (FAILED(m_pdev->CreateVertexDeclaration(vertex_decl, &m_pFVF)))
		return E_FAIL;

	if (pShader)
	{
		hr = m_pdev->CreateVertexShader((DWORD*)pShader->GetBufferPointer(), &m_pVertexShader);
		pShader->Release();
	}

	m_pField = new CFiled_01_22();
	m_pField->Create(m_pdev);

	return S_OK;
}

void CShader_3dapi_01_22::Release()
{
	if (m_pFVF)
	{
		m_pFVF->Release();
		m_pFVF = NULL;
	}

	if (m_pVertexShader)
	{
		m_pVertexShader->Release();
		m_pVertexShader = NULL;
	}
}

void CShader_3dapi_01_22::Render()
{
	if (m_pdev)
	{
		D3DXMATRIX mtWorld;
		D3DXMATRIX mtView;
		D3DXMATRIX mtProj;

		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);

		D3DXMatrixIdentity(&mtWorld);

		D3DXMATRIX	mtWorldView = mtWorld * mtView;
		D3DXMATRIX	mtWorldViewProj = mtWorld * mtView * mtProj;

		float fFogFactor[4];

		static float fBegin = 0.0f;
		static float fEnd = 400.0f;

		static DWORD dwBegin = timeGetTime();
		DWORD dwEnd = timeGetTime();
		static float fD = 10.0f;

		if (dwEnd - dwBegin > 60)
		{
			dwBegin = dwEnd;
			fEnd += fD;

			if (fEnd > 900)
			{
				fEnd = 900;
				fD = -fD;
			}

			if (fEnd < 300)
			{
				fEnd = 300;
				fD = -fD;
			}
		}

		fFogFactor[0] = fBegin;						//start
		fFogFactor[1] = fEnd;							//end
		fFogFactor[2] = fEnd / (fEnd - fBegin);	//FogF delta
		fFogFactor[3] = 1.0f / (fEnd - fBegin);	//delta reciprocal

		D3DXCOLOR fogColor(1.0f, 1.0f, 1.0f, 1.0f);

		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		m_pdev->SetRenderState(D3DRS_FOGENABLE, TRUE);
		m_pdev->SetRenderState(D3DRS_FOGCOLOR, fogColor);
		m_pdev->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);	//MEMO : 이게 필요. 픽셀 포그 관련인듯. 나중에 조사

		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		D3DXMatrixTranspose(&mtWorldView, &mtWorldView);
		D3DXMatrixTranspose(&mtWorldViewProj, &mtWorldViewProj);

		m_pdev->SetVertexShader(m_pVertexShader);
		m_pdev->SetVertexDeclaration(m_pFVF);

		m_pdev->SetVertexShaderConstantF(0, (FLOAT*)&mtWorldViewProj, 4);
		m_pdev->SetVertexShaderConstantF(4, (FLOAT*)&mtWorldView, 4);
		m_pdev->SetVertexShaderConstantF(12, (FLOAT*)fFogFactor, 1);

		m_pField->Render();

		m_pdev->SetRenderState(D3DRS_FOGENABLE, FALSE);
		m_pdev->SetVertexShader(NULL);
		m_pdev->SetVertexDeclaration(NULL);
	}
}

void CShader_3dapi_01_22::Update()
{
	if (m_pdev)
	{
		m_pField->FrameMove();
	}
}
