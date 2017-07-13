#include "stdafx.h"
#include "CameraManager.h"


CCameraManager::CCameraManager()
{
}


CCameraManager::~CCameraManager()
{
}

void CCameraManager::Create()
{
	Clear();
}

void CCameraManager::Release()
{
	Clear();
}

void CCameraManager::Clear()
{
	for (vector<CCamera*>::iterator itr = m_vCam.begin(); itr != m_vCam.end(); )
	{
		CCamera* pCam = static_cast<CCamera*>(*itr);
		
		if (pCam)
		{
			pCam->Release();
			delete pCam;
			itr = m_vCam.erase(itr);
		}
		else
			++itr;
	}
}

bool CCameraManager::CreateCamera(CCamera::CameraType camType /*= CCamera::CameraType::AIRCRAFT*/, LPDIRECT3DDEVICE9 pdev /*= NULL*/)
{
	CCamera* cam = new CCamera();

	if (cam)
	{
		cam->Create(camType, pdev);
		m_vCam.push_back(cam);
		return true;
	}

	return false;
}

bool CCameraManager::DeleteCamera(int idx)
{
	if (m_vCam[idx])
	{
		m_vCam[idx]->Release();
		delete m_vCam[idx];
		m_vCam.erase(m_vCam.begin() + idx);

		return true;
	}

	return false;
}

CCamera* CCameraManager::GetCamera()
{
	return GetCamera(0);
}

CCamera* CCameraManager::GetCamera(int idx)
{
	if (idx < m_vCam.size())
		return m_vCam[idx];
	else
		return NULL;
}
