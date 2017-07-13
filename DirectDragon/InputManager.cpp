#include "stdafx.h"
#include "InputManager.h"


CInputManager::CInputManager()
{
}


CInputManager::~CInputManager()
{
}

void CInputManager::Create()
{
	Clear();
}

void CInputManager::Release()
{
	Clear();
}

void CInputManager::Clear()
{

}

void CInputManager::OnCamInputUpdate(CCamera* pCam)
{
	if (pCam)
	{
		float fTimeDelta = g_pTimeManager->GetTimeDelta();
		float fRotSpeed = 0.03f;
		float fMoveSpeed = 1.0f;

		if (::GetAsyncKeyState(VK_UP) & 0x8000f)
			pCam->Pitch(-fRotSpeed);

		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
			pCam->Pitch(fRotSpeed);

		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
			pCam->Yaw(-fRotSpeed);

		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
			pCam->Yaw(fRotSpeed);

		if (::GetAsyncKeyState('N') & 0x8000f)
			pCam->Roll(-fRotSpeed);

		if (::GetAsyncKeyState('M') & 0x8000f)
			pCam->Roll(fRotSpeed);

		if (::GetAsyncKeyState('W') & 0x8000f)
			pCam->Walk(fMoveSpeed);

		if (::GetAsyncKeyState('S') & 0x8000f)
			pCam->Walk(-fMoveSpeed);

		if (::GetAsyncKeyState('A') & 0x8000f)
			pCam->Strafe(-fMoveSpeed);

		if (::GetAsyncKeyState('D') & 0x8000f)
			pCam->Strafe(fMoveSpeed);

		if (::GetAsyncKeyState('R') & 0x8000f)
			pCam->Fly(fMoveSpeed);

		if (::GetAsyncKeyState('F') & 0x8000f)
			pCam->Fly(-fMoveSpeed);
	}
}
