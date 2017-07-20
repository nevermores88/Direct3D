#include "stdafx.h"
#include "Skel_QuadTree.h"
#include "Skel_ZFrustum.h"

CSkel_QuadTree::CSkel_QuadTree(int iX, int iY)
{
	m_iCenter = 0;

	for (int i = 0; i < 4; i++)
	{
		m_pChild[i] = NULL;
	}

	m_iCorner[CORNER_TL] = 0;
	m_iCorner[CORNER_TR] = iX - 1;
	m_iCorner[CORNER_BL] = iX*(iY - 1);
	m_iCorner[CORNER_BR] = iX*iY - 1;
	m_iCenter = (m_iCorner[CORNER_TL] + m_iCorner[CORNER_TR] + m_iCorner[CORNER_BL] + m_iCorner[CORNER_BR]) / 4;

	m_fRadius = 0.0f;
	m_bCulled = false;
}

CSkel_QuadTree::CSkel_QuadTree(CSkel_QuadTree* pParent)
{
	m_iCenter = 0;

	for (int i = 0; i < 4; i++)
	{
		m_pChild[i] = NULL;
		m_iCorner[i] = 0;
	}

	m_fRadius = 0.0f;
	m_bCulled = false;
}

CSkel_QuadTree::~CSkel_QuadTree()
{
	Destroy();
}

bool CSkel_QuadTree::Build(CSkel_Terrain::Vertex* pHeightMap)
{
	if (SubDivide())
	{
		if (pHeightMap != NULL)
		{
			D3DXVECTOR3 v = *((D3DXVECTOR3*)(pHeightMap + m_iCorner[CORNER_TL])) -
				*((D3DXVECTOR3*)(pHeightMap + m_iCorner[CORNER_BR]));
			m_fRadius = D3DXVec3Length(&v) / 2.0f;
		}
		
		m_pChild[CORNER_TL]->Build(pHeightMap);
		m_pChild[CORNER_TR]->Build(pHeightMap);
		m_pChild[CORNER_BL]->Build(pHeightMap);
		m_pChild[CORNER_BR]->Build(pHeightMap);
	}

	return true;
}

int CSkel_QuadTree::GenerateIndex(LPVOID pIB, CSkel_Terrain::Vertex* pHeightMap, CSkel_ZFrustum* pFrustum)
{
	FrustumCull(pHeightMap, pFrustum);
	return GenTriIndex(0, pIB);
}

CSkel_QuadTree* CSkel_QuadTree::AddChild(int iCornerTL, int iCornerTR, int iCornerBL, int iCornerBR)
{
	CSkel_QuadTree* pChild;

	pChild = new CSkel_QuadTree(this);
	pChild->SetCorners(iCornerTL, iCornerTR, iCornerBL, iCornerBR);

	return pChild;
}

bool CSkel_QuadTree::SetCorners(int iCornerTL, int iCornerTR, int iCornerBL, int iCornerBR)
{
	m_iCorner[CORNER_TL] = iCornerTL;
	m_iCorner[CORNER_TR] = iCornerTR;
	m_iCorner[CORNER_BL] = iCornerBL;
	m_iCorner[CORNER_BR] = iCornerBR;
	m_iCenter = (m_iCorner[CORNER_TL] + m_iCorner[CORNER_TR] + m_iCorner[CORNER_BL] + m_iCorner[CORNER_BR]) / 4;

	return true;
}

bool CSkel_QuadTree::SubDivide()
{
	int iTopEdgeCenter;
	int iBottomEdgeCenter;
	int iLeftEdgeCenter;
	int iRightEdgeCenter;
	int iCentralPoint;

	iTopEdgeCenter = (m_iCorner[CORNER_TL] + m_iCorner[CORNER_TR]) / 2;
	iBottomEdgeCenter = (m_iCorner[CORNER_BL] + m_iCorner[CORNER_BR]) / 2;
	iLeftEdgeCenter = (m_iCorner[CORNER_TL] + m_iCorner[CORNER_BL]) / 2;
	iRightEdgeCenter = (m_iCorner[CORNER_TR] + m_iCorner[CORNER_BR]) / 2;
	iCentralPoint = (m_iCorner[CORNER_TL] + m_iCorner[CORNER_TR] + m_iCorner[CORNER_BL] + m_iCorner[CORNER_BR]) / 4;

	if (m_iCorner[CORNER_TR] - m_iCorner[CORNER_TL] <= 1)
		return false;

	m_pChild[CORNER_TL] = AddChild(m_iCorner[CORNER_TL], iTopEdgeCenter, iLeftEdgeCenter, iCentralPoint);
	m_pChild[CORNER_TR] = AddChild(iTopEdgeCenter, m_iCorner[CORNER_TR], iCentralPoint, iRightEdgeCenter);
	m_pChild[CORNER_BL] = AddChild(iLeftEdgeCenter, iCentralPoint, m_iCorner[CORNER_BL], iBottomEdgeCenter);
	m_pChild[CORNER_BR] = AddChild(iCentralPoint, iRightEdgeCenter, iBottomEdgeCenter, m_iCorner[CORNER_BR]);

	return true;
}

int CSkel_QuadTree::GenTriIndex(int iTriangles, LPVOID pIndex)
{
	if (m_bCulled)
	{
		m_bCulled = false;
		return iTriangles;
	}

	if (IsVisible())
	{
#ifdef _USE_INDEX16
		LPWORD p = ((LPWORD)pIndex) + iTriangles * 3;
#else
		LPDWORD p = ((LPDWORD)pIndex) + iTriangles * 3;
#endif

		//좌측상단 삼각형
		*p++ = m_iCorner[0];
		*p++ = m_iCorner[1];
		*p++ = m_iCorner[2];
		iTriangles++;

		//우측하단 삼각형
		*p++ = m_iCorner[2];
		*p++ = m_iCorner[1];
		*p++ = m_iCorner[3];
		iTriangles++;

		return iTriangles;
	}

	if (m_pChild[CORNER_TL]) iTriangles = m_pChild[CORNER_TL]->GenTriIndex(iTriangles, pIndex);
	if (m_pChild[CORNER_TR]) iTriangles = m_pChild[CORNER_TR]->GenTriIndex(iTriangles, pIndex);
	if (m_pChild[CORNER_BL]) iTriangles = m_pChild[CORNER_BL]->GenTriIndex(iTriangles, pIndex);
	if (m_pChild[CORNER_BR]) iTriangles = m_pChild[CORNER_BR]->GenTriIndex(iTriangles, pIndex);

	return iTriangles;
}

void CSkel_QuadTree::Destroy()
{
	for (int i = 0; i < 4; i++)
	{
		if (m_pChild[i])
			delete m_pChild[i];
	}
}

int CSkel_QuadTree::IsInFrustum(CSkel_Terrain::Vertex* pHeightMap, CSkel_ZFrustum* pFrustum)
{
	bool b[4];
	bool bInSphere;

	bInSphere = pFrustum->IsInSphere((D3DXVECTOR3*)(pHeightMap + m_iCenter), m_fRadius);
	if (!bInSphere)
		return FRUSTUM_OUT;

	//쿼드트리의 4군데 경계 프러스텀 테스트
	b[0] = pFrustum->IsIn((D3DXVECTOR3*)(pHeightMap + m_iCorner[0]));
	b[1] = pFrustum->IsIn((D3DXVECTOR3*)(pHeightMap + m_iCorner[1]));
	b[2] = pFrustum->IsIn((D3DXVECTOR3*)(pHeightMap + m_iCorner[2]));
	b[3] = pFrustum->IsIn((D3DXVECTOR3*)(pHeightMap + m_iCorner[3]));

	if ((b[0] + b[1] + b[2] + b[3]) == 4)
		return FRUSTUM_COMPLETELY_IN;

	return FRUSTUM_PARTIALLY_IN;
}

void CSkel_QuadTree::FrustumCull(CSkel_Terrain::Vertex* pHeightMap, CSkel_ZFrustum* pFrustum)
{
	int ret;

	ret = IsInFrustum(pHeightMap, pFrustum);
	switch (ret)
	{
	case FRUSTUM_COMPLETELY_IN:
		m_bCulled = false;
		return;
	case FRUSTUM_PARTIALLY_IN:
		m_bCulled = false;
		return;
	case FRUSTUM_OUT:
		m_bCulled = true;
		return;
	}

	if (m_pChild[0])
		m_pChild[0]->FrustumCull(pHeightMap, pFrustum);
	
	if (m_pChild[1])
		m_pChild[1]->FrustumCull(pHeightMap, pFrustum);
	
	if (m_pChild[2])
		m_pChild[2]->FrustumCull(pHeightMap, pFrustum);
	
	if (m_pChild[3])
		m_pChild[3]->FrustumCull(pHeightMap, pFrustum);
}
