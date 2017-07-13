#include "stdafx.h"
#include "Camera.h"


CCamera::CCamera()
{
}

CCamera::~CCamera()
{
}

void CCamera::Create(CameraType camType /*= AIRCRAFT*/, LPDIRECT3DDEVICE9 pdev /*= NULL*/)
{
	m_CamType = camType;

	m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vLook = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	m_pdev = pdev;
}

void CCamera::Pitch(float angle)
{
	D3DXMATRIX mat;

	D3DXMatrixRotationAxis(&mat, &m_vRight, angle);

	D3DXVec3TransformCoord(&m_vUp, &m_vUp, &mat);
	D3DXVec3TransformCoord(&m_vLook, &m_vLook, &mat);
}

void CCamera::Yaw(float angle)
{
	D3DXMATRIX mat;

	if (m_CamType == LANDOBJECT)
		D3DXMatrixRotationY(&mat, angle);

	if (m_CamType == AIRCRAFT)
		D3DXMatrixRotationAxis(&mat, &m_vUp, angle);

	D3DXVec3TransformCoord(&m_vRight, &m_vRight, &mat);
	D3DXVec3TransformCoord(&m_vLook, &m_vLook, &mat);
}

void CCamera::Roll(float angle)
{
	if (m_CamType == AIRCRAFT)
	{
		D3DXMATRIX mat;

		D3DXMatrixRotationAxis(&mat, &m_vLook, angle);

		D3DXVec3TransformCoord(&m_vRight, &m_vRight, &mat);
		D3DXVec3TransformCoord(&m_vUp, &m_vUp, &mat);
	}
}

void CCamera::GetViewMatrix(D3DXMATRIX* v)
{
	D3DXVec3Normalize(&m_vLook, &m_vLook);

	D3DXVec3Cross(&m_vUp, &m_vLook, &m_vRight);
	D3DXVec3Normalize(&m_vUp, &m_vUp);

	D3DXVec3Cross(&m_vRight, &m_vUp, &m_vLook);
	D3DXVec3Normalize(&m_vRight, &m_vRight);

	float x = -D3DXVec3Dot(&m_vRight, &m_vPos);
	float y = -D3DXVec3Dot(&m_vUp, &m_vPos);
	float z = -D3DXVec3Dot(&m_vLook, &m_vPos);

	(*v)(0, 0) = m_vRight.x; (*v)(0, 1) = m_vUp.x; (*v)(0, 2) = m_vLook.x; (*v)(0, 3) = 0.0f;
	(*v)(1, 0) = m_vRight.y; (*v)(1, 1) = m_vUp.y; (*v)(1, 2) = m_vLook.y; (*v)(1, 3) = 0.0f;
	(*v)(2, 0) = m_vRight.z; (*v)(2, 1) = m_vUp.z; (*v)(2, 2) = m_vLook.z; (*v)(2, 3) = 0.0f;
	(*v)(3, 0) = x;               (*v)(3, 1) = y;           (*v)(3, 2) = z;              (*v)(3, 3) = 1.0f;
}

void CCamera::SetCameraType(CameraType camType)
{
	m_CamType = camType;
}

void CCamera::GetPosition(D3DXVECTOR3* pos)
{
	*pos = m_vPos;
}

void CCamera::SetPosition(D3DXVECTOR3* pos)
{
	m_vPos = *pos;
}

void CCamera::GetRight(D3DXVECTOR3* right)
{
	*right = m_vRight;
}

void CCamera::GetUp(D3DXVECTOR3* up)
{
	*up = m_vUp;
}

void CCamera::GetLook(D3DXVECTOR3* look)
{
	*look = m_vLook;
}

void CCamera::Walk(float units)
{
	if (units == 0)
		return;

	if (m_CamType == LANDOBJECT)
		m_vPos += D3DXVECTOR3(m_vLook.x, 0.0f, m_vLook.z)*units;
	if (m_CamType == AIRCRAFT)
		m_vPos += m_vLook*units;
}

void CCamera::Strafe(float units)
{
	if (m_CamType == LANDOBJECT)
		m_vPos += D3DXVECTOR3(m_vRight.x, 0.0f, m_vRight.z)*units;
	if (m_CamType == AIRCRAFT)
		m_vPos += m_vRight * units;
}

void CCamera::Fly(float units)
{
	if (m_CamType == AIRCRAFT)
		m_vPos += m_vUp * units;
}

void CCamera::Release()
{

}

void CCamera::OnUpdate()
{
	if (m_pdev)
	{
		D3DXMATRIX m_mtView;
		GetViewMatrix(&m_mtView);

		m_pdev->SetTransform(D3DTS_VIEW, &m_mtView);
	}
}
