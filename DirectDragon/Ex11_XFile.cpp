#include "stdafx.h"
#include "Ex11_XFile.h"


CEx11_XFile::CEx11_XFile()
{
}


CEx11_XFile::~CEx11_XFile()
{
}

void CEx11_XFile::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;

	HRESULT hr = 0;

	LPD3DXBUFFER	pAdjBuffer = 0;
	LPD3DXBUFFER	pMtrlBuffer = 0;
	DWORD				dwNumMtrls = 0;

	hr = D3DXLoadMeshFromX(
		L"Ex11/bigship1.x",
		D3DXMESH_MANAGED,
		m_pdev, &pAdjBuffer, &pMtrlBuffer, 0, &dwNumMtrls, &m_pMesh);

	if (FAILED(hr))
	{
		::MessageBox(0, L"D3DXLoadMeshFromX() - FAILED", 0, 0);
		return;
	}

	if (pMtrlBuffer != 0 && dwNumMtrls != 0)
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)pMtrlBuffer->GetBufferPointer();

		for (int i = 0; i < dwNumMtrls; i++)
		{
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;
			m_vMtrls.push_back(mtrls[i].MatD3D);

			if (mtrls[i].pTextureFilename != NULL)
			{
				LPDIRECT3DTEXTURE9 tex = NULL;
				D3DXCreateTextureFromFileA(
					m_pdev, mtrls[i].pTextureFilename, &tex);
				m_vTextures.push_back(tex);
			}
			else
				m_vTextures.push_back(0);
		}
	}

	pMtrlBuffer->Release();

	/*std::ofstream outFile;
	outFile.open("Ex11/XFile_Before_Optimize.txt");
	d3d::DumpAttributeBuffer(outFile, m_pMesh);
	outFile.close();*/

	hr = m_pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)pAdjBuffer->GetBufferPointer(), 0, 0, 0);

	pAdjBuffer->Release();

	/*outFile.open("Ex11/XFile_After_Optimize.txt");
	d3d::DumpAttributeBuffer(outFile, m_pMesh);
	outFile.close();*/

	if (FAILED(hr))
	{
		::MessageBox(0, L"OptimizeInplace() - FAILED", 0, 0);
		return;
	}

	m_fYRot = 0.0f;

}

void CEx11_XFile::Release()
{

}

void CEx11_XFile::OnRender()
{
	if (m_pdev)
	{
		m_pdev->SetLight(0, &m_Light);
		m_pdev->LightEnable(0, true);
		m_pdev->SetRenderState(D3DRS_NORMALIZENORMALS, true);
		m_pdev->SetRenderState(D3DRS_SPECULARENABLE, true);

		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

		m_pdev->SetTransform(D3DTS_WORLD, &m_mtWorld);

		for (int i = 0; i < (int)m_vMtrls.size(); i++)
		{
			m_pdev->SetMaterial(&m_vMtrls[i]);
			m_pdev->SetTexture(0, m_vTextures[i]);
			m_pMesh->DrawSubset(i);
		}
	}
}

void CEx11_XFile::OnUpdate()
{
	if (m_pdev)
	{
		D3DXVECTOR3 dir(1.0f, -1.0f, 1.0f);
		D3DXCOLOR col(1.0f, 1.0f, 1.0f, 1.0f);
		m_Light = d3d::InitDirectionalLight(&dir, &col);

		D3DXMatrixRotationY(&m_mtWorld, m_fYRot);
		m_fYRot += g_pGame->GetManager()->GetTimeManager()->GetTimeDelta();

		if (m_fYRot >= 6.28f)
			m_fYRot = 0.0f;
	}
}
