#include "stdafx.h"
#include "Ex06_AddressModes.h"


CEx06_AddressModes::CEx06_AddressModes()
{
}


CEx06_AddressModes::~CEx06_AddressModes()
{
}

void CEx06_AddressModes::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;

	m_pdev->CreateVertexBuffer(
		6 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&m_pVB, 0);

	Vertex* v;
	m_pVB->Lock(0, 0, (void**)&v, 0);
	v[0] = Vertex(-1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 3.0f);
	v[1] = Vertex(-1.0f, 1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = Vertex(1.0f, 1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 3.0f, 0.0f);

	v[3] = Vertex(-1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 3.0f);
	v[4] = Vertex(1.0f, 1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 3.0f, 0.0f);
	v[5] = Vertex(1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 3.0f, 3.0f);

	m_pVB->Unlock();

	D3DXCreateTextureFromFile(m_pdev, L"Ex06/dx5_logo.bmp", &m_pTex);

}

void CEx06_AddressModes::Release()
{

}

void CEx06_AddressModes::OnRender()
{
	if (m_pdev)
	{
		m_pdev->SetTexture(0, m_pTex);

		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		m_pdev->SetRenderState(D3DRS_LIGHTING, false);

		m_pdev->SetStreamSource(0, m_pVB, 0, sizeof(Vertex));
		m_pdev->SetFVF(Vertex::FVF);
		m_pdev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
	}
}

void CEx06_AddressModes::OnUpdate()
{
	if (m_pdev)
	{
		// set wrap address mode
		if (::GetAsyncKeyState('W') & 0x8000f)
		{
			m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
			m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		}

		// set border color address mode
		if (::GetAsyncKeyState('B') & 0x8000f)
		{
			m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
			m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
			m_pdev->SetSamplerState(0, D3DSAMP_BORDERCOLOR, 0x000000ff);
		}

		// set clamp address mode
		if (::GetAsyncKeyState('C') & 0x8000f)
		{
			m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
			m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		}

		// set mirror address mode
		if (::GetAsyncKeyState('M') & 0x8000f)
		{
			m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
			m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
		}
	}
}
