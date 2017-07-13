#include "stdafx.h"
#include "BoundingBox.h"


CBoundingBox::CBoundingBox()
{
}


CBoundingBox::~CBoundingBox()
{
}

bool CBoundingBox::IsPointInside(D3DXVECTOR3& p)
{
	if (p.x >= m_vMin.x && p.y >= m_vMin.y && p.z >= m_vMin.z &&
		p.x <= m_vMax.x && p.y <= m_vMax.x && p.z <= m_vMax.z)
		return true;
	else
		return false;
}

bool CBoundingBox::ComputeBoundingBox(LPD3DXMESH pMesh)
{
	HRESULT hr = 0;
	
	BYTE* v = 0;
	pMesh->LockVertexBuffer(0, (void**)&v);
	DWORD dw1 = pMesh->GetFVF();
	DWORD dw2 = D3DXGetFVFVertexSize(dw1);
	hr = D3DXComputeBoundingBox(
		(D3DXVECTOR3*)v,
		pMesh->GetNumVertices(),
		D3DXGetFVFVertexSize(pMesh->GetFVF()),
		&m_vMin, &m_vMax);

	pMesh->UnlockVertexBuffer();

	if (FAILED(hr))
		return false;

	return true;
}