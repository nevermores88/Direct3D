#include "stdafx.h"
#include "Ex05_06Animation.h"


CEx05_06Animation::CEx05_06Animation()
{
}


CEx05_06Animation::~CEx05_06Animation()
{
}

HRESULT CEx05_06Animation::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	//정점버퍼 관련
	Vertex vertices[] =
	{
		{ -1, 1, 1, 0xffff0000 },		/// v0
		{ 1, 1, 1, 0xff00ff00 },		/// v1
		{ 1, 1, -1, 0xff0000ff },		/// v2
		{ -1, 1, -1, 0xffffff00 },		/// v3

		{ -1, -1, 1, 0xff00ffff },		/// v4
		{ 1, -1, 1, 0xffff00ff },		/// v5
		{ 1, -1, -1, 0xff000000 },		/// v6
		{ -1, -1, -1, 0xffffffff },		/// v7
	};

	if (FAILED(m_pdev->CreateVertexBuffer(8 * sizeof(Vertex), D3DUSAGE_WRITEONLY, Vertex::FVF, D3DPOOL_DEFAULT, &m_pVB, NULL)))
		return E_FAIL;

	VOID* pVertices;
	if (FAILED(m_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
		return E_FAIL;
	memcpy(pVertices, vertices, sizeof(vertices));
	m_pVB->Unlock();

	//인덱스버퍼 관련
	Index indices[] =
	{
		{ 0, 1, 2 }, { 0, 2, 3 },	/// 윗면
		{ 4, 6, 5 }, { 4, 7, 6 },	/// 아랫면
		{ 0, 3, 7 }, { 0, 7, 4 },	/// 왼면
		{ 1, 5, 6 }, { 1, 6, 2 },	/// 오른면
		{ 3, 2, 6 }, { 3, 6, 7 },	/// 앞면
		{ 0, 4, 5 }, { 0, 5, 1 }	/// 뒷면
	};

	if (FAILED(m_pdev->CreateIndexBuffer(12 * sizeof(Index), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIB, NULL)))
	{
		return E_FAIL;
	}

	VOID* pIndices;
	if (FAILED(m_pIB->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		return E_FAIL;
	memcpy(pIndices, indices, sizeof(indices));
	m_pIB->Unlock();

	InitAnimation();

	return S_OK;
}

void CEx05_06Animation::Release()
{
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
}

void CEx05_06Animation::Render()
{
	if (m_pdev)
	{
		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);

		D3DXMATRIX mtWorld;

		mtWorld = m_mtRParent * m_mtTMParent;
		DrawMesh(&mtWorld);

		mtWorld = m_mtRChild*m_mtTMChild*m_mtRParent*m_mtTMParent;
		DrawMesh(&mtWorld);
	}
}

void CEx05_06Animation::Update()
{
	if (m_pdev)
	{
		static float t = 0.0f;
		float x, y, z;
		D3DXQUATERNION quat;

		if (t > 1.0f) t = 0.0f;

		x = d3d::Linear(m_vAniPos[0].x, m_vAniPos[1].x, t);
		y = d3d::Linear(m_vAniPos[0].y, m_vAniPos[1].y, t);
		z = d3d::Linear(m_vAniPos[0].z, m_vAniPos[1].z, t);

		D3DXMatrixTranslation(&m_mtTMParent, x, y, z);

		//	위의 4줄은 다음의 3줄로 바꿀수 있다.
		//	D3DXVECTOR3 v;
		//	D3DXVec3Lerp( &v, &g_aniPos[0], &g_aniPos[1], t );
		//	D3DXMatrixTranslation( &g_matTMParent, v.x, v.y, v.z );

		// 회전값의 구면선형보간
		D3DXQuaternionSlerp(&quat, &m_qAniRot[0], &m_qAniRot[1], t);
		D3DXMatrixRotationQuaternion(&m_mtRParent, &quat);
		t += 0.005f;

		D3DXMatrixRotationZ(&m_mtRChild, GetTickCount() / 500.0f);
		D3DXMatrixTranslation(&m_mtTMChild, 3, 3, 3);
	}
}

void CEx05_06Animation::DrawMesh(D3DXMATRIX* pMat)
{
	m_pdev->SetTransform(D3DTS_WORLD, pMat);
	m_pdev->SetStreamSource(0, m_pVB, 0, sizeof(Vertex));
	m_pdev->SetFVF(Vertex::FVF);
	m_pdev->SetIndices(m_pIB);
	m_pdev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
}

void CEx05_06Animation::InitAnimation()
{
	m_vAniPos[0] = D3DXVECTOR3(0, 0, 0);	/// 위치 키(0,0,0)
	m_vAniPos[1] = D3DXVECTOR3(5, 5, 5);	/// 위치 키(5,5,5)

	FLOAT Yaw = D3DX_PI * 90.0f / 180.0f;		/// Y축 90도 회전
	FLOAT Pitch = 0;
	FLOAT Roll = 0;
	D3DXQuaternionRotationYawPitchRoll(&m_qAniRot[0], Yaw, Pitch, Roll);	/// 사원수 키(Y축90도)

	Yaw = 0;
	Pitch = D3DX_PI * 90.0f / 180.0f;			/// X축 90도 회전
	Roll = 0;
	D3DXQuaternionRotationYawPitchRoll(&m_qAniRot[1], Yaw, Pitch, Roll);	/// 사원수 키(X축90도)
}
