#include "stdafx.h"
#include "Ex05_01LightMapping.h"


CEx05_01LightMapping::CEx05_01LightMapping()
{
}


CEx05_01LightMapping::~CEx05_01LightMapping()
{
}

HRESULT CEx05_01LightMapping::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	//정점 정보 생성
	Vertex vertices[]	=
	{
		{-1, 1, 0, 0xffffffff, 0, 0},
		{ 1, 1, 0, 0xffffffff, 1, 0 },
		{ -1, -1, 0, 0xffffffff, 0, 1 },
		{ 1, -1, 0, 0xffffffff, 1, 1 }
	};

	if (FAILED(m_pdev->CreateVertexBuffer(4 * sizeof(Vertex), 0, Vertex::FVF, D3DPOOL_DEFAULT, &m_pVB, NULL)))
		return E_FAIL;

	VOID* pVertices;

	if (FAILED(m_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
		return E_FAIL;

	memcpy(pVertices, vertices, sizeof(vertices));

	m_pVB->Unlock();

	//텍스처 정보 생성
	if (FAILED(D3DXCreateTextureFromFile(m_pdev, L"Ex05_01/env2.bmp", &m_pTex0)))
		return E_FAIL;

	if (FAILED(D3DXCreateTextureFromFile(m_pdev, L"Ex05_01/spotlite.bmp", &m_pTex1)))
		return E_FAIL;

	return S_OK;
}

void CEx05_01LightMapping::Release()
{
	if (m_pVB)
	{
		m_pVB->Release();
		m_pVB = NULL;
	}

	if (m_pTex0)
	{
		m_pTex0->Release();
		m_pTex0 = NULL;
	}

	if (m_pTex1)
	{
		m_pTex1->Release();
		m_pTex1 = NULL;
	}
}

void CEx05_01LightMapping::Render()
{
	if (m_pdev)
	{
		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);

		m_pdev->SetTexture(0, m_pTex0);
		m_pdev->SetTexture(1, m_pTex1);

		//Vertex 구조체에서 0번 텍스쳐 좌표를 사용하도록 설정
		m_pdev->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
		m_pdev->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);

		//텍스쳐 품질 설정
		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		
		m_pdev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pdev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		m_pdev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

		m_pdev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);

		m_pdev->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pdev->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
		m_pdev->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

		m_pdev->SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);
		m_pdev->SetTextureStageState(2, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

		D3DXMATRIX mtWorld;
		D3DXMatrixIdentity(&mtWorld);
		m_pdev->SetTransform(D3DTS_WORLD, &mtWorld);

		m_pdev->SetStreamSource(0, m_pVB, 0, sizeof(Vertex));
		m_pdev->SetFVF(Vertex::FVF);
		m_pdev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

void CEx05_01LightMapping::Update()
{

}
