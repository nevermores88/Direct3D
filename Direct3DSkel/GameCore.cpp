#include "stdafx.h"
#include "GameCore.h"


CGameCore::CGameCore()
{
}


CGameCore::~CGameCore()
{
}

HRESULT CGameCore::Create(HWND hWnd)
{
	m_hWnd = hWnd;

	if (NULL == (m_pd3d = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES;
	d3dpp.MultiSampleQuality = 0;

	D3DCAPS9 caps;
	m_pd3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	DWORD vp = 0;

	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	if (FAILED(m_pd3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		hWnd, vp, &d3dpp, &m_pdev)))
	{
		return E_FAIL;
	}

	m_pdev->SetRenderState(D3DRS_ZENABLE, TRUE);

	return S_OK;
}

void CGameCore::Release()
{
	if (m_pdev != NULL)
		m_pdev->Release();

	if (m_pd3d != NULL)
		m_pd3d->Release();
}


