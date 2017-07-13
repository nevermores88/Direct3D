#pragma once
class CInputManager
{
public:
	CInputManager();
	~CInputManager();

	void Create();
	void Release();

	void Clear();

	void OnCamInputUpdate(CCamera* pCam);
};

