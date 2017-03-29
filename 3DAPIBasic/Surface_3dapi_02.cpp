#include "stdafx.h"
#include "Surface_3dapi_02.h"


CSurface_3dapi_02::CSurface_3dapi_02()
{
}


CSurface_3dapi_02::~CSurface_3dapi_02()
{
}

HRESULT CSurface_3dapi_02::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_iTexWidth = 128;

	LPDIRECT3DSURFACE9	pBackSurface = NULL;
	D3DSURFACE_DESC	desc;

	if (FAILED(m_pdev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackSurface)))
		return E_FAIL;

	pBackSurface->GetDesc(&desc);
	pBackSurface->Release();

	float sx = float(desc.Width);
	float sy = float(desc.Height);

	m_pVertices[0] = Vertex(0, 0, 0.f, 0xFFFFFF, 0, 0);
	m_pVertices[1] = Vertex(sx, 0, 0.f, 0xFFFFFF, 1, 0);
	m_pVertices[2] = Vertex(0, sy, 0.f, 0xFFFFFF, 0, 1);
	m_pVertices[3] = Vertex(sx, sy, 0.f, 0xFFFFFF, 1, 1);

	D3DXCreateTeapot(m_pdev, &m_pTeapot, NULL);

	{
		LPDIRECT3DSURFACE9 pSurface;
		D3DSURFACE_DESC descColor;
		D3DSURFACE_DESC descDepth;

		if (FAILED(m_pdev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pSurface)))
			return E_FAIL;

		pSurface->GetDesc(&descColor);
		pSurface->Release();

		if (FAILED(m_pdev->GetDepthStencilSurface(&pSurface)))
			return E_FAIL;

		pSurface->GetDesc(&descDepth);
		pSurface->Release();

		D3DXCreateTexture(m_pdev, sx, sy, 1, D3DUSAGE_RENDERTARGET, descColor.Format, D3DPOOL_DEFAULT, &m_pTex);
		m_pTex->GetSurfaceLevel(0, &m_pSurf);

		D3DSURFACE_DESC	descTex;
		m_pSurf->GetDesc(&descTex);

		D3DXCreateRenderToSurface(m_pdev, descTex.Width, descTex.Height, descColor.Format, TRUE, descDepth.Format, &m_pRenderToSurface);
	}

	return S_OK;
}

void CSurface_3dapi_02::Release()
{

}

void CSurface_3dapi_02::Render()
{
	if (m_pdev)
	{
		m_pdev->SetRenderState(D3DRS_ZENABLE, FALSE);

		m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

		m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		D3DXMATRIX mtScale;
		D3DXMatrixScaling(&mtScale, 10, 10, 10);
		m_pdev->SetTransform(D3DTS_WORLD, &mtScale);
		m_pTeapot->DrawSubset(0);

		m_pdev->SetTexture(0, m_pTex);
		m_pdev->SetFVF(Vertex::FVF);
		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_pVertices, sizeof(Vertex));

		m_pdev->SetRenderState(D3DRS_ZENABLE, TRUE);
	}
}

void CSurface_3dapi_02::Update()
{
	if (m_pdev)
	{
		m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

		m_pdev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

		m_pdev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		m_pdev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pdev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

		m_pRenderToSurface->BeginScene(m_pSurf, NULL);
		m_pdev->Clear(0L, NULL
			, D3DCLEAR_TARGET | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER
			, 0xFF006699, 1.0f, 0L);

		D3DXMATRIX mtScale;
		D3DXMatrixScaling(&mtScale, 10, 10, 10);
		m_pdev->SetTransform(D3DTS_WORLD, &mtScale);
		m_pTeapot->DrawSubset(0);

		m_pRenderToSurface->EndScene(D3DX_FILTER_NONE);
	}
}
