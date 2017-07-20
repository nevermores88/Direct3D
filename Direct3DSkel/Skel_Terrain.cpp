#include "stdafx.h"
#include "Skel_Terrain.h"
#include "Skel_QuadTree.h"
#include "Skel_ZFrustum.h"
#include "Dib.h"

CSkel_Terrain::CSkel_Terrain()
{
}

CSkel_Terrain::~CSkel_Terrain()
{
}

HRESULT CSkel_Terrain::Create(LPDIRECT3DDEVICE9 pdev, D3DXVECTOR3* pScaleVector, LPSTR lpBMPFilename, LPSTR lpTexFilename[MAX_TERRAIN_TEX])
{
	m_pdev = pdev;
	m_vScale = *pScaleVector;

	if (FAILED(BuildHeightMap(lpBMPFilename)))
	{
		Destroy();
		return E_FAIL;
	}

	if (FAILED(LoadTextures(lpTexFilename)))
	{
		Destroy();
		return E_FAIL;
	}

	if (FAILED(CreateVIB()))
	{
		Destroy();
		return E_FAIL;
	}
	
	m_pQuadTree = new CSkel_QuadTree(m_iXDib, m_iZDib);

	if (FAILED(BuildQuadTree()))
	{
		Destroy();
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CSkel_Terrain::Draw(CSkel_ZFrustum* pFrustum)
{
	LPDWORD		pI;

	if (FAILED(m_pIB->Lock(0, (m_iXDib - 1)*(m_iZDib - 1) * 2 * sizeof(Index), (void**)&pI, 0)))
		return E_FAIL;

	m_iTriangles = m_pQuadTree->GenerateIndex(pI, m_pHeightMap, pFrustum);
	m_pIB->Unlock();

	Render();

	return S_OK;
}

HRESULT CSkel_Terrain::Destroy()
{
	if (m_pQuadTree)
	{
		delete m_pQuadTree;
	}

	delete[] m_pHeightMap;

	if (m_pVB)
	{
		m_pVB->Release();
		m_pVB = NULL;
	}

	if (m_pIB)
	{
		m_pIB->Release();
		m_pIB = NULL;
	}

	for (int i = 0; i < 4; i++)
	{
		if (m_pTex[i])
			delete m_pTex[i];
	}

	return S_OK;
}

HRESULT CSkel_Terrain::LoadTextures(LPSTR lpTexFilename[MAX_TERRAIN_TEX])
{
	for (int i = 0; i < 4; i++)
		D3DXCreateTextureFromFile(m_pdev, lpTexFilename[i], &m_pTex[i]);

	return S_OK;
}

HRESULT CSkel_Terrain::BuildHeightMap(LPSTR lpFilename)
{
	LPBYTE pDib = DibLoadHandle(lpFilename);
	
	if (!pDib)
		return E_FAIL;

	m_iXDib = DIB_CX(pDib);
	m_iZDib = DIB_CY(pDib);

	m_pHeightMap = new Vertex[m_iXDib * m_iZDib];

	Vertex v;
	for (int z = 0; z < m_iZDib; z++)
	{
		for (int x = 0; x < m_iXDib; x++)
		{
			v.p.x = (float)((x - m_iXDib / 2)*m_vScale.x);
			v.p.z = -(float)((z - m_iZDib / 2)*m_vScale.z);
			v.p.y = (float)(*(DIB_DATAXY_INV(pDib, x, z)))*m_vScale.y;

			D3DXVec3Normalize(&v.n, &v.p);

			v.t.x = (float)x / (float)(m_iXDib - 1);
			v.t.y = (float)z / (float)(m_iZDib - 1);

			m_pHeightMap[x + z*m_iXDib] = v;
		}
	}

	DibDeleteHandle(pDib);

	return S_OK;
}

HRESULT CSkel_Terrain::BuildQuadTree()
{
	m_pQuadTree->Build(m_pHeightMap);

	return S_OK;
}

HRESULT CSkel_Terrain::CreateVIB()
{
	// VB생성
	if (FAILED(m_pdev->CreateVertexBuffer(m_iXDib*m_iZDib*sizeof(Vertex),
		0, Vertex::FVF, D3DPOOL_DEFAULT, &m_pVB, NULL)))
	{
		Destroy();
		return E_FAIL;
	}

	void* pVertices;
	if (FAILED(m_pVB->Lock(0, m_iXDib*m_iZDib*sizeof(Vertex), (void**)&pVertices, 0)))
	{
		Destroy();
		return E_FAIL;
	}
	memcpy(pVertices, m_pHeightMap, m_iXDib*m_iZDib*sizeof(Vertex));
	m_pVB->Unlock();

#ifdef _USE_INDEX16
	// IB생성
	if (FAILED(m_pdev->CreateIndexBuffer((m_iXDib - 1)*(m_iZDib - 1) * 2 * sizeof(Index), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIB, NULL)))
	{
		Destroy();
		return E_FAIL;
	}
#else
	// IB생성
	if (FAILED(m_pd3dDevice->CreateIndexBuffer((m_cxDIB - 1)*(m_czDIB - 1) * 2 * sizeof(TRIINDEX), 0, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &m_pIB, NULL)))
	{
		_Destroy();
		return E_FAIL;
	}
#endif

	return S_OK;
}

HRESULT CSkel_Terrain::Render()
{
	m_pdev->SetTexture(0, m_pTex[0]);								// 0번 텍스쳐 스테이지에 텍스쳐 고정(색깔맵)
	m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// 0번 텍스처 스테이지의 확대 필터
	m_pdev->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);		// 0번 텍스처 : 0번 텍스처 인덱스 사용

	m_pdev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pdev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pdev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pdev->SetStreamSource(0, m_pVB, 0, sizeof(Vertex));
	m_pdev->SetFVF(Vertex::FVF);
	m_pdev->SetIndices(m_pIB);
	m_pdev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iXDib * m_iZDib, 0, m_iTriangles);

	return S_OK;
}


