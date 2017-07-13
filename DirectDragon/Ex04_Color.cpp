#include "stdafx.h"
#include "Ex04_Color.h"


CEx04_Color::CEx04_Color()
{
}


CEx04_Color::~CEx04_Color()
{
}

void CEx04_Color::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;
	memset(m_WorldMat, 0, sizeof(m_WorldMat));
	VB = NULL;

	m_pdev->CreateVertexBuffer(3 * sizeof(ColorVertex),
		D3DUSAGE_WRITEONLY,
		ColorVertex::FVF,
		D3DPOOL_MANAGED,
		&VB,
		0);

	ColorVertex* v;
	VB->Lock(0, 0, (void**)&v, 0);

	v[0] = ColorVertex(-1.0f, 0.0f, 2.0f, D3DCOLOR_XRGB(255, 0, 0));
	v[1] = ColorVertex(0.0f, 1.0f, 2.0f, D3DCOLOR_XRGB(0, 255, 0));
	v[2] = ColorVertex(1.0f, 0.0f, 2.0f, D3DCOLOR_XRGB(0, 0, 255));

	VB->Unlock();
}

void CEx04_Color::Release()
{
	m_pdev = NULL;
	memset(m_WorldMat, 0, sizeof(m_WorldMat));
	VB->Release();
}

void CEx04_Color::OnRender()
{
	if (m_pdev)
	{
		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);

		m_pdev->SetFVF(ColorVertex::FVF);
		m_pdev->SetStreamSource(0, VB, 0, sizeof(ColorVertex));

		D3DXMatrixTranslation(&m_WorldMat, -1.25f, 0.0f, -2.0f);
		m_pdev->SetTransform(D3DTS_WORLD, &m_WorldMat);

		m_pdev->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
		m_pdev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

		D3DXMatrixTranslation(&m_WorldMat, -1.f, 0.0f, 0.0f);
		m_pdev->SetTransform(D3DTS_WORLD, &m_WorldMat);

		m_pdev->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
		m_pdev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

		
	}
}

void CEx04_Color::OnUpdate()
{
	if (m_pdev)
	{
	}
}
