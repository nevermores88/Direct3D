#include "stdafx.h"
#include "Mesh.h"


CMesh::CMesh()
{
}


CMesh::~CMesh()
{
}

void CMesh::Create(LPDIRECT3DDEVICE9 pdev, wchar_t* wsFileName, char* szTexPath)
{
	m_pdev = pdev;

	LPD3DXBUFFER	pMtrlBuffer;

	//1. 메쉬 먼저 로딩
	if (FAILED(D3DXLoadMeshFromX(
		wsFileName,
		D3DXMESH_SYSTEMMEM,
		m_pdev, NULL,
		&pMtrlBuffer, NULL,
		&m_dwNumMtrl, &m_pMesh)))
	{
		return;
	}

	//2. Material 과 Texture 생성
	if (pMtrlBuffer != NULL && m_dwNumMtrl != 0)
	{
		LPD3DXMATERIAL pMtrls = (LPD3DXMATERIAL)pMtrlBuffer->GetBufferPointer();

		for (DWORD i = 0; i < m_dwNumMtrl; i++)
		{
			pMtrls[i].MatD3D.Ambient = pMtrls[i].MatD3D.Diffuse;
			m_vMtrls.push_back(pMtrls[i].MatD3D);


			if (pMtrls[i].pTextureFilename != NULL)
			{
				char szFileName[512];
				sprintf(szFileName, "%s/%s", szTexPath, pMtrls[i].pTextureFilename);

				LPDIRECT3DTEXTURE9 pTex = NULL;
				D3DXCreateTextureFromFileExA(
					m_pdev,
					szFileName,
					D3DX_DEFAULT,
					D3DX_DEFAULT,
					D3DX_DEFAULT,
					0,
					D3DFMT_UNKNOWN,
					D3DPOOL_MANAGED,
					D3DX_DEFAULT,
					D3DX_DEFAULT,
					0x00ffffff,
					NULL,
					NULL,
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
}

void CMesh::Release()
{
	if (m_pMesh)
		m_pMesh->Release();

	m_vMtrls.clear();

	for (std::vector<LPDIRECT3DTEXTURE9>::iterator itr = m_vTextures.begin(); itr != m_vTextures.end();)
	{
		LPDIRECT3DTEXTURE9 pTex = static_cast<LPDIRECT3DTEXTURE9>(*itr);
		
		if (pTex)
			pTex->Release();
	}

	m_vTextures.clear();
}

void CMesh::OnRender()
{
	if (m_pdev)
	{
		m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);

		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		//!!!!!!!
		m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pdev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pdev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		m_pdev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		m_pdev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pdev->SetRenderState(D3DRS_ALPHAREF, 156);
		m_pdev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		for (DWORD i = 0; i < m_dwNumMtrl; ++i)
		{
			m_pdev->SetMaterial(&m_vMtrls[i]);
			m_pdev->SetTexture(0, m_vTextures[i]);

			m_pMesh->DrawSubset(i);
		}

		m_pdev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_pdev->SetTexture(0, NULL);
	}
}

void CMesh::OnUpdate()
{
	if (m_pdev)
	{

	}
}
