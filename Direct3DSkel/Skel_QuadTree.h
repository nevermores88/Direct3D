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
	float m_fRadius;		//��带 ���δ� ��豸�� ������

public:
	//���� ��Ʈ��� ������
	CSkel_QuadTree(int iX, int iY);

	//���� �ڽĳ�� ������
	CSkel_QuadTree(CSkel_QuadTree* pParent);

	~CSkel_QuadTree();

	//����Ʈ�� ����
	bool Build(CSkel_Terrain::Vertex* pHeightMap = NULL);

	//�ﰢ���� �ε����� ����� ����� �ﰢ���� ������ ��ȯ
	int GenerateIndex(LPVOID pIB, CSkel_Terrain::Vertex* pHeightMap, CSkel_ZFrustum* pFrustum);

private:
	//�ڽ� ��� �߰�
	CSkel_QuadTree*	AddChild(int iCornerTL, int iCornerTR, int iCornerBL, int iCornerBR);

	//4���� �ڳʰ� ����
	bool SetCorners(int iCornerTL, int iCornerTR, int iCornerBL, int iCornerBR);

	//Quadtree�� 4���� ���� Ʈ���� �κк���
	bool SubDivide();

	//���� ��尡 ��� ������ �������?
	bool IsVisible() { return (m_iCorner[CORNER_TR] - m_iCorner[CORNER_TL] <= 1); }

	//����� �������� �ε����� ����
	int GenTriIndex(int iTriangles, LPVOID pIndex);

	void Destroy();

	//�����尡 �������ҿ� ���ԵǴ°�?
	int IsInFrustum(CSkel_Terrain::Vertex* pHeightMap, CSkel_ZFrustum* pFrustum);

	//IsInFrustumCull �Լ��� ����� ���� �������� �ø� ����
	void FrustumCull(CSkel_Terrain::Vertex* pHeightMap, CSkel_ZFrustum* pFrustum);
};

