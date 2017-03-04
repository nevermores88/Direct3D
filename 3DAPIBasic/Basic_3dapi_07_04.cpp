#include "stdafx.h"
#include "Basic_3dapi_07_04.h"


CBasic_3dapi_07_04::CBasic_3dapi_07_04()
{
}


CBasic_3dapi_07_04::~CBasic_3dapi_07_04()
{
}

HRESULT CBasic_3dapi_07_04::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pVertices1[8] = Vertex(0.f, -200.f, 0.f, 0x00000000);
	m_pVertices2[8] = Vertex(0.f, -200.f, 0.f, 0x00000000);

	for (int i = 0; i < 8; ++i)
	{
		m_pVertices1[i].x = 300.f * cosf(D3DXToRadian(360.f * i / 8));
		m_pVertices1[i].y = 180.f * 1.f + 50.f;
		m_pVertices1[i].z = 300.f * sinf(D3DXToRadian(360.f * i / 8));
		m_pVertices1[i].color = D3DXCOLOR(rand() % 6 * 0.1f, rand() % 6 * 0.1f, rand() % 6 * 0.1f, 1.f);

		m_pVertices2[i].x = 400.f * cosf(D3DXToRadian(360.f * i / 8));
		m_pVertices2[i].y = 120.f * 1.f + 50.f;
		m_pVertices2[i].z = 400.f * sinf(D3DXToRadian(360.f * i / 8));
		m_pVertices2[i].color = D3DXCOLOR(rand() % 6 * 0.1f, rand() % 6 * 0.1f, rand() % 6 * 0.1f, 1.f);
	}

	for (int i = 0; i < 8; ++i)
	{
		m_pIndicies[i] = Index(8, (i + 1) % 8, (i + 2) % 8);
	}

	return S_OK;
}

void CBasic_3dapi_07_04::Release()
{

}

void CBasic_3dapi_07_04::Render()
{
	if (m_pdev)
	{
		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		m_pdev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		m_pdev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

		m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pdev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pdev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);
		
		m_pdev->SetTexture(0, 0);
		m_pdev->SetFVF(Vertex::FVF);

		//m_pdev->SetRenderState(D3DRS_ZENABLE, FALSE);
		m_pdev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		m_pdev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8 + 1, 8, m_pIndicies, D3DFMT_INDEX16, m_pVertices1, sizeof(Vertex));

		//m_pdev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		m_pdev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8 + 1, 8, m_pIndicies, D3DFMT_INDEX16, m_pVertices2, sizeof(Vertex));

		m_pdev->SetRenderState(D3DRS_ZENABLE, TRUE);
		m_pdev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}

void CBasic_3dapi_07_04::Update()
{
	if (m_pdev)
	{

	}
}
