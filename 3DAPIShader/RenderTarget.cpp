#include "stdafx.h"
#include "RenderTarget.h"


CRenderTarget::CRenderTarget()
{
}


CRenderTarget::~CRenderTarget()
{
}

HRESULT CRenderTarget::Create(void* p1 /*= NULL*/, void* p2 /*= NULL*/, void* p3 /*= NULL*/, void* p4 /*= NULL*/)
{
	m_pdev = (LPDIRECT3DDEVICE9)p1;
	m_iWidth = (int)p2;
	m_iHeight = (int)p3;
	m_iType = (int)p4;

	return CreateRenderSurface();
}

void CRenderTarget::Release()
{
	if (m_pRts)
	{
		m_pRts->Release();
		m_pRts = NULL;
	}

	if (m_pTex)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}

	if (m_pSurface)
	{
		m_pSurface->Release();
		m_pSurface = NULL;
	}
}

int CRenderTarget::BeginScene(DWORD dwClearMode /*= (0x1L | 0x2L | 0x4L)*/, DWORD dwClearColor /*= 0xFF006699*/)
{
	HRESULT hr = -1;

	hr = m_pRts->BeginScene(m_pSurface, NULL);

	if (0xFFFFFFFF != dwClearColor)
		hr = m_pdev->Clear(0, NULL, dwClearMode, dwClearColor, 1.0f, 0);

	return hr;
}

int CRenderTarget::EndScene()
{
	HRESULT hr = -1;

	if (0 == m_iType)
		hr = m_pRts->EndScene(0);

	return hr;
}

int CRenderTarget::GetWidth()
{
	return m_iWidth;
}

int CRenderTarget::GetHeight()
{
	return m_iHeight;
}

DWORD CRenderTarget::GetDepth()
{
	return m_dwDepthFormat;
}

void* CRenderTarget::GetTexture() const
{
	return m_pTex;
}

void* CRenderTarget::GetSurface() const
{
	return m_pSurface;
}

//º¹½À
int CRenderTarget::CreateRenderSurface()
{
	HRESULT hr = -1;

	DWORD dwMip = 1;
	LPDIRECT3DSURFACE9 pSurfaceC = NULL;
	LPDIRECT3DSURFACE9 pSurfaceD = NULL;
	D3DSURFACE_DESC dscC;
	D3DSURFACE_DESC dscD;

	D3DCAPS9 caps;

	m_pdev->GetRenderTarget(0, &pSurfaceC);
	m_pdev->GetDepthStencilSurface(&pSurfaceD);

	pSurfaceC->GetDesc(&dscC);
	pSurfaceD->GetDesc(&dscD);

	m_pdev->GetDeviceCaps(&caps);

	pSurfaceC->Release();
	pSurfaceD->Release();

	if (m_iWidth < 0)
		m_iWidth = dscC.Width;

	if (m_iHeight < 0)
		m_iHeight = dscC.Height;

	hr = D3DXCreateRenderToSurface(m_pdev, m_iWidth, m_iHeight, dscC.Format, TRUE, dscD.Format, &m_pRts);
	hr = D3DXCreateTexture(m_pdev, m_iWidth, m_iHeight, dwMip, D3DUSAGE_RENDERTARGET, dscC.Format, D3DPOOL_DEFAULT, &m_pTex);

	if (FAILED(hr))
		return -1;

	hr = m_pTex->GetSurfaceLevel(0, &m_pSurface);
	m_dwDepthFormat = (D3DFORMAT)dscD.Format;

	return hr;
}

int LcD3D_CreateRenderTarget(char* szCmd, IRenderTarget** pData, void* pdev, int iWidth, int iHeight)
{
	*pData = NULL;

	CRenderTarget* p = new CRenderTarget;

	int iType = 0;

	if (szCmd && 0 == _stricmp("Sphere", szCmd))
		iType = 1;

	if (szCmd && 0 == _stricmp("Cube", szCmd))
		iType = 2;

	if (FAILED(p->Create(pdev, (void*)iWidth, (void*)iHeight, (void*)iType)))
	{
		delete p;
		return -1;
	}

	*pData = p;

	return 0;
}