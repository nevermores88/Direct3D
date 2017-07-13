#pragma once
#include "BoundingBox.h"
#include <list>

struct Particle
{
	D3DXVECTOR3 m_Pos;
	D3DCOLOR		 m_Color;
	static const DWORD FVF;
};

const DWORD Particle::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

struct Attribute
{
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_Velocity;
	D3DXVECTOR3 m_Acceleration;

	float m_fLifeTime;
	float m_fAge;

	D3DXCOLOR m_Color;
	D3DXCOLOR m_ColorFade;
	
	bool m_bAlive;

	Attribute()
	{
		ZeroMemory(&m_Pos, sizeof(m_Pos));
		ZeroMemory(&m_Velocity, sizeof(m_Velocity));
		ZeroMemory(&m_Acceleration, sizeof(m_Acceleration));

		m_fLifeTime = 0.f;
		m_fAge = 0.f;

		ZeroMemory(&m_Color, sizeof(m_Color));
		ZeroMemory(&m_ColorFade, sizeof(m_ColorFade));

		m_bAlive = FALSE;
	}
};

class CParticleSystem
{
protected:
	LPDIRECT3DDEVICE9	m_pdev;
	D3DXVECTOR3			m_Origin;
	CBoundingBox				m_BoundingBox;

	LPDIRECT3DTEXTURE9				m_pTex;
	LPDIRECT3DVERTEXBUFFER9	m_pVB;

	std::list<Attribute>		m_Particles;

	float	m_fEmitRate;
	float  m_fSize;

	int		m_nMaxParticles;

	DWORD	m_dwVBSize;
	DWORD	m_dwVBOffset;
	DWORD	m_dwVBBatchSize;

protected:
	virtual void RemoveDeadParticles();

public:
	CParticleSystem();
	virtual ~CParticleSystem();

	virtual bool Init(LPDIRECT3DDEVICE9 pdev, wchar_t* wsTexFileName);
	virtual void Reset();

	virtual void ResetParticle(Attribute* pAttribute) = 0;
	virtual void AddParticle();

	virtual void Update(float fTimeDelta) = 0;

	virtual void PreRender();
	virtual void Render();
	virtual void PostRender();

	bool IsEmpty();
	bool IsDead();
};

class CSnow : public CParticleSystem
{
public:
	CSnow(CBoundingBox* pBoundingBox, int nNumParticles);
	void ResetParticle(Attribute* pAttribute);
	void Update(float fTimeDelta);
	void PreRender();
	void PostRender();
};

class CFireWork : public CParticleSystem
{
public:
	CFireWork(D3DXVECTOR3* pOrigin, int nNumParticles);
	void ResetParticle(Attribute* pAttribute);
	void Update(float fTimeDelta);
	void PreRender();
	void PostRender();
};

class CParticleGun : public CParticleSystem
{
private:
	CCamera*	m_pCamera;
public:
	CParticleGun(CCamera* pCamera);
	void	ResetParticle(Attribute* pAttribute);
	void	Update(float fTimeDelta);
	void PreRender();
	void PostRender();
};