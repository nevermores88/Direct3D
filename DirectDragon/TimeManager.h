#pragma once
class CTimeManager
{
public:
	float	m_fLastTime;
	float	m_fDeltaTime;

public:
	CTimeManager();
	~CTimeManager();

	void Create();
	void Release();

	void Clear();

	float GetTimeDelta();
};

