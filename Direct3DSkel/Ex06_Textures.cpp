#include "stdafx.h"
#include "Ex06_Textures.h"


CEx06_Textures::CEx06_Textures()
{
}


CEx06_Textures::~CEx06_Textures()
{
}

HRESULT CEx06_Textures::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	if (FAILED(D3DXCreateTextureFromFile(pdev, "Ex06_Textures/crate.jpg", &m_pTex)))
	{
		::MessageBox(NULL, "Couldn't find banna.bmp", "Ex06_Textures", MB_OK);
		return E_FAIL;
	}

	if (FAILED(m_pdev->CreateVertexBuffer(50 * 2 * sizeof(Vertex), 0, Vertex::FVF, D3DPOOL_DEFAULT, &m_pVB, NULL)))
	{
		return E_FAIL;
	}

	Vertex* pVertices;

	if (FAILED(m_pVB->Lock(0, 0, (void**)&pVertices, 0)))
		return E_FAIL;

	for (DWORD i = 0; i < 50; i++)
	{
		float theta = (2 * D3DX_PI*i) / (50 - 1);

		pVertices[2 * i + 0].x = sinf(theta);
		pVertices[2 * i + 0].y = -1.0f;
		pVertices[2 * i + 0].z = cosf(theta);
		pVertices[2 * i + 0].color = 0xffffffff;
#ifndef SHOW_HOW_TO_USE_TCI
		pVertices[2*i+0].u = ((float)i)/(50-1);
		pVertices[2 * i + 0].v = 1.0f;
#endif

		pVertices[2 * i + 1].x = sinf(theta);
		pVertices[2 * i + 1].y = 1.0f;
		pVertices[2 * i + 1].z = cosf(theta);
		pVertices[2 * i + 1].color = 0xff0000ff;
#ifndef SHOW_HOW_TO_USE_TCI
		pVertices[2 * i + 1].u = ((float)i) / (50 - 1);
		pVertices[2 * i + 1].v = 0.0f;
#endif
	}

	m_pVB->Unlock();

	return S_OK;
}

void CEx06_Textures::Release()
{
	if (m_pVB)
		m_pVB->Release();
}

void CEx06_Textures::Render()
{
	if (m_pdev)
	{
		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);

		//월드행렬 설정
		D3DXMATRIX mtWorld;
		D3DXMatrixIdentity(&mtWorld);
		D3DXMatrixRotationX(&mtWorld, timeGetTime()/1000.0f);
		m_pdev->SetTransform(D3DTS_WORLD, &mtWorld);

		//텍스쳐 설정
		m_pdev->SetTexture(0, m_pTex);
		m_pdev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
		m_pdev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

#ifdef SHOW_HOW_TO_USE_TCI
		D3DXMATRIX mtTex;

		mtTex._11 = 0.25f; mtTex._12 = 0.00f; mtTex._13 = 0.00f; mtTex._14 = 0.00f;
		mtTex._21 = 0.00f; mtTex._22 = -0.25f; mtTex._23 = 0.00f; mtTex._24 = 0.00f;
		mtTex._31 = 0.00f; mtTex._32 = 0.00f; mtTex._33 = 1.00f; mtTex._34 = 0.00f;
		mtTex._41 = 0.50f; mtTex._42 = 0.50f; mtTex._43 = 0.00f; mtTex._44 = 1.00f;

		m_pdev->SetTransform(D3DTS_TEXTURE0, &mtTex);
		m_pdev->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
		m_pdev->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
#endif

		m_pdev->SetStreamSource(0, m_pVB, 0, sizeof(Vertex));
		m_pdev->SetFVF(Vertex::FVF);
		m_pdev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2 * 50 - 2);
	}
}

void CEx06_Textures::Update()
{

}
