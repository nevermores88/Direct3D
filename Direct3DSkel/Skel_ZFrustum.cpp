#include "stdafx.h"
#include "Skel_ZFrustum.h"

CSkel_ZFrustum::CSkel_ZFrustum()
{
}


CSkel_ZFrustum::~CSkel_ZFrustum()
{
}

bool CSkel_ZFrustum::Make(D3DXMATRIX* pViewProjMatrix)
{
	D3DXMATRIX mtInverse;

	// 투영행렬까지 거치면 모든 3차원 월드좌표의 점은 (-1,-1,0) ~ (1,1,1)사이의 값으로 바뀐다.
	// m_vtx에 이 동차공간의 경계값을 넣어둔다.
	m_pVertices[0].x = -1.0f;	m_pVertices[0].y = -1.0f;	m_pVertices[0].z = 0.0f;
	m_pVertices[1].x = 1.0f;	m_pVertices[1].y = -1.0f;	m_pVertices[1].z = 0.0f;
	m_pVertices[2].x = 1.0f;	m_pVertices[2].y = -1.0f;	m_pVertices[2].z = 1.0f;
	m_pVertices[3].x = -1.0f;	m_pVertices[3].y = -1.0f;	m_pVertices[3].z = 1.0f;
	m_pVertices[4].x = -1.0f;	m_pVertices[4].y = 1.0f;	m_pVertices[4].z = 0.0f;
	m_pVertices[5].x = 1.0f;	m_pVertices[5].y = 1.0f;	m_pVertices[5].z = 0.0f;
	m_pVertices[6].x = 1.0f;	m_pVertices[6].y = 1.0f;	m_pVertices[6].z = 1.0f;
	m_pVertices[7].x = -1.0f;	m_pVertices[7].y = 1.0f;	m_pVertices[7].z = 1.0f;

	// view * proj의 역행렬을 구한다.
	D3DXMatrixInverse(&mtInverse, NULL, pViewProjMatrix);

	// Vertex_최종 = Vertex_local * Matrix_world * Matrix_view * Matrix_Proj 인데,
	// Vertex_world = Vertex_local * Matrix_world이므로,
	// Vertex_최종 = Vertex_world * Matrix_view * Matrix_Proj 이다.
	// Vertex_최종 = Vertex_world * ( Matrix_view * Matrix_Proj ) 에서
	// 역행렬( Matrix_view * Matrix_Proj )^-1를 양변에 곱하면
	// Vertex_최종 * 역행렬( Matrix_view * Matrix_Proj )^-1 = Vertex_World 가 된다.
	// 그러므로, m_vtx * matInv = Vertex_world가 되어, 월드좌표계의 프러스텀 좌표를 얻을 수 있다.
	for (int i = 0; i < 8; i++)
	{
		D3DXVec3TransformCoord(&m_pVertices[i], &m_pVertices[i], &mtInverse);
	}

	// 0번과 5번은 프러스텀중 near평면의 좌측상단과 우측하단이므로, 둘의 좌표를 더해서 2로 나누면
	// 카메라의 좌표를 얻을 수 있다.(정확히 일치하는 것은 아니다.)
	m_vPos = (m_pVertices[0] + m_pVertices[5]) / 2.0f;

	// 얻어진 월드좌표로 프러스텀 평면을 만든다
	// 벡터가 프러스텀 안쪽에서 바깥쪽으로 나가는 평면들이다.
	//	D3DXPlaneFromPoints(&m_plane[0], m_vtx+4, m_vtx+7, m_vtx+6);	// 상 평면(top)
	//	D3DXPlaneFromPoints(&m_plane[1], m_vtx  , m_vtx+1, m_vtx+2);	// 하 평면(bottom)
	//	D3DXPlaneFromPoints(&m_plane[2], m_vtx  , m_vtx+4, m_vtx+5);	// 근 평면(near)
	D3DXPlaneFromPoints(&m_Plane[3], m_pVertices + 2, m_pVertices + 6, m_pVertices + 7);	// 원 평면(far)
	D3DXPlaneFromPoints(&m_Plane[4], m_pVertices, m_pVertices + 3, m_pVertices + 7);	// 좌 평면(left)
	D3DXPlaneFromPoints(&m_Plane[5], m_pVertices + 1, m_pVertices + 5, m_pVertices + 6);	// 우 평면(right)

	return true;
}

bool CSkel_ZFrustum::IsIn(D3DXVECTOR3* pPos)
{
	float		fDist;

	// 현재는 left, right, far plane만 적용한다.
	//	for(int i = 0 ; i < 6 ; i++ )
	{
		fDist = D3DXPlaneDotCoord(&m_Plane[3], pPos);
		if (fDist > PLANE_EPSILON) 
			return FALSE;	// plane의 normal벡터가 far로 향하고 있으므로 양수이면 프러스텀의 바깥쪽
		
		fDist = D3DXPlaneDotCoord(&m_Plane[4], pPos);
		if (fDist > PLANE_EPSILON) 
			return FALSE;	// plane의 normal벡터가 left로 향하고 있으므로 양수이면 프러스텀의 왼쪽
		
		fDist = D3DXPlaneDotCoord(&m_Plane[5], pPos);
		if (fDist > PLANE_EPSILON) 
			return FALSE;	// plane의 normal벡터가 right로 향하고 있으므로 양수이면 프러스텀의 오른쪽
	}

	return true;
}

/** 중심(v)와 반지름(radius)를 갖는 경계구(bounding sphere)가 프러스텀안에 있으면
*  TRUE를 반환, 아니면 FALSE를 반환한다.
*/
bool CSkel_ZFrustum::IsInSphere(D3DXVECTOR3* pPos, float fRadius)
{
	float		fDist;

	fDist = D3DXPlaneDotCoord(&m_Plane[3], pPos);
	if (fDist > (fRadius + PLANE_EPSILON))
		return FALSE;	// 평면과 중심점의 거리가 반지름보다 크면 프러스텀에 없음

	fDist = D3DXPlaneDotCoord(&m_Plane[4], pPos);
	if (fDist > (fRadius + PLANE_EPSILON))
		return FALSE;	// 평면과 중심점의 거리가 반지름보다 크면 프러스텀에 없음

	fDist = D3DXPlaneDotCoord(&m_Plane[5], pPos);
	if (fDist > (fRadius + PLANE_EPSILON))
		return FALSE;	// 평면과 중심점의 거리가 반지름보다 크면 프러스텀에 없음

	return TRUE;
}

bool CSkel_ZFrustum::Draw(LPDIRECT3DDEVICE9 pdev)
{
	WORD		index[] = { 0, 1, 2,
		0, 2, 3,
		4, 7, 6,
		4, 6, 5,
		1, 5, 6,
		1, 6, 2,
		0, 3, 7,
		0, 7, 4,
		0, 4, 5,
		0, 5, 1,
		3, 7, 6,
		3, 6, 2 };

	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));

	typedef struct tagVTX
	{
		D3DXVECTOR3	p;
	} Vertex;

	Vertex pVertices[8];

	for (int i = 0; i < 8; i++)
	{
		pVertices[i].p = m_pVertices[i];
	}

	pdev->SetFVF(D3DFVF_XYZ);
	pdev->SetStreamSource(0, NULL, 0, sizeof(Vertex));
	pdev->SetTexture(0, NULL);
	pdev->SetIndices(0);
	pdev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	pdev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pdev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	pdev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	pdev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// 파란색으로 상,하 평면을 그린다.
	pdev->SetRenderState(D3DRS_LIGHTING, TRUE);
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	pdev->SetMaterial(&mtrl);
	pdev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 4, index, D3DFMT_INDEX16, pVertices, sizeof(pVertices[0]));

	// 녹색으로 좌,우 평면을 그린다.
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	pdev->SetMaterial(&mtrl);
	pdev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 4, index + 4 * 3, D3DFMT_INDEX16, pVertices, sizeof(pVertices[0]));

	// 붉은색으로 원,근 평면을 그린다.
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	pdev->SetMaterial(&mtrl);
	pdev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 4, index + 8 * 3, D3DFMT_INDEX16, pVertices, sizeof(pVertices[0]));

	pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pdev->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return TRUE;
}

void ZMakePlane(D3DXPLANE* pPlane, D3DXVECTOR3* pV0, D3DXVECTOR3* pV1, D3DXVECTOR3* pV2)
{

}