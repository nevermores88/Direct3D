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

