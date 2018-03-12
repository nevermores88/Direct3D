#pragma once

#include "BoundingSphere.h"

class CEx15_Picking : public CBaseClass
{
	LPD3DXMESH m_pTeapot;
	LPD3DXMESH m_pSphere;

	CBoundingSphere m_BoundingSphere;

public:
	CEx15_Picking();
	~CEx15_Picking();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();

private:
	d3d::Ray CalculatePickingRay(int x, int y);
	void TransformRay(d3d::Ray* ray, D3DXMATRIX* mat);
	bool RaySphereIntersectionTest(d3d::Ray* ray, CBoundingSphere* pSphere);
};

