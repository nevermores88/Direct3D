#pragma once

/// 프러스텀에 정확하게 포함되지 않더라도, 약간의 여분을 주어서 프러스텀에 포함시키기 위한 값
#define PLANE_EPSILON	5.0f

class CSkel_ZFrustum
{
	D3DXVECTOR3	m_pVertices[8];				//절두체를 구성할 정점 8개
	D3DXVECTOR3	m_vPos;							//현재 카메라의 월드좌표
	D3DXPLANE		m_Plane[6];					//프러스텀을 구성하는 6개의 평면

public:
	CSkel_ZFrustum();
	~CSkel_ZFrustum();

	// 카메라(view) * 프로젝션(projection)행렬을 입력받아 6개의 평면을 만든다.
	bool Make(D3DXMATRIX* pViewProjMatrix);

	// 한점 v가 프러스텀안에 있으면 TRUE를 반환, 아니면 FALSE를 반환한다.
	bool IsIn(D3DXVECTOR3* pPos);

	/** 중심(v)와 반지름(radius)를 갖는 경계구(bounding sphere)가 프러스텀안에 있으면
	*  TRUE를 반환, 아니면 FALSE를 반환한다.
	*/
	bool IsInSphere(D3DXVECTOR3* pPos, float fRadius);

	// 프러스텀을 화면에 그려준다.
	bool Draw(LPDIRECT3DDEVICE9 pdev);

	// 현재 카메라의 월드좌표를 얻어준다.
	D3DXVECTOR3* GetPos(){ return &m_vPos; }
};

void ZMakePlane(D3DXPLANE* pPlane, D3DXVECTOR3* pV0, D3DXVECTOR3* pV1, D3DXVECTOR3* pV2);
