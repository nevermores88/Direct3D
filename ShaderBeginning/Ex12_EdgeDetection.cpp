#include "stdafx.h"
#include "Ex12_EdgeDetection.h"


CEx12_EdgeDetection::CEx12_EdgeDetection()
{
}


CEx12_EdgeDetection::~CEx12_EdgeDetection()
{
}

HRESULT CEx12_EdgeDetection::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pEnvironmentMappingShader = LoadShader("Ex12/EnvrionmentMapping.fx");
	if (!m_pEnvironmentMappingShader)
		return E_FAIL;

	m_pNoEffect = LoadShader("Ex12/NoEffect.fx");
	if (!m_pNoEffect)
		return E_FAIL;

	m_pGrayScale = LoadShader("Ex12/Grayscale.fx");
	if (!m_pGrayScale)
		return E_FAIL;

	m_pSepia = LoadShader("Ex12/Sepia.fx");
	if (!m_pSepia)
		return E_FAIL;

	m_pEdgeDetection = LoadShader("Ex12/EdgeDetection.fx");
	if (!m_pEdgeDetection)
		return E_FAIL;

	m_pEmboss = LoadShader("Ex12/Emboss.fx");
	if (!m_pEmboss)
		return E_FAIL;

	m_pTeapot = LoadModel("Ex12/TeapotWithTangent.x");
	if (!m_pTeapot)
		return E_FAIL;

	m_pTexDM = LoadTexture("Ex12/Fieldstone_DM.tga");
	if (!m_pTexDM)
		return E_FAIL;

	m_pTexSM = LoadTexture("Ex12/Fieldstone_SM.tga");
	if (!m_pTexSM)
		return E_FAIL;

	m_pTexNM = LoadTexture("Ex12/Fieldstone_NM.tga");
	if (!m_pTexNM)
		return E_FAIL;

	D3DXCreateCubeTextureFromFile(m_pdev, "Ex12/Snow_ENV.dds", &m_pSnowENV);
	if (!m_pSnowENV)
		return E_FAIL;

	InitFullScreenQuad();

	LPDIRECT3DSURFACE9 pSurfaceC = NULL;
	D3DSURFACE_DESC dscC;

	m_pdev->GetRenderTarget(0, &pSurfaceC);
	pSurfaceC->GetDesc(&dscC);

	if (FAILED(m_pdev->CreateTexture(dscC.Width, dscC.Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_pRenderTarget, NULL)))
		return false;

	pSurfaceC->Release();

	m_iProcessIdx = 0;

	return S_OK;
}

void CEx12_EdgeDetection::Release()
{
	if (m_pEnvironmentMappingShader)
	{
		m_pEnvironmentMappingShader->Release();
		m_pEnvironmentMappingShader = NULL;
	}
	if (m_pNoEffect)
	{
		m_pNoEffect->Release();
		m_pNoEffect = NULL;
	}
	if (m_pGrayScale)
	{
		m_pGrayScale->Release();
		m_pGrayScale = NULL;
	}
	if (m_pSepia)
	{
		m_pSepia->Release();
		m_pSepia = NULL;
	}
	if (m_pEdgeDetection)
	{
		m_pEdgeDetection->Release();
		m_pEdgeDetection = NULL;
	}
	if (m_pEmboss)
	{
		m_pEmboss->Release();
		m_pEmboss = NULL;
	}
	if (m_pTeapot)
	{
		m_pTeapot->Release();
		m_pTeapot = NULL;
	}
	if (m_pTexDM)
	{
		m_pTexDM->Release();
		m_pTexDM = NULL;
	}
	if (m_pTexSM)
	{
		m_pTexSM->Release();
		m_pTexSM = NULL;
	}
	if (m_pTexNM)
	{
		m_pTexNM->Release();
		m_pTexNM = NULL;
	}
	if (m_pSnowENV)
	{
		m_pSnowENV->Release();
		m_pSnowENV = NULL;
	}
	if (m_pQuadDecl)
	{
		m_pQuadDecl->Release();
		m_pQuadDecl = NULL;
	}
	if (m_pQuadVB)
	{
		m_pQuadVB->Release();
		m_pQuadVB = NULL;
	}
	if (m_pQuadIB)
	{
		m_pQuadIB->Release();
		m_pQuadIB = NULL;
	}
	if (m_pRenderTarget)
	{
		m_pRenderTarget->Release();
		m_pRenderTarget = NULL;
	}
}

void CEx12_EdgeDetection::Render()
{
	if (m_pdev)
	{
		D3DXVECTOR4 vLightPos = D3DXVECTOR4(500.0f, 500.0f, -500.0f, 1.0f);
		D3DXVECTOR4 vCameraPos(g_pCamera->GetEye());
		D3DXVECTOR4 vLightColor(0.7f, 0.7f, 1.0f, 1.0f);

		//월드 행렬 생성
		D3DXMATRIX mtWorld;
		D3DXMatrixIdentity(&mtWorld);

		//View, Proj 행렬 가져오기
		D3DXMATRIX mtView;
		D3DXMATRIX mtProj;
		D3DXMATRIX mtWorldView;
		D3DXMATRIX mtWorldViewProj;

		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);

		D3DXMatrixMultiply(&mtWorldView, &mtWorld, &mtView);
		D3DXMatrixMultiply(&mtWorldViewProj, &mtWorldView, &mtProj);

		/////////////////////////
		// 1. 장면을 렌더타깃 안에 그린다
		/////////////////////////
		// 현재 하드웨어 벡버퍼
		LPDIRECT3DSURFACE9	pHWBackBuffer = NULL;
		m_pdev->GetRenderTarget(0, &pHWBackBuffer);

		LPDIRECT3DSURFACE9	pSceneSurface = NULL;
		if (SUCCEEDED(m_pRenderTarget->GetSurfaceLevel(0, &pSceneSurface)))
		{
			m_pdev->SetRenderTarget(0, pSceneSurface);
			pSceneSurface->Release();
			pSceneSurface = NULL;
		}

		// 저번 프레임에 그렸던 장면을 지운다
		m_pdev->Clear(0, NULL, D3DCLEAR_TARGET, 0xFF000000, 1.0f, 0);

		//정점셰이더
		m_pEnvironmentMappingShader->SetMatrix("gWorldMatrix", &mtWorld);
		m_pEnvironmentMappingShader->SetMatrix("gWorldViewProjectionMatrix", &mtWorldViewProj);

		m_pEnvironmentMappingShader->SetVector("gWorldLightPosition", &vLightPos);
		m_pEnvironmentMappingShader->SetVector("gWorldCameraPosition", &vCameraPos);

		//픽셀셰이더
		m_pEnvironmentMappingShader->SetVector("gLightColor", &vLightColor);

		m_pEnvironmentMappingShader->SetTexture("DiffuseMap_Tex", m_pTexDM);
		m_pEnvironmentMappingShader->SetTexture("SpecularMap_Tex", m_pTexSM);
		m_pEnvironmentMappingShader->SetTexture("NormalMap_Tex", m_pTexNM);
		m_pEnvironmentMappingShader->SetTexture("EnvironmentMap_Tex", m_pSnowENV);

		// 쉐이더를 시작한다.
		UINT numPasses = 0;
		m_pEnvironmentMappingShader->Begin(&numPasses, NULL);
		{
			for (UINT i = 0; i < numPasses; ++i)
			{
				m_pEnvironmentMappingShader->BeginPass(i);
				{
					// 구체를 그린다.
					m_pTeapot->DrawSubset(0);
				}
				m_pEnvironmentMappingShader->EndPass();
			}
		}
		m_pEnvironmentMappingShader->End();

		m_pdev->SetRenderTarget(0, pHWBackBuffer);

		/////////////////////////
		// 2. 포스트프로세싱을 적용한다.
		/////////////////////////
		// 하드웨어 백버퍼를 사용한다.
		m_pdev->SetRenderTarget(0, pHWBackBuffer);
		pHWBackBuffer->Release();
		pHWBackBuffer = NULL;

		// 사용할 포스트프로세스 효과
		LPD3DXEFFECT	pEffectToUse = m_pNoEffect;
		if (m_iProcessIdx == 1)
		{
			pEffectToUse = m_pGrayScale;
		}
		else if (m_iProcessIdx == 2)
		{
			pEffectToUse = m_pSepia;
		}
		else if (m_iProcessIdx == 3)
		{
			pEffectToUse = m_pEdgeDetection;
		}
		else if (m_iProcessIdx == 4)
		{
			pEffectToUse = m_pEmboss;
		}

		int iWidth;
		int iHeight;

		g_pGame->GetWidthHeight(iWidth, iHeight);

		D3DXVECTOR4 pixelOffset(1 / (float)iWidth, 1 / (float)iHeight, 0, 0);
		if (pEffectToUse == m_pEdgeDetection || pEffectToUse == m_pEmboss)
		{
			pEffectToUse->SetVector("gPixelOffset", &pixelOffset);
		}

		pEffectToUse->SetTexture("SceneTexture_Tex", m_pRenderTarget);
		pEffectToUse->Begin(&numPasses, NULL);
		{
			for (UINT i = 0; i < numPasses; ++i)
			{
				pEffectToUse->BeginPass(i);
				{
					m_pdev->SetStreamSource(0, m_pQuadVB, 0, sizeof(float) * 5);
					m_pdev->SetIndices(m_pQuadIB);
					m_pdev->SetVertexDeclaration(m_pQuadDecl);
					m_pdev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);
				}
				pEffectToUse->EndPass();
			}
		}
		pEffectToUse->End();
	}
}

void CEx12_EdgeDetection::Update()
{
	if (m_pdev)
	{
		if (m_pdev)
		{
			if (GetAsyncKeyState('1'))
			{
				m_iProcessIdx = 0;
			}
			else if (GetAsyncKeyState('2'))
			{
				m_iProcessIdx = 1;
			}
			else if (GetAsyncKeyState('3'))
			{
				m_iProcessIdx = 2;
			}
			else if (GetAsyncKeyState('4'))
			{
				m_iProcessIdx = 3;
			}
			else if (GetAsyncKeyState('5'))
			{
				m_iProcessIdx = 4;
			}
		}
	}
}

void CEx12_EdgeDetection::InitFullScreenQuad()
{
	D3DVERTEXELEMENT9 vtxDesc[3];

	int iOffset = 0;
	int i = 0;

	//위치
	vtxDesc[i].Stream = 0;
	vtxDesc[i].Offset = iOffset;
	vtxDesc[i].Type = D3DDECLTYPE_FLOAT3;
	vtxDesc[i].Method = D3DDECLMETHOD_DEFAULT;
	vtxDesc[i].Usage = D3DDECLUSAGE_POSITION;
	vtxDesc[i].UsageIndex = 0;

	iOffset += sizeof(float) * 3;
	++i;

	//UV좌표 0
	vtxDesc[i].Stream = 0;
	vtxDesc[i].Offset = iOffset;
	vtxDesc[i].Type = D3DDECLTYPE_FLOAT2;
	vtxDesc[i].Method = D3DDECLMETHOD_DEFAULT;
	vtxDesc[i].Usage = D3DDECLUSAGE_TEXCOORD;
	vtxDesc[i].UsageIndex = 0;

	iOffset += sizeof(float) * 2;
	++i;

	// 정점포맷의 끝임을 표현 (D3DDECL_END())
	vtxDesc[i].Stream = 0xFF;
	vtxDesc[i].Offset = 0;
	vtxDesc[i].Type = D3DDECLTYPE_UNUSED;
	vtxDesc[i].Method = 0;
	vtxDesc[i].Usage = 0;
	vtxDesc[i].UsageIndex = 0;

	m_pdev->CreateVertexDeclaration(vtxDesc, &m_pQuadDecl);

	m_pdev->CreateVertexBuffer(iOffset * 4, 0, 0, D3DPOOL_MANAGED, &m_pQuadVB, NULL);
	void*	vertexData = NULL;
	m_pQuadVB->Lock(0, 0, &vertexData, 0);
	{
		float* data = (float*)vertexData;
		*data++ = -1.0f;	*data++ = 1.0f;		*data++ = 0.0f;
		*data++ = 0.0f;		*data++ = 0.0f;

		*data++ = 1.0f;		*data++ = 1.0f;		*data++ = 0.0f;
		*data++ = 1.0f;		*data++ = 0;

		*data++ = 1.0f;		*data++ = -1.0f;	*data++ = 0.0f;
		*data++ = 1.0f;		*data++ = 1.0f;

		*data++ = -1.0f;	*data++ = -1.0f;	*data++ = 0.0f;
		*data++ = 0.0f;		*data++ = 1.0f;
	}
	m_pQuadVB->Unlock();

	// 색인버퍼를 만든다.
	m_pdev->CreateIndexBuffer(sizeof(short) * 6, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pQuadIB, NULL);
	void * indexData = NULL;
	m_pQuadIB->Lock(0, 0, &indexData, 0);
	{
		unsigned short* data = (unsigned short*)indexData;
		*data++ = 0;	*data++ = 1;	*data++ = 3;
		*data++ = 3;	*data++ = 1;	*data++ = 2;
	}
	m_pQuadIB->Unlock();
}
