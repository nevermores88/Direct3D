#include "stdafx.h"
#include "TimeManager.h"


CTimeManager::CTimeManager()
{
}


CTimeManager::~CTimeManager()
{
}

void CTimeManager::Create()
{
	Clear();
}

void CTimeManager::Release()
{
	Clear();
}

void CTimeManager::Clear()
{
	m_fLastTime = 0.f;
	m_fDeltaTime = 0.f;
}

float CTimeManager::GetTimeDelta()
{
	return m_fDeltaTime;
}