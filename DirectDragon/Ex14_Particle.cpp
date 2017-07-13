#include "stdafx.h"
#include "Ex14_Particle.h"
#include <time.h>

CEx14_Particle::CEx14_Particle()
{
}


CEx14_Particle::~CEx14_Particle()
{
}

void CEx14_Particle::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;

	m_pCamera = new CCamera();

	if (!m_pCamera)
		return;

	m_pCamera->Create(CCamera::LANDOBJECT);

	srand((unsigned int)time(0));

	m_dwParticleType = en_particle_gun;

	switch (m_dwParticleType)
	{
	case en_particle_snow:
	{
		CBoundingBox boundingBox;
		boundingBox.m_vMin = D3DXVECTOR3(-100.0f, -100.0f, -100.0f);
		boundingBox.m_vMax = D3DXVECTOR3(100.0f, 100.0f, 100.0f);
		m_pSnow = new CSnow(&boundingBox, 10000);
		m_pSnow->Init(m_pdev, L"Ex14/snowflake.dds");
	}
	break;
	case en_particle_firework:
	{
		D3DXVECTOR3 origin(0.0f, 10.0f, 5.0f);
		m_pFireWork = new CFireWork(&origin, 6000);
		m_pFireWork->Init(m_pdev, L"Ex14/flare.bmp");
		//m_pFireWork->Init(m_pdev, L"Ex14/snowflake.dds");
	}
	break;
	case en_particle_gun:
	{
		m_pParticleGun = new CParticleGun(m_pCamera);
		m_pParticleGun->Init(m_pdev, L"Ex14/flare_alpha.dds");
	}
	break;
	}
}

void CEx14_Particle::Release()
{

}

void CEx14_Particle::OnRender()
{
	if (m_pdev)
	{
		switch (m_dwParticleType)
		{
		case en_particle_snow:
		{
			m_pSnow->Render();
		}
		break;
		case en_particle_firework:
		{
			m_pFireWork->Render();
		}
		break;
		case en_particle_gun:
		{
			m_pParticleGun->Render();
		}
		break;
		}
	}
}

void CEx14_Particle::OnUpdate()
{
	if (m_pdev)
	{
		float fTimeDelta = g_pGame->GetManager()->GetTimeManager()->GetTimeDelta();

		if (::GetAsyncKeyState('W') & 0x8000f)
			m_pCamera->Walk(30.0f * fTimeDelta);

		if (::GetAsyncKeyState('S') & 0x8000f)
			m_pCamera->Walk(-30.0f * fTimeDelta);

		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
			m_pCamera->Yaw(-1.0f * fTimeDelta);

		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
			m_pCamera->Yaw(1.0f * fTimeDelta);

		if (::GetAsyncKeyState('A') & 0x8000f)
			m_pCamera->Strafe(-30.0f * fTimeDelta);

		if (::GetAsyncKeyState('D') & 0x8000f)
			m_pCamera->Strafe(30.0f * fTimeDelta);

		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
			m_pCamera->Pitch(1.0f * fTimeDelta);

		if (::GetAsyncKeyState(VK_UP) & 0x8000f)
			m_pCamera->Pitch(-1.0f * fTimeDelta);

		if (::GetAsyncKeyState(VK_SPACE) & 0x8000f)
		{
			static float fInterval = 0.0f;
			fInterval += fTimeDelta;

			if (fInterval > 0.1f)
			{
				m_pParticleGun->AddParticle();
				fInterval = 0.0f;
			}
		}

		D3DXMATRIX	mtView;
		m_pCamera->GetViewMatrix(&mtView);
		m_pdev->SetTransform(D3DTS_VIEW, &mtView);

		switch (m_dwParticleType)
		{
		case en_particle_snow:
		{
			m_pSnow->Update(fTimeDelta);
		}
		break;
		case en_particle_firework:
		{
			m_pFireWork->Update(fTimeDelta);
			
			if (m_pFireWork->IsDead())
				m_pFireWork->Reset();
		}
		break;
		case en_particle_gun:
		{
			m_pParticleGun->Update(fTimeDelta);
		}
		break;
		}
		
	}
}
