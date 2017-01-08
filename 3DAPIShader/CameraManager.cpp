#include "stdafx.h"
#include "CameraManager.h"

CCameraManager* CCameraManager::m_pCameraManager = NULL;

CCameraManager::CCameraManager()
{
}


CCameraManager::~CCameraManager()
{
}

void CCameraManager::Create(LPDIRECT3DDEVICE9 pdev)
{
	DataClear();

	m_pdev = pdev;

	//다시 한번 더 체크
	D3DSURFACE_DESC d3dDesc;
	LPDIRECT3DSURFACE9 pd3dSurf;
	m_pdev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pd3dSurf);
	pd3dSurf->GetDesc(&d3dDesc);
	pd3dSurf->Release();

	m_fYaw = 0.0f;
	m_fPitch = 0.0f;
	m_vEye = D3DXVECTOR3(160, 200, -90);
	m_vLook = D3DXVECTOR3(230, 130, 110);
	m_vUp = D3DXVECTOR3(0, 1, 0);

	m_fScreenWidth = d3dDesc.Width;
	m_fScreenHeight = d3dDesc.Height;
	m_fFov = D3DX_PI / 4.0f;
	m_fNear = 1.0f;
	m_fFar = 5000.0f;

	D3DXMatrixLookAtLH(&m_mtView, &m_vEye, &m_vLook, &m_vUp);
	D3DXMatrixPerspectiveFovLH(&m_mtProj, m_fFov, m_fScreenWidth / m_fScreenHeight, m_fNear, m_fFar);
}

void CCameraManager::Release()
{
	DataClear();

	if (m_pCameraManager)
	{
		delete m_pCameraManager;
		m_pCameraManager = NULL;
	}
}

void CCameraManager::DataClear()
{
	m_pdev = NULL;

	memset(m_mtView, 0, sizeof(m_mtView));
	memset(m_mtProj, 0, sizeof(m_mtProj));

	memset(m_vEye, 0, sizeof(m_vEye));
	memset(m_vLook, 0, sizeof(m_vLook));
	memset(m_vUp, 0, sizeof(m_vUp));

	m_fScreenWidth = 0.0f;
	m_fScreenHeight = 0.0f;
	m_fFov = 0.0f;
	m_fNear = 0.0f;
	m_fFar = 0.0f;

	m_fYaw = 0.0f;
	m_fPitch = 0.0f;
}

void CCameraManager::Update()
{
	D3DXMatrixLookAtLH(&m_mtView, &m_vEye, &m_vLook, &m_vUp);
	D3DXMatrixPerspectiveFovLH(&m_mtProj, m_fFov, m_fScreenWidth / m_fScreenHeight, m_fNear, m_fFar);
}

CCameraManager* CCameraManager::GetInstance()
{
	if (!m_pCameraManager)
		m_pCameraManager = new CCameraManager();

	return m_pCameraManager;
}

void CCameraManager::SetEye(D3DXVECTOR3 vEye)
{
	m_vEye = vEye;
}

void CCameraManager::SetLook(D3DXVECTOR3 vLook)
{
	m_vLook = vLook;
}

void CCameraManager::SetUp(D3DXVECTOR3 vUp)
{
	m_vUp = vUp;
}

void CCameraManager::SetProjection(float fScreenWidth, float fScreenHeight, float fFov, float fNear, float fFar)
{
	m_fScreenWidth = fScreenWidth;
	m_fScreenHeight = fScreenHeight;
	m_fFov = fFov;
	m_fNear = fNear;
	m_fFar = fFar;
}

D3DXVECTOR3 CCameraManager::GetEye()
{
	return m_vEye;
}

D3DXVECTOR3 CCameraManager::GetLook()
{
	return m_vLook;
}

D3DXVECTOR3 CCameraManager::GetUp()
{
	return m_vUp;
}

void CCameraManager::MoveSide(float fSpeed)
{
	D3DXVECTOR3 tmp(m_mtView._11, 0, m_mtView._31);
	D3DXVec3Normalize(&tmp, &tmp);

	m_vEye += tmp * fSpeed;
	m_vLook += tmp*fSpeed;
}

void CCameraManager::MoveForward(float fSpeed, float fY /*= 0*/)
{
	D3DXVECTOR3 tmp(m_mtView._13, m_mtView._23*fY, m_mtView._33);
	D3DXVec3Normalize(&tmp, &tmp);

	m_vEye += tmp*fSpeed;
	m_vLook += tmp*fSpeed;
}

void CCameraManager::Rotation(D3DXVECTOR3 vDelta)
{
	m_fYaw = D3DXToRadian(vDelta.x*0.1f);
	m_fPitch = D3DXToRadian(vDelta.y*0.1f);

	D3DXMATRIX rot;
	D3DXVECTOR3 vZ = m_vLook - m_vEye;
	D3DXVECTOR3 vX;
	D3DXMatrixRotationY(&rot, m_fYaw);
	D3DXVec3TransformCoord(&vZ, &vZ, &rot);
	D3DXVec3TransformCoord(&m_vUp, &m_vUp, &rot);

	m_vLook = vZ + m_vEye;
	D3DXMatrixLookAtLH(&m_mtView, &m_vEye, &m_vLook, &m_vUp);


	vZ = m_vLook - m_vEye;
	vX = D3DXVECTOR3(m_mtView._11, m_mtView._21, m_mtView._31);

	D3DXMatrixRotationAxis(&rot, &vX, m_fPitch);
	D3DXVec3TransformCoord(&vZ, &vZ, &rot);
	D3DXVec3TransformCoord(&m_vUp, &m_vUp, &rot);

	m_vLook = vZ + m_vEye;
}

void CCameraManager::SetTransfrom()
{
	m_pdev->SetTransform(D3DTS_VIEW, &m_mtView);
	m_pdev->SetTransform(D3DTS_PROJECTION, &m_mtProj);
}
