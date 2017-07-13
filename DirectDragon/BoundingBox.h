#pragma once
class CBoundingBox
{
public:
	D3DXVECTOR3	m_vMin;
	D3DXVECTOR3	m_vMax;

public:
	CBoundingBox();
	~CBoundingBox();

	bool IsPointInside(D3DXVECTOR3& p);
	bool ComputeBoundingBox(LPD3DXMESH pMesh);
};

