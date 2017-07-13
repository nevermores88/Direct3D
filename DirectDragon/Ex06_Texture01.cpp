#include "stdafx.h"
#include "Ex06_Texture01.h"


CEx06_Texture01::CEx06_Texture01()
{
}


CEx06_Texture01::~CEx06_Texture01()
{
}

void CEx06_Texture01::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;
	VB = NULL;
	m_Tex = NULL;
	m_cFilter = '2';
	m_cMipFilter = 'Q';

	m_pdev->CreateVertexBuffer(6 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&VB,
		0);

	Vertex* v;
	VB->Lock(0, 0, (void**)&v, 0);

	v[0] = Vertex(-3.0f, -3.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[1] = Vertex(-3.0f, 3.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = Vertex(3.0f, 3.0f, 1.25f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[3] = Vertex(-3.0f, -3.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[4] = Vertex(3.0f, 3.0f, 1.25f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[5] = Vertex(3.0f, -3.0f, 1.25f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	VB->Unlock();

	D3DXCreateTextureFromFile(m_pdev,
		L"Ex06/dx5_logo.bmp",
		&m_Tex);
}

void CEx06_Texture01::Release()
{

}

void CEx06_Texture01::OnRender()
{
	if (m_pdev)
	{
		m_pdev->SetTexture(0, m_Tex);

		switch (m_cFilter)
		{
		case '1':
			m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
			m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
			break;
		case '2':
			m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			break;
		case '3':
			m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
			m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
			break;
		}

		switch (m_cMipFilter)
		{
		case 'Q':
			m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
			break;
		case 'W':
			m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
			break;
		}

		m_pdev->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 16);

		/*m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);*/

		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);

		m_pdev->SetStreamSource(0, VB, 0, sizeof(Vertex));
		m_pdev->SetFVF(Vertex::FVF);

		m_pdev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
	}
}

void CEx06_Texture01::OnUpdate()
{
	if (m_pdev)
	{
		if (::GetAsyncKeyState('1') & 0x8000f)
		{
			m_cFilter = '1';
		}

		if (::GetAsyncKeyState('2') & 0x8000f)
		{
			m_cFilter = '2';
		}

		if (::GetAsyncKeyState('3') & 0x8000f)
		{
			m_cFilter = '3';
		}

		if (::GetAsyncKeyState('Q') & 0x8000f)
		{
			m_cMipFilter = 'Q';
		}

		if (::GetAsyncKeyState('W') & 0x8000f)
		{
			m_cMipFilter = 'W';
		}
	}
}
