#include "stdafx.h"
#include "ParticleSystem.h"


CParticleSystem::CParticleSystem()
{
}


CParticleSystem::~CParticleSystem()
{
}

bool CParticleSystem::Init(LPDIRECT3DDEVICE9 pdev, wchar_t* wsTexFileName)
{
	m_pdev = pdev;

	HRESULT hr = 0;

	//동적버퍼에 저장하며 D3DUSAGE_POINTS사용, 동적버퍼에서는 D3DPOOL_MANAGED 사용못함
	hr = m_pdev->CreateVertexBuffer(
		m_dwVBSize*sizeof(Particle),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		Particle::FVF,
		D3DPOOL_DEFAULT,
		&m_pVB, 0);

	if (FAILED(hr))
	{
		::MessageBox(0, L"CreateVertexBuffer() - FAILED", L"PSystem", 0);
		return false;
	}

	hr = D3DXCreateTextureFromFile(
		m_pdev, wsTexFileName, &m_pTex);

	if (FAILED(hr))
	{
		::MessageBox(0, L"D3DXCreateTextureFromFile() - FAILED", L"PSystem", 0);
		return false;
	}

	return true;
}

void CParticleSystem::Reset()
{
	std::list<Attribute>::iterator itr;

	for (itr = m_Particles.begin(); itr != m_Particles.end(); ++itr)
		ResetParticle(&(*itr));
}

void CParticleSystem::AddParticle()
{
	Attribute attr;

	ResetParticle(&attr);

	m_Particles.push_back(attr);
}

void CParticleSystem::PreRender()
{
	m_pdev->SetRenderState(D3DRS_LIGHTING, false);
	m_pdev->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	m_pdev->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	m_pdev->SetRenderState(D3DRS_POINTSIZE, d3d::FtoDw(m_fSize));
	m_pdev->SetRenderState(D3DRS_POINTSIZE_MIN, d3d::FtoDw(0.0f));

	m_pdev->SetRenderState(D3DRS_POINTSCALE_A, d3d::FtoDw(0.0f));
	m_pdev->SetRenderState(D3DRS_POINTSCALE_B, d3d::FtoDw(0.0f));
	m_pdev->SetRenderState(D3DRS_POINTSCALE_C, d3d::FtoDw(1.0f));

	m_pdev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pdev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pdev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pdev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CParticleSystem::Render()
{
	if (!m_Particles.empty())
	{
		PreRender();

		m_pdev->SetTexture(0, m_pTex);
		m_pdev->SetFVF(Particle::FVF);
		m_pdev->SetStreamSource(0, m_pVB, 0, sizeof(Particle));

		if (m_dwVBOffset >= m_dwVBSize)
			m_dwVBOffset = 0;

		Particle* v = NULL;

		m_pVB->Lock(
			m_dwVBOffset * sizeof(Particle),
			m_dwVBBatchSize*sizeof(Particle),
			(void**)&v,
			m_dwVBOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

		DWORD dwNumParticlesInBatch = 0;

		std::list<Attribute>::iterator itr;

		for (itr = m_Particles.begin(); itr != m_Particles.end(); ++itr)
		{
			if (itr->m_bAlive)
			{
				v->m_Pos = itr->m_Pos;
				v->m_Color = itr->m_Color;
				v++;

				dwNumParticlesInBatch++;

				if (dwNumParticlesInBatch == m_dwVBBatchSize)
				{
					m_pVB->Unlock();

					m_pdev->DrawPrimitive(
						D3DPT_POINTLIST,
						m_dwVBOffset,
						m_dwVBBatchSize);

					m_dwVBOffset += m_dwVBBatchSize;

					if (m_dwVBOffset >= m_dwVBSize)
						m_dwVBOffset = 0;

					m_pVB->Lock(
						m_dwVBOffset*sizeof(Particle),
						m_dwVBBatchSize*sizeof(Particle),
						(void**)&v,
						m_dwVBOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

					dwNumParticlesInBatch = 0;
				}
			}
		}

		m_pVB->Unlock();

		//파티클이 남아있으면 마저 그린다.
		if (dwNumParticlesInBatch)
		{
			m_pdev->DrawPrimitive(
				D3DPT_POINTLIST,
				m_dwVBOffset,
				dwNumParticlesInBatch);
		}

		m_dwVBOffset += m_dwVBBatchSize;

		PostRender();
	}
}

void CParticleSystem::PostRender()
{
	m_pdev->SetRenderState(D3DRS_LIGHTING, true);
	m_pdev->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	m_pdev->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

bool CParticleSystem::IsEmpty()
{
	return m_Particles.empty();
}

bool CParticleSystem::IsDead()
{
	std::list<Attribute>::iterator itr;

	for (itr = m_Particles.begin(); itr != m_Particles.end(); ++itr)
	{
		if (itr->m_bAlive)
			return false;
	}

	return true;
}

void CParticleSystem::RemoveDeadParticles()
{
	std::list<Attribute>::iterator itr;

	itr = m_Particles.begin();

	while (itr != m_Particles.end())
	{
		if (itr->m_bAlive == false)
		{
			itr = m_Particles.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}

CSnow::CSnow(CBoundingBox* pBoundingBox, int nNumParticles)
{
	m_BoundingBox = *pBoundingBox;
	m_fSize = 1.0f;
	m_dwVBSize = 2048;
	m_dwVBOffset = 0;
	m_dwVBBatchSize = 512;

	for (int i = 0; i < nNumParticles; i++)
		AddParticle();
}

void CSnow::ResetParticle(Attribute* pAttribute)
{
	pAttribute->m_bAlive = true;

	d3d::GetRandomVector(
		&pAttribute->m_Pos,
		&m_BoundingBox.m_vMin,
		&m_BoundingBox.m_vMax);

	pAttribute->m_Pos.y = m_BoundingBox.m_vMax.y;

	pAttribute->m_Velocity.x = d3d::GetRandomFloat(0.0f, 1.0f) * -3.0f;
	pAttribute->m_Velocity.y = d3d::GetRandomFloat(0.0f, 1.0f) * -10.0f;
	pAttribute->m_Velocity.z = 0.0f;

	pAttribute->m_Color = d3d::WHITE;
}

void CSnow::Update(float fTimeDelta)
{
	std::list<Attribute>::iterator itr;

	for (itr = m_Particles.begin(); itr != m_Particles.end(); ++itr)
	{
		itr->m_Pos += itr->m_Velocity*fTimeDelta;

		if (m_BoundingBox.IsPointInside(itr->m_Pos) == false)
			ResetParticle(&(*itr));
	}
}

void CSnow::PreRender()
{
	CParticleSystem::PreRender();

	m_pdev->SetRenderState(D3DRS_ZWRITEENABLE, false);

	//m_pdev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	//m_pdev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
}

void CSnow::PostRender()
{
	CParticleSystem::PostRender();

	m_pdev->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

CFireWork::CFireWork(D3DXVECTOR3* pOrigin, int nNumParticles)
{
	m_Origin = *pOrigin;
	m_fSize = 0.9f;
	m_dwVBSize = 2048;
	m_dwVBOffset = 0;
	m_dwVBBatchSize = 512;

	for (int i = 0; i < nNumParticles; i++)
		AddParticle();
}

void CFireWork::ResetParticle(Attribute* pAttribute)
{
	pAttribute->m_bAlive = true;
	pAttribute->m_Pos = m_Origin;

	D3DXVECTOR3 min = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	D3DXVECTOR3 max = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	d3d::GetRandomVector(
		&pAttribute->m_Velocity,
		&min, &max);

	D3DXVec3Normalize(
		&pAttribute->m_Velocity,
		&pAttribute->m_Velocity);

	pAttribute->m_Velocity *= 50.0f;

	pAttribute->m_Color = D3DXCOLOR(
		d3d::GetRandomFloat(0.0f, 1.0f),
		d3d::GetRandomFloat(0.0f, 1.0f),
		d3d::GetRandomFloat(0.0f, 1.0f),
		1.0f);

	pAttribute->m_fAge = 0.0f;
	pAttribute->m_fLifeTime = 2.0f;
}

void CFireWork::Update(float fTimeDelta)
{
	std::list<Attribute>::iterator itr;

	for (itr = m_Particles.begin(); itr != m_Particles.end(); ++itr)
	{
		if (itr->m_bAlive)
		{
			itr->m_Pos += itr->m_Velocity*fTimeDelta;

			itr->m_fAge += fTimeDelta;

			if (itr->m_fAge > itr->m_fLifeTime)
				itr->m_bAlive = false;
		}
	}
}

void CFireWork::PreRender()
{
	CParticleSystem::PreRender();

	m_pdev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pdev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	m_pdev->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void CFireWork::PostRender()
{
	CParticleSystem::PostRender();

	m_pdev->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

CParticleGun::CParticleGun(CCamera* pCamera)
{
	m_pCamera = pCamera;
	m_fSize = 0.8f;
	m_dwVBSize = 2048;
	m_dwVBOffset = 0;
	m_dwVBBatchSize = 512;
}

void CParticleGun::ResetParticle(Attribute* pAttribute)
{
	pAttribute->m_bAlive = true;

	D3DXVECTOR3 camPos;

	m_pCamera->GetPosition(&camPos);

	D3DXVECTOR3 camDir;
	m_pCamera->GetLook(&camDir);

	pAttribute->m_Pos = camPos;
	pAttribute->m_Pos.y -= 1.0f;

	pAttribute->m_Velocity = camDir * 100.0f;

	pAttribute->m_Color = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

	pAttribute->m_fAge = 0.0f;
	pAttribute->m_fLifeTime = 2.0f;
}

void CParticleGun::Update(float fTimeDelta)
{
	std::list<Attribute>::iterator itr;

	for (itr = m_Particles.begin(); itr != m_Particles.end(); ++itr)
	{
		itr->m_Pos += itr->m_Velocity * fTimeDelta;
		D3DXVECTOR3 pos = itr->m_Pos;
		itr->m_fAge += fTimeDelta;

		if (itr->m_fAge > itr->m_fLifeTime)
			itr->m_bAlive = false;
	}

	RemoveDeadParticles();
}

void CParticleGun::PreRender()
{
	CParticleSystem::PreRender();
}

void CParticleGun::PostRender()
{
	CParticleSystem::PostRender();
}
