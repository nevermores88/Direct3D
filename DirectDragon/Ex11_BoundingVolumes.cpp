#include "stdafx.h"
#include "Ex11_BoundingVolumes.h"


CEx11_BoundingVolumes::CEx11_BoundingVolumes()
{
}


CEx11_BoundingVolumes::~CEx11_BoundingVolumes()
{
}

void CEx11_BoundingVolumes::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;
	m_fYRot = 0.0f;
	m_bRenderBoundingSphere = FALSE;

	HRESULT hr = 0;

	LPD3DXBUFFER	pAdjBuffer;
	LPD3DXBUFFER	pMtrlBuffer;
	DWORD				dwNumMtrls;

	hr = D3DXLoadMeshFromX(
		L"Ex11/bigship1.x",
		D3DXMESH_MANAGED,
		m_pdev,
		&pAdjBuffer,
		&pMtrlBuffer,
		0,
		&dwNumMtrls,
		&m_pMesh);

	if (pMtrlBuffer != NULL && dwNumMtrls != 0)
	{
		D3DXMATERIAL*	mtrls = (D3DXMATERIAL*)pMtrlBuffer->GetBufferPointer();

		for (int i = 0; i < dwNumMtrls; i++)
		{
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			m_vMtrls.push_back(mtrls[i].MatD3D);

			if (mtrls[i].pTextureFilename != NULL)
			{
				LPDIRECT3DTEXTURE9	pTex = NULL;

				D3DXCreateTextureFromFileA(
					m_pdev,
					mtrls[i].pTextureFilename,
					&pTex);

				m_vTextures.push_back(pTex);
			}
			else
			{
				m_vTextures.push_back(NULL);
			}
		}
	}

	pMtrlBuffer->Release();

	hr = m_pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)pAdjBuffer->GetBufferPointer(),
		0, 0, 0);

	pAdjBuffer->Release();

	CBoundingBox boundingBox;
	CBoundingSphere boundingSphere;

	boundingBox.ComputeBoundingBox(m_pMesh);
	boundingSphere.ComputeBoundingSphere(m_pMesh);

	D3DXCreateSphere(m_pdev, boundingSphere.m_fRadius, 20, 20, &m_pSphereMesh, 0);

	D3DXCreateBox(m_pdev,
		boundingBox.m_vMax.x - boundingBox.m_vMin.x,
		boundingBox.m_vMax.y - boundingBox.m_vMin.y,
		boundingBox.m_vMax.z - boundingBox.m_vMin.z,
		&m_pBoxMesh, 0);
}

void CEx11_BoundingVolumes::Release()
{

}

void CEx11_BoundingVolumes::OnRender()
{
	if (m_pdev)
	{
		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

		D3DXVECTOR3 dir(1.0f, -1.0f, 1.0f);
		D3DXCOLOR col(1.0f, 1.0f, 1.0f, 1.0f);
		D3DLIGHT9 light = d3d::InitDirectionalLight(&dir, &col);

		m_pdev->SetLight(0, &light);
		m_pdev->LightEnable(0, true);
		m_pdev->SetRenderState(D3DRS_NORMALIZENORMALS, true);
		m_pdev->SetRenderState(D3DRS_SPECULARENABLE, true);

		m_pdev->SetTransform(D3DTS_WORLD, &m_mtWorld);

		for (int i = 0; i < m_vMtrls.size(); ++i)
		{
			m_pdev->SetMaterial(&m_vMtrls[i]);
			m_pdev->SetTexture(0, m_vTextures[i]);
			m_pMesh->DrawSubset(i);
		}

		D3DMATERIAL9 blue = d3d::BLUE_MTRL;
		blue.Diffuse.a = 0.1f;

		m_pdev->SetMaterial(&blue);
		m_pdev->SetTexture(0, 0);

		m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		m_pdev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pdev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		if (m_bRenderBoundingSphere)
			m_pSphereMesh->DrawSubset(0);
		else
			m_pBoxMesh->DrawSubset(0);

		m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	}
}

void CEx11_BoundingVolumes::OnUpdate()
{
	if (m_pdev)
	{
		D3DXMatrixRotationY(&m_mtWorld, m_fYRot);
		m_fYRot += g_pGame->GetManager()->GetTimeManager()->GetTimeDelta();

		if (m_fYRot >= 6.28f)
			m_fYRot = 0.0f;

		if (::GetAsyncKeyState('Y') & 0x8000f)
			m_bRenderBoundingSphere = TRUE;
		if (::GetAsyncKeyState('U') & 0x8000f)
			m_bRenderBoundingSphere = FALSE;
	}
}
