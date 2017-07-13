#pragma once
#include "TimeManager.h"
#include "CameraManager.h"
#include "InputManager.h"

class CManager
{
private:
	CTimeManager* m_pTimeManager;
	CCameraManager*	m_pCameraManager;
	CInputManager*	m_pInputManager;

public:
	CManager();
	~CManager();

	void Create();
	void Release();

	CTimeManager*	GetTimeManager(){ return m_pTimeManager; }
	CCameraManager*	GetCameraManager(){ return m_pCameraManager; }
	CInputManager*	GetInputManager(){ return m_pInputManager; }
};

