#include "stdafx.h"
#include "Basic_3dapi_07_01.h"


CBasic_3dapi_07_01::CBasic_3dapi_07_01()
{
}


CBasic_3dapi_07_01::~CBasic_3dapi_07_01()
{
}

HRESULT CBasic_3dapi_07_01::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pVertices[0] = Vertex(-6.f, 5.f, 5.f, D3DXCOLOR(1, 0, 0, 0));
	m_pVertices[1] = Vertex(6.f, 5.f, 5.f, D3DXCOLOR(1, 0, 0, 1));
	m_pVertices[2] = Vertex(6.f, -5.f, 5.f, D3DXCOLOR(1, 0, 0, 1));
	m_pVertices[3] = Vertex(-6.f, -5.f, 5.f, D3DXCOLOR(1, 0, 0, 0));

	return S_OK;
}

void CBasic_3dapi_07_01::Release()
{

}

void CBasic_3dapi_07_01::Render()
{
	if (m_pdev)
	{
		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		m_pdev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pdev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		m_pdev->SetTexture(0, NULL);
		m_pdev->SetFVF(Vertex::FVF);
		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_pVertices, sizeof(Vertex));

		m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pdev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pdev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

void CBasic_3dapi_07_01::Update()
{
	if (m_pdev)
	{
		
	}
}
