#include "stdafx.h"
#include "Mesh.h"


CMesh::CMesh()
{
}


CMesh::~CMesh()
{
}

HRESULT CMesh::Create(LPDIRECT3DDEVICE9 pdev, char* xFileName, char* szPath)
{
	m_pdev = pdev;
	HRESULT hr = 0;

	LPD3DXBUFFER pBuffer;

	if (FAILED(D3DXLoadMeshFromX(xFileName, D3DXMESH_SYSTEMMEM, m_pdev, NULL, &pBuffer, NULL, &m_dwNumMtrl, &m_pMesh)))
		return E_FAIL;

	D3DXMATERIAL* pMtrls = (D3DXMATERIAL*)pBuffer->GetBufferPointer();
	m_pMtrls = new D3DMATERIAL9[m_dwNumMtrl];
	m_pTextures = new LPDIRECT3DTEXTURE9[m_dwNumMtrl];

	for(DWORD i = 0; i < m_dwNumMtrl; i++)
	{
		m_pMtrls[i] = pMtrls[i].MatD3D;
		m_pMtrls[i].Ambient = m_pMtrls[i].Diffuse;
		m_pTextures[i] = NULL;

		if (pMtrls[i].pTextureFilename != NULL && lstrlen(pMtrls[i].pTextureFilename) > 0)
		{
			char szFile[512] = "";
			
			if (szPath != NULL && lstrlen(szPath) > 0)
				sprintf(szFile, "%s/", szPath);
			
			strcat(szFile, pMtrls[i].pTextureFilename);
			
			hr = D3DXCreateTextureFromFileEx(m_pdev
				, szFile
				, D3DX_DEFAULT
				, D3DX_DEFAULT
				, D3DX_DEFAULT
				, 0
				, D3DFMT_UNKNOWN
				, D3DPOOL_MANAGED
				, D3DX_DEFAULT
				, D3DX_DEFAULT
				, 0x00FFFFFF
				, NULL
				, NULL
				, &m_pTextures[i]);

			if (FAILED(hr))
				OutputDebugString("Could not find texture map");
		}
	}

	pBuffer->Release();
}

void CMesh::Release()
{
	if (m_pMtrls)
		delete[] m_pMtrls;

	if (m_pTextures)
	{
		for (DWORD i = 0; i < m_dwNumMtrl; i++)
		{
			if (m_pTextures[i])
				m_pTextures[i]->Release();
		}
		delete[] m_pTextures;
	}

	if (m_pMesh)
		m_pMesh->Release();
}

void CMesh::Render(bool bUseMaterial)
{
	if (m_pdev)
	{
		//// 텍스춰 U, V, W의 어드레스 모드를 Wrap으로 설정한다.
		m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);

		// 텍스춰의 필터링을 Linear로 설정한다.
		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);


		m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pdev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pdev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		m_pdev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		m_pdev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pdev->SetRenderState(D3DRS_ALPHAREF, 156);
		m_pdev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


		for (DWORD i = 0; i < m_dwNumMtrl; i++)
		{
			// Set the material and texture for this subset
			if (bUseMaterial)
			{
				m_pdev->SetMaterial(&m_pMtrls[i]);
				m_pdev->SetTexture(0, m_pTextures[i]);
			}

			// Draw the mesh subset
			m_pMesh->DrawSubset(i);
		}

		m_pdev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pdev->SetTexture(0, NULL);
	}
}

void CMesh::Update()
{
	if (m_pdev)
	{

	}
}
