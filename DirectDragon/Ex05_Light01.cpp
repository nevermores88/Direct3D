#include "stdafx.h"
#include "Ex05_Light01.h"


CEx05_Light01::CEx05_Light01()
{
}


CEx05_Light01::~CEx05_Light01()
{
}

void CEx05_Light01::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;
	VB = NULL;

	m_pdev->CreateVertexBuffer(12 * sizeof(NomalVertex01),
		D3DUSAGE_WRITEONLY,
		NomalVertex01::FVF,
		D3DPOOL_MANAGED,
		&VB,
		0);

	NomalVertex01* v;
	VB->Lock(0, 0, (void**)&v, 0);

	// front face
	v[0] = NomalVertex01(-1.0f, 0.0f, -1.0f, 0.0f, 0.707f, -0.707f);
	v[1] = NomalVertex01(0.0f, 1.0f, 0.0f, 0.0f, 0.707f, -0.707f);
	v[2] = NomalVertex01(1.0f, 0.0f, -1.0f, 0.0f, 0.707f, -0.707f);

	// left face
	v[3] = NomalVertex01(-1.0f, 0.0f, 1.0f, -0.707f, 0.707f, 0.0f);
	v[4] = NomalVertex01(0.0f, 1.0f, 0.0f, -0.707f, 0.707f, 0.0f);
	v[5] = NomalVertex01(-1.0f, 0.0f, -1.0f, -0.707f, 0.707f, 0.0f);

	// right face
	v[6] = NomalVertex01(1.0f, 0.0f, -1.0f, 0.707f, 0.707f, 0.0f);
	v[7] = NomalVertex01(0.0f, 1.0f, 0.0f, 0.707f, 0.707f, 0.0f);
	v[8] = NomalVertex01(1.0f, 0.0f, 1.0f, 0.707f, 0.707f, 0.0f);

	// back face
	v[9] = NomalVertex01(1.0f, 0.0f, 1.0f, 0.0f, 0.707f, 0.707f);
	v[10] = NomalVertex01(0.0f, 1.0f, 0.0f, 0.0f, 0.707f, 0.707f);
	v[11] = NomalVertex01(-1.0f, 0.0f, 1.0f, 0.0f, 0.707f, 0.707f);

	VB->Unlock();

}

void CEx05_Light01::Release()
{
	m_pdev = NULL;

	ZeroMemory(&m_WorldMat, sizeof(m_WorldMat));
	VB->Release();

	ZeroMemory(&m_Mtrl, sizeof(m_Mtrl));
	ZeroMemory(&m_Light, sizeof(m_Light));
}

void CEx05_Light01::OnRender()
{
	if (m_pdev)
	{
		m_pdev->SetRenderState(D3DRS_LIGHTING, TRUE);

		m_pdev->SetMaterial(&m_Mtrl);

		m_pdev->SetLight(0, &m_Light);
		m_pdev->LightEnable(0, TRUE);

		m_pdev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		m_pdev->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

		m_pdev->SetTransform(D3DTS_WORLD, &m_WorldMat);

		m_pdev->SetFVF(NomalVertex01::FVF);
		m_pdev->SetStreamSource(0, VB, 0, sizeof(NomalVertex01));
		m_pdev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 4);
	}
}

void CEx05_Light01::OnUpdate()
{
	if (m_pdev)
	{
		static float y = 0.0f;

		D3DXMatrixRotationY(&m_WorldMat, y);
		y += 0.01f;

		if (y >= 6.28f)
			y = 0.0f;

		m_Mtrl.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f)*0.6f;
		m_Mtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		m_Mtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f)*0.3f;
		m_Mtrl.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
		m_Mtrl.Power = 5.0f;

		ZeroMemory(&m_Light, sizeof(m_Light));
		m_Light.Type = D3DLIGHT_DIRECTIONAL;
		m_Light.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		m_Light.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		m_Light.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		m_Light.Direction = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	}
}
