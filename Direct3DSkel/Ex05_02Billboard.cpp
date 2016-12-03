#include "stdafx.h"
#include "Ex05_02Billboard.h"


CEx05_02Billboard::CEx05_02Billboard()
{
	m_pWater = NULL;
	m_pCam = NULL;

	memset(m_pTexBillboard, 0, sizeof(m_pTexBillboard));

	m_dwMouseX = 0;
	m_dwMouseY = 0;
	m_bBillboard = TRUE;
	m_bWireframe = FALSE;
}


CEx05_02Billboard::~CEx05_02Billboard()
{
}

HRESULT CEx05_02Billboard::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pWater = new CEx05_02Water();
	m_pWater->Create(pdev, 64, 64, 100);

	m_pCam = new CEx05_02Camera();

	//텍스쳐 생성
	D3DXCreateTextureFromFile(m_pdev, L"Ex05_02/tree01S.dds", &m_pTexBillboard[0]);
	D3DXCreateTextureFromFile(m_pdev, L"Ex05_02/tree02S.dds", &m_pTexBillboard[1]);
	D3DXCreateTextureFromFile(m_pdev, L"Ex05_02/tree35S.dds", &m_pTexBillboard[2]);
	D3DXCreateTextureFromFile(m_pdev, L"Ex05_02/tex.jpg", &m_pTexBillboard[3]);

	return S_OK;
}

void CEx05_02Billboard::Release()
{
	if (m_pWater)
	{
		m_pWater->Release();
		delete m_pWater;
		m_pWater = NULL;
	}

	if (m_pCam)
	{
		delete m_pCam;
		m_pCam = NULL;
	}

	for (int i = 0; i < 4; i++)
	{
		if (m_pTexBillboard[i])
		{
			m_pTexBillboard[i]->Release();
			m_pTexBillboard[i] = NULL;
		}
	}
}

void CEx05_02Billboard::Render()
{
	if (m_pdev)
	{
		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);

		m_pWater->Render();

		m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pdev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pdev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pdev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pdev->SetRenderState(D3DRS_ALPHAREF, 0x08);
		m_pdev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

		struct MYVERTEX
		{
			enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
			float px, py, pz;
			float tu, tv;
		};

		//빌보드 관련
		Vertex vtx[4] =
		{
			{ -1, 0, 0, 0x80ffffff, 0, 1 },
			{ -1, 4, 0, 0x80ffffff, 0, 0 },
			{ 1, 0, 0, 0x80ffffff, 1, 1 },
			{ 1, 4, 0, 0x80ffffff, 1, 0 }
		};

		/*Vertex vtx[4] =
		{
		{ -1, 0, 0, 0xffffffff, 0, 1 },
		{ -1, 4, 0, 0xffffffff, 0, 0 },
		{ 1, 0, 0, 0xffffffff, 1, 1 },
		{ 1, 4, 0, 0xffffffff, 1, 0 }
		}; */

		D3DXMATRIX mtBillboard;
		D3DXMatrixIdentity(&mtBillboard);

		m_pdev->SetTexture(1, NULL);
		m_pdev->SetFVF(Vertex::FVF);

		if (m_bBillboard)
		{
			// Y축 회전행렬은 _11, _13, _31, _33번 행렬에 회전값이 들어간다
			// 카메라의 Y축 회전행렬값을 읽어서 역행렬을 만들면 X,Z축이 고정된
			// Y축 회전 빌보드 행렬을 만들수 있다
			mtBillboard._11 = g_pGame->m_matView._11;//m_pCam->GetViewMatrix()->_11;
			mtBillboard._13 = g_pGame->m_matView._13;//m_pCam->GetViewMatrix()->_13;
			mtBillboard._31 = g_pGame->m_matView._31;//m_pCam->GetViewMatrix()->_31;
			mtBillboard._33 = g_pGame->m_matView._33;//m_pCam->GetViewMatrix()->_33;
			D3DXMatrixInverse(&mtBillboard, NULL, &mtBillboard);
		}

		// 빌보드의 좌표를 바꿔가며 찍는다
		for (int z = 0; z <= 40; z += 5)
		{
			for (int x = 0; x <= 40; x += 5)
			{
				mtBillboard._41 = x - 20;
				mtBillboard._42 = 0;
				mtBillboard._43 = z - 20;
				m_pdev->SetTexture(0, m_pTexBillboard[(x + z) % 4]);
				m_pdev->SetTransform(D3DTS_WORLD, &mtBillboard);
				m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtx, sizeof(Vertex));
			}
		}

		m_pdev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		//m_pdev->SetTransform(D3DTS_WORLD, &g_matWorld);

	}
}

void CEx05_02Billboard::Update()
{

}

CEx05_02Water::CEx05_02Water()
{
	m_pVB = NULL;
	m_pIB = NULL;
	memset(m_pTex, 0, sizeof(m_pTex));

	m_nX = 0;
	m_nZ =0;
	m_nWaveTick =0;
	m_nPrevTick = 0;
	m_fWave = 0;
	m_fDir = 0;
}

CEx05_02Water::~CEx05_02Water()
{

}

HRESULT CEx05_02Water::Create(LPDIRECT3DDEVICE9 pdev, int nX, int nZ, int nSpeed)
{
	CBaseClass::Create(pdev);

	m_nX = nX;
	m_nZ = nZ;
	m_nWaveTick = nSpeed;

	m_nPrevTick = GetTickCount();
	m_fDir = 0.1f;

	LoadTexture();
	CreateVIB();

	return S_OK;
}

void CEx05_02Water::Release()
{
	if (m_pVB)
	{
		m_pVB->Release();
		m_pVB = NULL;
	}

	for (int i = 0; i < 2; i++)
	{
		if (m_pTex[i])
		{
			m_pTex[i]->Release();
			m_pTex[i] = NULL;
		}
	}
}

void CEx05_02Water::Render()
{
	if (m_pdev)
	{
		float x, z;
		D3DXMATRIX mtWorld;
		D3DXMatrixIdentity(&mtWorld);

		WaveTexture();

		m_pdev->SetTexture(0, m_pTex[0]);
		m_pdev->SetTexture(1, m_pTex[1]);

		m_pdev->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);		// 0번 텍스처 : 0번 텍스처 인덱스 사용
		m_pdev->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);		// 1번 텍스처 : 0번 텍스처 인덱스 사용

		m_pdev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		m_pdev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pdev->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pdev->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);

		//m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
		//m_pdev->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);

		m_pdev->SetStreamSource(0, m_pVB, 0, sizeof(Vertex));
		m_pdev->SetFVF(Vertex::FVF);

		for (z = 0.0f; z < (float)m_nZ; z++)
		{
			for (x = 0.0f; x < (float)m_nX; x++)
			{
				mtWorld._41 = x - m_nX / 2.0f;
				mtWorld._43 = -((float)z - m_nZ / 2.0f);
				m_pdev->SetTransform(D3DTS_WORLD, &mtWorld);
				m_pdev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
			}
		}

		D3DXMatrixIdentity(&mtWorld);
		m_pdev->SetTransform(D3DTS_WORLD, &mtWorld);
		m_pdev->SetTransform(D3DTS_TEXTURE0, &mtWorld);
		m_pdev->SetTransform(D3DTS_TEXTURE1, &mtWorld);
	}
}

void CEx05_02Water::Update()
{

}

HRESULT CEx05_02Water::LoadTexture()
{
	D3DXCreateTextureFromFile(m_pdev, L"Ex05_02/water1.bmp", &m_pTex[0]);
	D3DXCreateTextureFromFile(m_pdev, L"Ex05_02/water2.bmp", &m_pTex[1]);

	return S_OK;
}

// 4각형 정점버퍼를 만든다
// 지금은 4각형 메시를 여러번 출력하는데, 이것은 무지무지 비효율적인 방식이다
// 물전체를 표현할수 있는 커다란 정점버퍼(혹은 메시)를 만드는것이 더 낫다
HRESULT CEx05_02Water::CreateVIB()
{
	Vertex vertices[4] =
	{
		{ -0.5f, 0.0f, -0.5f, 0x7fffffff, 0.0f, 1.0f },
		{ -0.5f, 0.0f, 0.5f, 0x7fffffff, 0.0f, 0.0f },
		{ 0.5f, 0.0f, -0.5f, 0x7fffffff, 1.0f, 1.0f },
		{ 0.5f, 0.0f, 0.5f, 0x7fffffff, 1.0f, 0.0f },
	};

	if (FAILED(m_pdev->CreateVertexBuffer(4 * sizeof(Vertex), 0, Vertex::FVF, D3DPOOL_DEFAULT, &m_pVB, NULL)))
		return S_OK;

	Vertex* pVertices;

	if (FAILED(m_pVB->Lock(0, 4 * sizeof(Vertex), (void**)&pVertices, 0)))
		return S_OK;

	memcpy(pVertices, vertices, 4 * sizeof(Vertex));
	m_pVB->Unlock();

	return S_OK;
}

HRESULT CEx05_02Water::WaveTexture()
{
	D3DXMATRIX mtTex;
	D3DXMatrixIdentity(&mtTex);

	int nCurTick = GetTickCount();

	if ((nCurTick - m_nPrevTick) > m_nWaveTick)
	{
		m_fWave += m_fDir;
		if (m_fWave > (D3DX_PI / 8.0f)) m_fDir = -0.01f;
		if (m_fWave < (-D3DX_PI / 8.0f)) m_fDir = 0.01f;

		m_pdev->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
		m_pdev->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

		D3DXMatrixRotationX(&mtTex, m_fWave);
		m_pdev->SetTransform(D3DTS_TEXTURE0, &mtTex);

		D3DXMatrixRotationY(&mtTex, m_fWave);
		m_pdev->SetTransform(D3DTS_TEXTURE1, &mtTex);
	}

	return S_OK;
}

// 생성자
CEx05_02Camera::CEx05_02Camera()
{
	D3DXVECTOR3	eye(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3	lookat(0.0f, 0.0f, -1.0f);
	D3DXVECTOR3	up(0.0f, 1.0f, 0.0f);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matBill);
	SetView(&eye, &lookat, &up);
}

/// 카메라 행렬을 생성하기위한 기본 벡터값들을 설정한다.
D3DXMATRIXA16*	CEx05_02Camera::SetView(D3DXVECTOR3* pvEye, D3DXVECTOR3* pvLookat, D3DXVECTOR3* pvUp)
{
	m_vEye = *pvEye;
	m_vLookat = *pvLookat;
	m_vUp = *pvUp;
	D3DXVec3Normalize(&m_vView, &(m_vLookat - m_vEye));
	D3DXVec3Cross(&m_vCross, &m_vUp, &m_vView);

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vLookat, &m_vUp);
	D3DXMatrixInverse(&m_matBill, NULL, &m_matView);
	m_matBill._41 = 0.0f;
	m_matBill._42 = 0.0f;
	m_matBill._43 = 0.0f;

	return &m_matView;
}

/// 카메라 좌표계의 X축으로 angle만큼 회전한다.
D3DXMATRIXA16* CEx05_02Camera::RotateLocalX(float angle)
{
	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationAxis(&matRot, &m_vCross, angle);

	D3DXVECTOR3 vNewDst, vNewUp;
	D3DXVec3TransformCoord(&vNewDst, &m_vView, &matRot);	// view * rot로 새로운 dst vector를 구한다.
	//	D3DXVec3Cross( &vNewUp, &vNewDst, &m_vCross );			// cross( dst, x축)으로 up vector를 구한다.
	//	D3DXVec3Normalize( &vNewUp, &vNewUp );					// up vector를 unit vector로...
	vNewDst += m_vEye;										// 실제 dst position =  eye Position + dst vector

	return SetView(&m_vEye, &vNewDst, &m_vUp);
}

/// 카메라 좌표계의 Y축으로 angle만큼 회전한다.
D3DXMATRIXA16* CEx05_02Camera::RotateLocalY(float angle)
{
	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationAxis(&matRot, &m_vUp, angle);

	D3DXVECTOR3 vNewDst;
	D3DXVec3TransformCoord(&vNewDst, &m_vView, &matRot);	// view * rot로 새로운 dst vector를 구한다.
	vNewDst += m_vEye;										// 실제 dst position =  eye Position + dst vector

	return SetView(&m_vEye, &vNewDst, &m_vUp);
}

/// 카메라 좌표계의 X축방향으로 dist만큼 전진한다.(후진은 -dist를 넣으면 된다.)
D3DXMATRIXA16* CEx05_02Camera::MoveLocalX(float dist)
{
	D3DXVECTOR3 vNewEye = m_vEye;
	D3DXVECTOR3 vNewDst = m_vLookat;

	D3DXVECTOR3 vMove;
	D3DXVec3Normalize(&vMove, &m_vCross);
	vMove *= dist;
	vNewEye += vMove;
	vNewDst += vMove;

	return SetView(&vNewEye, &vNewDst, &m_vUp);
}

/// 카메라 좌표계의 Y축방향으로 dist만큼 전진한다.(후진은 -dist를 넣으면 된다.)
D3DXMATRIXA16* CEx05_02Camera::MoveLocalY(float dist)
{
	D3DXVECTOR3 vNewEye = m_vEye;
	D3DXVECTOR3 vNewDst = m_vLookat;

	D3DXVECTOR3 vMove;
	D3DXVec3Normalize(&vMove, &m_vUp);
	vMove *= dist;
	vNewEye += vMove;
	vNewDst += vMove;

	return SetView(&vNewEye, &vNewDst, &m_vUp);
}

/// 카메라 좌표계의 Z축방향으로 dist만큼 전진한다.(후진은 -dist를 넣으면 된다.)
D3DXMATRIXA16* CEx05_02Camera::MoveLocalZ(float dist)
{
	D3DXVECTOR3 vNewEye = m_vEye;
	D3DXVECTOR3 vNewDst = m_vLookat;

	D3DXVECTOR3 vMove;
	D3DXVec3Normalize(&vMove, &m_vView);
	vMove *= dist;
	vNewEye += vMove;
	vNewDst += vMove;

	return SetView(&vNewEye, &vNewDst, &m_vUp);
}

/// 월드좌표계의 *pv값의 위치로 카메라를 이동한다.
D3DXMATRIXA16* CEx05_02Camera::MoveTo(D3DXVECTOR3* pv)
{
	D3DXVECTOR3	dv = *pv - m_vEye;
	m_vEye = *pv;
	m_vLookat += dv;
	return SetView(&m_vEye, &m_vLookat, &m_vUp);
}
