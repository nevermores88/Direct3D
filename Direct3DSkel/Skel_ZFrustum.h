#pragma once

/// �������ҿ� ��Ȯ�ϰ� ���Ե��� �ʴ���, �ణ�� ������ �־ �������ҿ� ���Խ�Ű�� ���� ��
#define PLANE_EPSILON	5.0f

class CSkel_ZFrustum
{
	D3DXVECTOR3	m_pVertices[8];				//����ü�� ������ ���� 8��
	D3DXVECTOR3	m_vPos;							//���� ī�޶��� ������ǥ
	D3DXPLANE		m_Plane[6];					//���������� �����ϴ� 6���� ���

public:
	CSkel_ZFrustum();
	~CSkel_ZFrustum();

	// ī�޶�(view) * ��������(projection)����� �Է¹޾� 6���� ����� �����.
	bool Make(D3DXMATRIX* pViewProjMatrix);

	// ���� v�� �������Ҿȿ� ������ TRUE�� ��ȯ, �ƴϸ� FALSE�� ��ȯ�Ѵ�.
	bool IsIn(D3DXVECTOR3* pPos);

	/** �߽�(v)�� ������(radius)�� ���� ��豸(bounding sphere)�� �������Ҿȿ� ������
	*  TRUE�� ��ȯ, �ƴϸ� FALSE�� ��ȯ�Ѵ�.
	*/
	bool IsInSphere(D3DXVECTOR3* pPos, float fRadius);

	// ���������� ȭ�鿡 �׷��ش�.
	bool Draw(LPDIRECT3DDEVICE9 pdev);

	// ���� ī�޶��� ������ǥ�� ����ش�.
	D3DXVECTOR3* GetPos(){ return &m_vPos; }
};

void ZMakePlane(D3DXPLANE* pPlane, D3DXVECTOR3* pV0, D3DXVECTOR3* pV1, D3DXVECTOR3* pV2);
