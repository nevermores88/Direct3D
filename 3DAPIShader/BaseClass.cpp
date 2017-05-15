#include "stdafx.h"
#include "BaseClass.h"


CBaseClass::CBaseClass()
{
}


CBaseClass::~CBaseClass()
{
}

HRESULT CBaseClass::Create(LPDIRECT3DDEVICE9 pdev)
{
	m_pdev = pdev;

	return S_OK;
}

LPDIRECT3DVERTEXSHADER9 CBaseClass::LoadVertexShader(const char* szFileName, LPD3DXCONSTANTTABLE* pConstTbl)
{
	LPDIRECT3DVERTEXSHADER9 ret = NULL;

	DWORD dwShaderFlags = 0;

#if _DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

	HRESULT	hr = 0;

	LPD3DXBUFFER	pShader = NULL;
	LPD3DXBUFFER	pErr = NULL;

	hr = D3DXCompileShaderFromFile(szFileName
		, NULL
		, NULL
		, "VtxPrc"
		, "vs_2_0"
		, dwShaderFlags
		, &pShader
		, &pErr
		, pConstTbl
		);

	if (FAILED(hr) && pErr)
	{
		int size = pErr->GetBufferSize();
		void* ack = pErr->GetBufferPointer();

		if (ack)
		{
			char* str = new char[size];
			sprintf(str, (const char*)ack, size);
			OutputDebugString(str);
			delete[] str;
		}
	}

	if (pShader)
	{
		m_pdev->CreateVertexShader((DWORD*)pShader->GetBufferPointer(), &ret);
		pShader->Release();
	}

	return ret;
}

LPDIRECT3DPIXELSHADER9 CBaseClass::LoadPixelShader(const char* szFileName, LPD3DXCONSTANTTABLE* pConstTbl)
{
	LPDIRECT3DPIXELSHADER9 ret = NULL;

	DWORD dwShaderFlags = 0;

#if _DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

	HRESULT	hr = 0;

	LPD3DXBUFFER	pShader = NULL;
	LPD3DXBUFFER	pErr = NULL;

	hr = D3DXCompileShaderFromFile(szFileName
		, NULL
		, NULL
		, "PxlPrc"
		, "ps_2_0"
		, dwShaderFlags
		, &pShader
		, &pErr
		, pConstTbl
		);

	if (FAILED(hr) && pErr)
	{
		int size = pErr->GetBufferSize();
		void* ack = pErr->GetBufferPointer();

		if (ack)
		{
			char* str = new char[size];
			sprintf(str, (const char*)ack, size);
			OutputDebugString(str);
			delete[] str;
		}
	}

	if (pShader)
	{
		m_pdev->CreatePixelShader((DWORD*)pShader->GetBufferPointer(), &ret);
		pShader->Release();
	}

	return ret;
}

LPD3DXEFFECT CBaseClass::LoadShader(const char* szFileName)
{
	LPD3DXEFFECT ret = NULL;
	LPD3DXBUFFER pError = NULL;

	DWORD dwShaderFlags = 0;

#if _DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

	D3DXCreateEffectFromFile(m_pdev, szFileName, NULL, NULL, dwShaderFlags, NULL, &ret, &pError);

	if (!ret && pError)
	{
		int size = pError->GetBufferSize();
		void* ack = pError->GetBufferPointer();

		if (ack)
		{
			char* str = new char[size];
			sprintf(str, (const char*)ack, size);
			OutputDebugString(str);
			delete[] str;
		}
	}

	return ret;
}

LPD3DXMESH CBaseClass::LoadModel(const char* szFileName)
{
	LPD3DXMESH ret = NULL;

	if (FAILED(D3DXLoadMeshFromX(szFileName, D3DXMESH_MANAGED/*D3DXMESH_SYSTEMMEM*/, m_pdev, NULL, NULL, NULL, NULL, &ret)))
	{
		OutputDebugString("모델 로딩 실패: ");
		OutputDebugString(szFileName);
		OutputDebugString("\n");
	}

	return ret;
}

LPDIRECT3DTEXTURE9 CBaseClass::LoadTexture(const char* szFileName)
{
	LPDIRECT3DTEXTURE9 ret = NULL;

	if (FAILED(D3DXCreateTextureFromFile(m_pdev, szFileName, &ret)))
	{
		OutputDebugString("텍스처 로딩 실패: ");
		OutputDebugString(szFileName);
		OutputDebugString("\n");
	}

	return ret;
}

