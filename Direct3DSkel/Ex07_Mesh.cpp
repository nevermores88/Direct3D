#include "stdafx.h"
#include "Ex07_Mesh.h"


CEx07_Mesh::CEx07_Mesh()
{
}


CEx07_Mesh::~CEx07_Mesh()
{
}

HRESULT CEx07_Mesh::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	LPD3DXBUFFER pMtrlBuffer;
	//용책에는 D3DXMESH_MANAGED 이걸로 세팅되어있음
	if (FAILED(D3DXLoadMeshFromX(L"Ex07_Mesh/Tiger.x", D3DXMESH_SYSTEMMEM, m_pdev, NULL,
		&pMtrlBuffer, NULL, &m_dwNumMaterials, &m_pMesh)))
	{
		return E_FAIL;
	}

	//읽어드린 메쉬 정보를 변수에 대입
	if (pMtrlBuffer != NULL && m_dwNumMaterials != 0)
	{
		D3DXMATERIAL* pMtrls = (D3DXMATERIAL*)pMtrlBuffer->GetBufferPointer();
		
		m_Materials = new D3DMATERIAL9[m_dwNumMaterials];

		if (m_Materials == NULL)
			return E_OUTOFMEMORY;

		m_pTextures = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];

		if (m_pTextures == NULL)
			return E_OUTOFMEMORY;

		for (int i = 0; i < m_dwNumMaterials; i++)
		{
			m_Materials[i] = pMtrls[i].MatD3D;
			m_Materials[i].Ambient = pMtrls[i].MatD3D.Diffuse;

			m_pTextures[i] = NULL;

			if (pMtrls[i].pTextureFilename != NULL && lstrlenA(pMtrls[i].pTextureFilename)>0)
			{
				const CHAR* strPrefix = "..\\Direct3DSkel02\\Ex07_Mesh\\";
				CHAR strTexture[MAX_PATH];
				strcpy_s(strTexture, strPrefix);
				strcat_s(strTexture, pMtrls[i].pTextureFilename);

				if (FAILED(D3DXCreateTextureFromFileA(m_pdev, strTexture, &m_pTextures[i])))
				{
					::MessageBox(NULL, L"Couldn't find texture map", L"Meshes.exe", MB_OK);
				}
			}
		}

		pMtrlBuffer->Release();
	}
	
	return S_OK;
}

void CEx07_Mesh::Release()
{
	if (m_Materials != NULL)
		delete[] m_Materials;

	if (m_pTextures)
	{
		for (DWORD i = 0; i < m_dwNumMaterials; i++)
		{
			if (m_pTextures[i])
				m_pTextures[i]->Release();
		}
		delete[] m_pTextures;
	}

	if (m_pMesh)
		m_pMesh->Release();
}

void CEx07_Mesh::Render()
{
	if (m_pdev)
	{
		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);

		//월드행렬 설정
		D3DXMATRIX mtWorld;
		D3DXMatrixIdentity(&mtWorld);
		D3DXMatrixRotationY(&mtWorld, timeGetTime() / 1000.0f);
		m_pdev->SetTransform(D3DTS_WORLD, &mtWorld);

		for (DWORD i = 0; i < m_dwNumMaterials; i++)
		{
			m_pdev->SetMaterial(&m_Materials[i]);
			m_pdev->SetTexture(0, m_pTextures[i]);

			m_pMesh->DrawSubset(i);
		}
	}
}

void CEx07_Mesh::Update()
{

}
