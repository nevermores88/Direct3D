#include "stdafx.h"
#include "BoundingSphere.h"


CBoundingSphere::CBoundingSphere()
{
}


CBoundingSphere::~CBoundingSphere()
{
}

bool CBoundingSphere::ComputeBoundingSphere(LPD3DXMESH pMesh)
{
	HRESULT hr = 0;

	BYTE*	v = 0;
	pMesh->LockVertexBuffer(0, (void**)&v);

	hr = D3DXComputeBoundingSphere(
		(D3DXVECTOR3*)v,
		pMesh->GetNumVertices(),
		D3DXGetFVFVertexSize(pMesh->GetFVF()),
		&m_v3Center,
		&m_fRadius);

	pMesh->UnlockVertexBuffer();

	if (FAILED(hr))
		return false;
	
	return true;
}
