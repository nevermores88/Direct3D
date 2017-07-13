#include "stdafx.h"
#include "Ex11_PMesh.h"


CEx11_PMesh::CEx11_PMesh()
{
}


CEx11_PMesh::~CEx11_PMesh()
{
}

void CEx11_PMesh::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;
	m_fYRot = 0.0f;

	HRESULT hr = 0;

	LPD3DXBUFFER	pAdjBuffer = NULL;
	LPD3DXBUFFER	pMtrlBuffer = NULL;
	DWORD				dwNumMtrls = NULL;

	hr = D3DXLoadMeshFromX(
		L"Ex11/bigship1.x",
		D3DXMESH_MANAGED,
		m_pdev,
		&pAdjBuffer, &pMtrlBuffer, 0, &dwNumMtrls, &m_pSourceMesh);

	if (pMtrlBuffer != NULL && dwNumMtrls != 0)
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)pMtrlBuffer->GetBufferPointer();

		for (int i = 0; i < dwNumMtrls; i++)
		{
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			m_vMtrls.push_back(mtrls[i].MatD3D);

			if (mtrls[i].pTextureFilename != NULL)
			{
				LPDIRECT3DTEXTURE9 pTex = NULL;
				D3DXCreateTextureFromFileA(m_pdev, mtrls[i].pTextureFilename, &pTex);
				m_vTextures.push_back(pTex);
			}
			else
			{
				m_vTextures.push_back(NULL);
			}
		}
	}
	pMtrlBuffer->Release();

	hr = m_pSourceMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)pAdjBuffer->GetBufferPointer(),
		(DWORD*)pAdjBuffer->GetBufferPointer(),
		0, 0);

	hr = D3DXGeneratePMesh(m_pSourceMesh,
		(DWORD*)pAdjBuffer->GetBufferPointer(),
		0, 0, 1, D3DXMESHSIMP_FACE, &m_pPMesh);

	m_pSourceMesh->Release();

	DWORD maxFaces = m_pPMesh->GetMaxFaces();
	m_pPMesh->SetNumFaces(maxFaces);


}

void CEx11_PMesh::Release()
{

}

void CEx11_PMesh::OnRender()
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
			m_pPMesh->DrawSubset(i);

			m_pdev->SetMaterial(&d3d::YELLOW_MTRL);
			m_pdev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			m_pPMesh->DrawSubset(i);
			m_pdev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}

	}
}

void CEx11_PMesh::OnUpdate()
{
	if (m_pdev)
	{
		int numFaces = m_pPMesh->GetNumFaces();

		if (::GetAsyncKeyState('Y') & 0x8000f)
		{
			//m_pPMesh->SetNumFaces(numFaces + 1);
			//if (m_pPMesh->GetNumFaces() == numFaces)
				m_pPMesh->SetNumFaces(numFaces + 30);
		}

		if (::GetAsyncKeyState('U') & 0x8000f)
			m_pPMesh->SetNumFaces(numFaces - 30);

		D3DXMatrixRotationY(&m_mtWorld, m_fYRot);
		m_fYRot += g_pGame->GetManager()->GetTimeManager()->GetTimeDelta();

		if (m_fYRot >= 6.28f)
			m_fYRot = 0.0f;
	}
}
