#pragma once
#include "Camera.h"

class CCameraManager
{
	std::vector<CCamera*> m_vCam;

public:
	CCameraManager();
	~CCameraManager();

	void Create();
	void Release();

	void Clear();

	bool CreateCamera(CCamera::CameraType camType = CCamera::CameraType::AIRCRAFT, LPDIRECT3DDEVICE9 pdev = NULL);
	bool DeleteCamera(int idx);

	CCamera* GetCamera();
	CCamera* GetCamera(int idx);
};

