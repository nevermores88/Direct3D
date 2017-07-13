#include "stdafx.h"
#include "Ex12_Camera.h"


CEx12_Camera::CEx12_Camera()
{
}


CEx12_Camera::~CEx12_Camera()
{
}

void CEx12_Camera::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;

	m_pFloorVB = NULL;
	m_pTex = NULL;
	m_pPillarMesh = NULL;

	m_Camera.Create(CCamera::LANDOBJECT);
}

void CEx12_Camera::Release()
{
	
}

void CEx12_Camera::OnRender()
{
	if (m_pdev)
	{
		D3DXMATRIX mat;
		m_Camera.GetViewMatrix(&mat);
		m_pdev->SetTransform(D3DTS_VIEW, &mat);
		DrawBasicScene();
	}
}

void CEx12_Camera::OnUpdate()
{
	if (m_pdev)
	{
		float fTimeDelta = g_pGame->GetManager()->GetTimeManager()->GetTimeDelta();

		if (::GetAsyncKeyState(VK_UP) & 0x8000f)
			m_Camera.Pitch(-1.0f * fTimeDelta);

		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
			m_Camera.Pitch(1.0f * fTimeDelta);

		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
			m_Camera.Yaw(-1.0f * fTimeDelta);

		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
			m_Camera.Yaw(1.0f * fTimeDelta);

		if (::GetAsyncKeyState('N') & 0x8000f)
			m_Camera.Roll(1.0f * fTimeDelta);

		if (::GetAsyncKeyState('M') & 0x8000f)
			m_Camera.Roll(-1.0f * fTimeDelta);

		if (::GetAsyncKeyState('W') & 0x8000f)
			m_Camera.Walk(4.0f * fTimeDelta);

		if (::GetAsyncKeyState('S') & 0x8000f)
			m_Camera.Walk(-4.0f * fTimeDelta);

		if (::GetAsyncKeyState('A') & 0x8000f)
			m_Camera.Strafe(-4.0f * fTimeDelta);

		if (::GetAsyncKeyState('D') & 0x8000f)
			m_Camera.Strafe(4.0f * fTimeDelta);

		if (::GetAsyncKeyState('R') & 0x8000f)
			m_Camera.Fly(4.0f * fTimeDelta);

		if (::GetAsyncKeyState('F') & 0x8000f)
			m_Camera.Fly(-4.0f * fTimeDelta);
	}
}

void CEx12_Camera::DrawBasicScene()
{
	if (m_pdev)
	{
		HRESULT hr = 0;

		if (!m_pFloorVB && !m_pTex && !m_pPillarMesh)
		{
			m_pdev->CreateVertexBuffer(
				6 * sizeof(Vertex),
				0,
				Vertex::FVF,
				D3DPOOL_MANAGED,
				&m_pFloorVB,
				0);

			Vertex* v = 0;
			m_pFloorVB->Lock(0, 0, (void**)&v, 0);

			v[0] = Vertex(-20.0f, -2.5f, -20.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
			v[1] = Vertex(-20.0f, -2.5f, 20.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			v[2] = Vertex(20.0f, -2.5f, 20.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

			v[3] = Vertex(-20.0f, -2.5f, -20.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
			v[4] = Vertex(20.0f, -2.5f, 20.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
			v[5] = Vertex(20.0f, -2.5f, -20.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

			m_pFloorVB->Unlock();

			D3DXCreateCylinder(m_pdev, 0.5f, 0.5f, 5.0f, 20, 20, &m_pPillarMesh, 0);

			D3DXCreateTextureFromFile(m_pdev, L"Ex12/desert.bmp", &m_pTex);
		}
		else
		{
			m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

			D3DXVECTOR3 dir(0.707f, -0.707f, 0.707f);
			D3DXCOLOR col(1.0f, 1.0f, 1.0f, 1.0f);
			D3DLIGHT9 light = d3d::InitDirectionalLight(&dir, &col);

			m_pdev->SetLight(0, &light);
			m_pdev->LightEnable(0, true);
			m_pdev->SetRenderState(D3DRS_NORMALIZENORMALS, true);
			m_pdev->SetRenderState(D3DRS_SPECULARENABLE, true);

			D3DXMATRIX T, R, P, S;

			D3DXMatrixRotationX(&R, -D3DX_PI*0.5f);

			D3DXMatrixIdentity(&T);

			m_pdev->SetTransform(D3DTS_WORLD, &T);
			m_pdev->SetMaterial(&d3d::WHITE_MTRL);
			m_pdev->SetTexture(0, m_pTex);
			m_pdev->SetStreamSource(0, m_pFloorVB, 0, sizeof(Vertex));
			m_pdev->SetFVF(Vertex::FVF);
			m_pdev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

			m_pdev->SetMaterial(&d3d::BLUE_MTRL);
			m_pdev->SetTexture(0, 0);

			for (int i = 0; i < 5; i++)
			{
				D3DXMatrixTranslation(&T, -5.0f, 0.0f, -15.0f + i*(7.5f));
				P = R*T;
				m_pdev->SetTransform(D3DTS_WORLD, &P);
				m_pPillarMesh->DrawSubset(0);

				D3DXMatrixTranslation(&T, 5.0f, 0.0f, -15.0f + (i * 7.5f));
				P = R*T;
				m_pdev->SetTransform(D3DTS_WORLD, &P);
				m_pPillarMesh->DrawSubset(0);
			}
		}
	}
}
