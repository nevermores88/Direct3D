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

	// ������ı��� ��ġ�� ��� 3���� ������ǥ�� ���� (-1,-1,0) ~ (1,1,1)������ ������ �ٲ��.
	// m_vtx�� �� ���������� ��谪�� �־�д�.
	m_pVertices[0].x = -1.0f;	m_pVertices[0].y = -1.0f;	m_pVertices[0].z = 0.0f;
	m_pVertices[1].x = 1.0f;	m_pVertices[1].y = -1.0f;	m_pVertices[1].z = 0.0f;
	m_pVertices[2].x = 1.0f;	m_pVertices[2].y = -1.0f;	m_pVertices[2].z = 1.0f;
	m_pVertices[3].x = -1.0f;	m_pVertices[3].y = -1.0f;	m_pVertices[3].z = 1.0f;
	m_pVertices[4].x = -1.0f;	m_pVertices[4].y = 1.0f;	m_pVertices[4].z = 0.0f;
	m_pVertices[5].x = 1.0f;	m_pVertices[5].y = 1.0f;	m_pVertices[5].z = 0.0f;
	m_pVertices[6].x = 1.0f;	m_pVertices[6].y = 1.0f;	m_pVertices[6].z = 1.0f;
	m_pVertices[7].x = -1.0f;	m_pVertices[7].y = 1.0f;	m_pVertices[7].z = 1.0f;

	// view * proj�� ������� ���Ѵ�.
	D3DXMatrixInverse(&mtInverse, NULL, pViewProjMatrix);

	// Vertex_���� = Vertex_local * Matrix_world * Matrix_view * Matrix_Proj �ε�,
	// Vertex_world = Vertex_local * Matrix_world�̹Ƿ�,
	// Vertex_���� = Vertex_world * Matrix_view * Matrix_Proj �̴�.
	// Vertex_���� = Vertex_world * ( Matrix_view * Matrix_Proj ) ����
	// �����( Matrix_view * Matrix_Proj )^-1�� �纯�� ���ϸ�
	// Vertex_���� * �����( Matrix_view * Matrix_Proj )^-1 = Vertex_World �� �ȴ�.
	// �׷��Ƿ�, m_vtx * matInv = Vertex_world�� �Ǿ�, ������ǥ���� �������� ��ǥ�� ���� �� �ִ�.
	for (int i = 0; i < 8; i++)
	{
		D3DXVec3TransformCoord(&m_pVertices[i], &m_pVertices[i], &mtInverse);
	}

	// 0���� 5���� ���������� near����� ������ܰ� �����ϴ��̹Ƿ�, ���� ��ǥ�� ���ؼ� 2�� ������
	// ī�޶��� ��ǥ�� ���� �� �ִ�.(��Ȯ�� ��ġ�ϴ� ���� �ƴϴ�.)
	m_vPos = (m_pVertices[0] + m_pVertices[5]) / 2.0f;

	// ����� ������ǥ�� �������� ����� �����
	// ���Ͱ� �������� ���ʿ��� �ٱ������� ������ �����̴�.
	//	D3DXPlaneFromPoints(&m_plane[0], m_vtx+4, m_vtx+7, m_vtx+6);	// �� ���(top)
	//	D3DXPlaneFromPoints(&m_plane[1], m_vtx  , m_vtx+1, m_vtx+2);	// �� ���(bottom)
	//	D3DXPlaneFromPoints(&m_plane[2], m_vtx  , m_vtx+4, m_vtx+5);	// �� ���(near)
	D3DXPlaneFromPoints(&m_Plane[3], m_pVertices + 2, m_pVertices + 6, m_pVertices + 7);	// �� ���(far)
	D3DXPlaneFromPoints(&m_Plane[4], m_pVertices, m_pVertices + 3, m_pVertices + 7);	// �� ���(left)
	D3DXPlaneFromPoints(&m_Plane[5], m_pVertices + 1, m_pVertices + 5, m_pVertices + 6);	// �� ���(right)

	return true;
}

bool CSkel_ZFrustum::IsIn(D3DXVECTOR3* pPos)
{
	float		fDist;

	// ����� left, right, far plane�� �����Ѵ�.
	//	for(int i = 0 ; i < 6 ; i++ )
	{
		fDist = D3DXPlaneDotCoord(&m_Plane[3], pPos);
		if (fDist > PLANE_EPSILON) 
			return FALSE;	// plane�� normal���Ͱ� far�� ���ϰ� �����Ƿ� ����̸� ���������� �ٱ���
		
		fDist = D3DXPlaneDotCoord(&m_Plane[4], pPos);
		if (fDist > PLANE_EPSILON) 
			return FALSE;	// plane�� normal���Ͱ� left�� ���ϰ� �����Ƿ� ����̸� ���������� ����
		
		fDist = D3DXPlaneDotCoord(&m_Plane[5], pPos);
		if (fDist > PLANE_EPSILON) 
			return FALSE;	// plane�� normal���Ͱ� right�� ���ϰ� �����Ƿ� ����̸� ���������� ������
	}

	return true;
}

/** �߽�(v)�� ������(radius)�� ���� ��豸(bounding sphere)�� �������Ҿȿ� ������
*  TRUE�� ��ȯ, �ƴϸ� FALSE�� ��ȯ�Ѵ�.
*/
bool CSkel_ZFrustum::IsInSphere(D3DXVECTOR3* pPos, float fRadius)
{
	float		fDist;

	fDist = D3DXPlaneDotCoord(&m_Plane[3], pPos);
	if (fDist > (fRadius + PLANE_EPSILON))
		return FALSE;	// ���� �߽����� �Ÿ��� ���������� ũ�� �������ҿ� ����

	fDist = D3DXPlaneDotCoord(&m_Plane[4], pPos);
	if (fDist > (fRadius + PLANE_EPSILON))
		return FALSE;	// ���� �߽����� �Ÿ��� ���������� ũ�� �������ҿ� ����

	fDist = D3DXPlaneDotCoord(&m_Plane[5], pPos);
	if (fDist > (fRadius + PLANE_EPSILON))
		return FALSE;	// ���� �߽����� �Ÿ��� ���������� ũ�� �������ҿ� ����

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

	// �Ķ������� ��,�� ����� �׸���.
	pdev->SetRenderState(D3DRS_LIGHTING, TRUE);
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	pdev->SetMaterial(&mtrl);
	pdev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 4, index, D3DFMT_INDEX16, pVertices, sizeof(pVertices[0]));

	// ������� ��,�� ����� �׸���.
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	pdev->SetMaterial(&mtrl);
	pdev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 4, index + 4 * 3, D3DFMT_INDEX16, pVertices, sizeof(pVertices[0]));

	// ���������� ��,�� ����� �׸���.
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