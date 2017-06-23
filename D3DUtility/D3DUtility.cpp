#include "stdafx.h"
#include "D3DUtility.h"

//CD3DUtility* CD3DUtility::m_pThis = NULL;

CD3DUtility::CD3DUtility()
{
}


CD3DUtility::~CD3DUtility()
{
}

//CD3DUtility* CD3DUtility::GetThis()
//{
//	if (!m_pThis)
//	{
//		m_pThis = new CD3DUtility();
//	}
//
//	return m_pThis;
//}

LPD3DXEFFECT CD3DUtility::LoadShader(LPDIRECT3DDEVICE9 pdev, const char* szFileName)
{
	LPD3DXEFFECT ret = NULL;
	LPD3DXBUFFER pError = NULL;

	DWORD dwShaderFlags = 0;

#if _DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

	D3DXCreateEffectFromFile(pdev, szFileName, NULL, NULL, dwShaderFlags, NULL, &ret, &pError);

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

LPD3DXMESH CD3DUtility::LoadModel(LPDIRECT3DDEVICE9 pdev, const char* szFileName)
{
	LPD3DXMESH ret = NULL;

	if (FAILED(D3DXLoadMeshFromX(szFileName, D3DXMESH_MANAGED/*D3DXMESH_SYSTEMMEM*/, pdev, NULL, NULL, NULL, NULL, &ret)))
	{
		OutputDebugString("모델 로딩 실패: ");
		OutputDebugString(szFileName);
		OutputDebugString("\n");
	}

	return ret;
}

LPDIRECT3DTEXTURE9 CD3DUtility::LoadTexture(LPDIRECT3DDEVICE9 pdev, const char* szFileName)
{
	LPDIRECT3DTEXTURE9 ret = NULL;

	if (FAILED(D3DXCreateTextureFromFile(pdev, szFileName, &ret)))
	{
		OutputDebugString("텍스처 로딩 실패: ");
		OutputDebugString(szFileName);
		OutputDebugString("\n");
	}

	return ret;
}

DWORD CD3DUtility::VectorToRGB(D3DXVECTOR3* vNormal)
{
	float r = vNormal->x;
	float g = vNormal->y;
	float b = vNormal->z;

	DWORD dwR = (DWORD)(127 * r + 128);
	DWORD dwG = (DWORD)(127 * g + 128);
	DWORD dwB = (DWORD)(127 * b + 128);

	return (DWORD)(0xff000000 + (dwR << 16) + (dwG << 8) + dwB);
}
