#include "stdafx.h"
#include "Surface_3dapi_01.h"


CSurface_3dapi_01::CSurface_3dapi_01()
{
}


CSurface_3dapi_01::~CSurface_3dapi_01()
{
}

HRESULT CSurface_3dapi_01::Create(LPDIRECT3DDEVICE9 pdev)
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

	/*m_pVertices[0] = Vertex(200.f, 200.f, 0.f, 0xFFFFFF, 0.0005f, 0.05f);
	m_pVertices[1] = Vertex(sx - 200.f, 200.f, 0.f, 0xFFFFFF, 0.9995f, 0.05f);
	m_pVertices[2] = Vertex(200.f, sy - 200.f, 0.f, 0xFFFFFF, 0.0005f, 0.95f);
	m_pVertices[3] = Vertex(sx - 200.f, sy - 200.f, 0.f, 0xFFFFFF, 0.9995f, 0.95f);*/
	m_pVertices[0] = Vertex(0, 0, 0.f, 0xFFFFFF, 0, 0);
	m_pVertices[1] = Vertex(sx, 0, 0.f, 0xFFFFFF, 1, 0);
	m_pVertices[2] = Vertex(0, sy , 0.f, 0xFFFFFF, 0, 1);
	m_pVertices[3] = Vertex(sx, sy, 0.f, 0xFFFFFF, 1, 1); 

	D3DXCreateTeapot(m_pdev, &m_pTeapot, NULL);

	m_pdev->CreateTexture(sx*1.5, sy*1.5, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTex, NULL);
	m_pdev->CreateDepthStencilSurface(sx*1.5, sy * 1.5, D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pShadowDepthStencil, NULL);
	m_pTex->GetSurfaceLevel(0, &m_pSurf);

	return S_OK;
}

void CSurface_3dapi_01::Release()
{
	if (!m_pTex)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}

	if (!m_pSurf)
	{
		m_pSurf->Release();
		m_pSurf = NULL;
	}
}

void CSurface_3dapi_01::Render()
{
	if (m_pdev)
	{
		m_pdev->SetRenderState(D3DRS_ZENABLE, FALSE);

		m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

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

void CSurface_3dapi_01::Update()
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

		LPDIRECT3DSURFACE9	pHWBackBuffer = NULL;		// Back buffer Depth and stencil
		LPDIRECT3DSURFACE9	pHWDepthStencilBuffer = NULL;		// Back buffer target

		m_pdev->GetRenderTarget(0, &pHWBackBuffer);
		m_pdev->GetDepthStencilSurface(&pHWDepthStencilBuffer);
		m_pdev->SetRenderTarget(0, m_pSurf);
		m_pdev->SetDepthStencilSurface(m_pShadowDepthStencil);

		m_pdev->Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, /*0xFF006699*/0xFF00FFFF, 1.0f, 0L);

		//¾À ·»´õ¸µ
		D3DXMATRIX mtScale;
		D3DXMatrixScaling(&mtScale, 10, 10, 10);
		m_pdev->SetTransform(D3DTS_WORLD, &mtScale);
		m_pTeapot->DrawSubset(0);

		m_pdev->SetRenderTarget(0, pHWBackBuffer);
		m_pdev->SetDepthStencilSurface(pHWDepthStencilBuffer);

		pHWBackBuffer->Release();
		pHWDepthStencilBuffer->Release();
	}
}
