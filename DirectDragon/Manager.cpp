#include "stdafx.h"
#include "Manager.h"


CManager::CManager()
{
}


CManager::~CManager()
{
}

void CManager::Create()
{
	m_pTimeManager = new CTimeManager;
	
	if (m_pTimeManager)
		m_pTimeManager->Create();

	m_pCameraManager = new CCameraManager;

	if (m_pCameraManager)
		m_pCameraManager->Create();

	m_pInputManager = new CInputManager;

	if (m_pInputManager)
		m_pInputManager->Create();
}

void CManager::Release()
{
	if (m_pTimeManager)
	{
		m_pTimeManager->Release();
		delete m_pTimeManager;
		m_pTimeManager = NULL;
	}

	if (m_pCameraManager)
	{
		m_pCameraManager->Release();
		delete m_pCameraManager;
		m_pCameraManager = NULL;
	}

	if (m_pInputManager)
	{
		m_pInputManager->Release();
		delete m_pInputManager;
		m_pInputManager = NULL;
	}
}
