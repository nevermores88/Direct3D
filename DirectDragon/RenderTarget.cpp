#include "stdafx.h"
#include "RenderTarget.h"


CRenderTarget::CRenderTarget()
{
	m_nType = 0;
	m_iW = -1;
	m_iH = -1;
	m_dD = 0xFFFFFFFF;

	m_pDev = NULL;

	m_pRts = NULL;

	m_pTxP = NULL;
	m_pSfc = NULL;
}


CRenderTarget::~CRenderTarget()
{
	Destroy();
}

INT CRenderTarget::Create(void* p1, void* p2, void* p3, void* p4)
{
	m_pDev = (LPDIRECT3DDEVICE9)p1;
	m_iW = (INT)p2;
	m_iH = (INT)p3;
	m_nType = (INT)p4;

	return CreateRenderSurface();
}



INT CRenderTarget::CreateRenderSurface()
{
	HRESULT hr = -1;

	DWORD				dMip = 1;
	LPDIRECT3DSURFACE9	pSfC = NULL;
	LPDIRECT3DSURFACE9	pSfD = NULL;
	D3DSURFACE_DESC		dscC;
	D3DSURFACE_DESC		dscD;

	D3DCAPS9			m_Caps;

	m_pDev->GetRenderTarget(0, &pSfC);
	m_pDev->GetDepthStencilSurface(&pSfD);

	pSfC->GetDesc(&dscC);
	pSfD->GetDesc(&dscD);

	m_pDev->GetDeviceCaps(&m_Caps);

	pSfC->Release();
	pSfD->Release();

	if (m_iW < 0)
		m_iW = dscC.Width;

	if (m_iH < 0)
		m_iH = dscC.Height;

	hr = D3DXCreateRenderToSurface(m_pDev
		, m_iW
		, m_iH
		, dscC.Format
		, TRUE
		, dscD.Format
		, &m_pRts);

	hr = D3DXCreateTexture(m_pDev
		, m_iW
		, m_iH
		, dMip
		, D3DUSAGE_RENDERTARGET
		, dscC.Format
		, D3DPOOL_DEFAULT
		, &m_pTxP);

	if (FAILED(hr))
		return -1;


	hr = m_pTxP->GetSurfaceLevel(0, &m_pSfc);
	m_dD = (D3DFORMAT)dscD.Format;

	return hr;
}


void CRenderTarget::Destroy()
{
	/*SAFE_RELEASE(m_pSfc);
	SAFE_RELEASE(m_pTxP);
	SAFE_RELEASE(m_pRts);*/
}


INT CRenderTarget::OnResetDevice()
{
	if (NULL == m_pRts)
		return CreateRenderSurface();

	return 0;
}

INT CRenderTarget::OnLostDevice()
{
	Destroy();

	return 0;
}


INT CRenderTarget::BeginScene(DWORD dClearMode, DWORD dClearColor)
{
	HRESULT hr = -1;

	hr = m_pRts->BeginScene(m_pSfc, NULL);

	if (0xFFFFFFFF != dClearMode)
		hr = m_pDev->Clear(0L, NULL, dClearMode, dClearColor, 1.0f, 0L);

	return hr;
}


INT CRenderTarget::EndScene()
{
	HRESULT hr = -1;

	if (0 == m_nType)
		hr = m_pRts->EndScene(0);

	return hr;
}





INT CRenderTarget::GetWidth()
{
	return m_iW;
}

INT CRenderTarget::GetHeight()
{
	return m_iH;
}


DWORD CRenderTarget::GetDepth()
{
	return m_dD;
}


void* CRenderTarget::GetTexture() const
{
	return m_pTxP;
}


void* CRenderTarget::GetSurface() const
{
	return m_pSfc;
}





INT LcD3D_CreateRenderTarget(char* sCmd, IRenderTarget** pData, void* pDevice, INT iWidth, INT iHeight)
{
	*pData = NULL;

	CRenderTarget*	p = new CRenderTarget;

	INT	nType = 0;

	if (sCmd && 0 == _stricmp("Sphere", sCmd))
		nType = 1;

	if (sCmd && 0 == _stricmp("Cube", sCmd))
		nType = 2;


	if (FAILED(p->Create(pDevice, (void*)iWidth, (void*)iHeight, (void*)nType)))
	{
		delete p;
		return -1;
	}

	*pData = p;

	return 0;
}