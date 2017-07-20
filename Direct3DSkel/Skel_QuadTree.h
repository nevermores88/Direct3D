#pragma once

#define _USE_INDEX16

#include "Skel_Terrain.h"

class CSkel_ZFrustum;
class CSkel_Terrain;

class CSkel_QuadTree
{

	enum CornerType { CORNER_TL, CORNER_TR, CORNER_BL, CORNER_BR };
	enum QuadLocation{FRUSTUM_OUT, FRUSTUM_PARTIALLY_IN, FRUSTUM_COMPLETELY_IN, FRUSUTM_UNKNOWN};

	CSkel_QuadTree* m_pChild[4];

	int m_iCenter;
	int m_iCorner[4];

	bool m_bCulled;
	float m_fRadius;		//노드를 감싸는 경계구의 반지름

public:
	//최초 루트노드 생성자
	CSkel_QuadTree(int iX, int iY);

	//하위 자식노드 생성자
	CSkel_QuadTree(CSkel_QuadTree* pParent);

	~CSkel_QuadTree();

	//쿼드트리 구축
	bool Build(CSkel_Terrain::Vertex* pHeightMap = NULL);

	//삼각형의 인덱스를 만들고 출력할 삼각형의 개수를 반환
	int GenerateIndex(LPVOID pIB, CSkel_Terrain::Vertex* pHeightMap, CSkel_ZFrustum* pFrustum);

private:
	//자식 노드 추가
	CSkel_QuadTree*	AddChild(int iCornerTL, int iCornerTR, int iCornerBL, int iCornerBR);

	//4개의 코너값 세팅
	bool SetCorners(int iCornerTL, int iCornerTR, int iCornerBL, int iCornerBR);

	//Quadtree를 4개의 하위 트리로 부분분할
	bool SubDivide();

	//현재 노드가 출력 가능한 노드인지?
	bool IsVisible() { return (m_iCorner[CORNER_TR] - m_iCorner[CORNER_TL] <= 1); }

	//출력할 폴리곤의 인덱스를 생성
	int GenTriIndex(int iTriangles, LPVOID pIndex);

	void Destroy();

	//현재노드가 프러스텀에 포함되는가?
	int IsInFrustum(CSkel_Terrain::Vertex* pHeightMap, CSkel_ZFrustum* pFrustum);

	//IsInFrustumCull 함수의 결과에 따라 프러스텀 컬링 수행
	void FrustumCull(CSkel_Terrain::Vertex* pHeightMap, CSkel_ZFrustum* pFrustum);
};

