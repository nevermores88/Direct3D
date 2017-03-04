#include "stdafx.h"
#include "Basic_3dapi_07_03.h"


CBasic_3dapi_07_03::CBasic_3dapi_07_03()
{
}


CBasic_3dapi_07_03::~CBasic_3dapi_07_03()
{
}

HRESULT CBasic_3dapi_07_03::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pVertices[0] = Vertex(-6.f, 5.f, 5.f, D3DXCOLOR(1, 0, 0, 0), 0.f, 0.f);
	m_pVertices[1] = Vertex(6.f, 5.f, 5.f, D3DXCOLOR(1, 0, 0, 1), 1.f, 0.f);
	m_pVertices[2] = Vertex(6.f, -5.f, 5.f, D3DXCOLOR(1, 0, 0, 1), 1.f, 1.f);
	m_pVertices[3] = Vertex(-6.f, -5.f, 5.f, D3DXCOLOR(1, 0, 0, 0), 0.f, 1.f);

	D3DXCreateTextureFromFile(m_pdev, "Ex07_03/env3_alpha.png", &m_pTex);

	return S_OK;
}

void CBasic_3dapi_07_03::Release()
{
	if (m_pTex)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
}

void CBasic_3dapi_07_03::Render()
{
	if (m_pdev)
	{
		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		DWORD v;
		m_pdev->GetTextureStageState(0, D3DTSS_ALPHAARG1, &v);		// Default는 Texture로 되어 있음
		m_pdev->GetTextureStageState(0, D3DTSS_ALPHAARG2, &v);		// Default는 Current로 되어 있음
		m_pdev->GetTextureStageState(0, D3DTSS_ALPHAOP, &v);		// Default는 SelectArg1으로 되어 있음

		// Default 설정을 변경한다.
		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		m_pdev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

		m_pdev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pdev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		m_pdev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);


		m_pdev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pdev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		m_pdev->SetTexture(0, m_pTex);
		m_pdev->SetFVF(Vertex::FVF);
		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_pVertices, sizeof(Vertex));


		m_pdev->SetTexture(0, NULL);

		m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pdev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pdev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

void CBasic_3dapi_07_03::Update()
{
	if (m_pdev)
	{

	}
}
