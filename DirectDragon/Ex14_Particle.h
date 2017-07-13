#pragma once
#include "ParticleSystem.h"

class CEx14_Particle : public CBaseClass
{
	enum{
		en_particle_snow,
		en_particle_firework,
		en_particle_gun
	};

	LPDIRECT3DTEXTURE9		m_pTex;
	LPD3DXMESH					m_pMesh;

	CSnow*	m_pSnow;
	CFireWork* m_pFireWork;
	CParticleGun* m_pParticleGun;

	CCamera*	m_pCamera;

	DWORD	m_dwParticleType;

public:
	CEx14_Particle();
	~CEx14_Particle();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

