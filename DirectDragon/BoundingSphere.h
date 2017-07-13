#pragma once
class CBoundingSphere
{
public:
	D3DXVECTOR3	m_v3Center;
	float					m_fRadius;

public:
	CBoundingSphere();
	~CBoundingSphere();

	bool ComputeBoundingSphere(LPD3DXMESH pMesh);
};

