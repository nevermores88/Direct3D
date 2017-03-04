#include "stdafx.h"
#include "InputManager.h"

CInputManager* CInputManager::m_pInputManager = NULL;

CInputManager::CInputManager()
{

}

CInputManager::~CInputManager()
{
}

void CInputManager::Create(HWND hWnd)
{
	DataClear();

	m_hWnd = hWnd;

	m_dwTimeDC = GetDoubleClickTime();

	m_dwBtnBegin[0] = GetTickCount();

	for (int i = 1; i < MAX_INPUT_BTN; i++)
	{
		m_dwBtnBegin[i] = m_dwBtnBegin[0];
	}

	int iNumLines = 0;		// 3 is the default?????
	SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &iNumLines, 0);
}

void CInputManager::Release()
{
	DataClear();

	if (m_pInputManager)
	{
		delete m_pInputManager;
		m_pInputManager = NULL;
	}
}

void CInputManager::DataClear()
{
	m_hWnd = NULL;

	memset(m_KeyOld, 0, sizeof(m_KeyOld));
	memset(m_KeyCur, 0, sizeof(m_KeyCur));
	memset(m_KeyMap, 0, sizeof(m_KeyMap));

	memset(m_BtnCur, 0, sizeof(m_BtnCur));
	memset(m_BtnOld, 0, sizeof(m_BtnOld));
	memset(m_BtnMap, 0, sizeof(m_BtnMap));

	m_vCur = D3DXVECTOR3(0, 0, 0);
	m_vOld = D3DXVECTOR3(0, 0, 0);
	m_vEps = D3DXVECTOR3(0, 0, 0);


	m_dwTimeDC = 0;
	memset(m_dwBtnBegin, 0, sizeof(m_dwBtnBegin));
	memset(m_dwBtnCnt, 0, sizeof(m_dwBtnCnt));
}

void CInputManager::Update()
{
	// 1. current 값을 old에 복사, 이전 상태를 저장한다.
	memcpy(m_KeyOld, m_KeyCur, sizeof(m_KeyOld));		//Keyboard
	memcpy(m_BtnOld, m_BtnCur, sizeof(m_BtnOld));		// Mouse

	// 2. Current 와 Map의 값을 0으로 초기화
	memset(m_KeyCur, 0, sizeof(m_KeyCur));				//Keyboard
	memset(m_KeyMap, 0, sizeof(m_KeyMap));

	memset(m_BtnCur, 0, sizeof(m_BtnCur));				// Mouse
	memset(m_BtnMap, 0, sizeof(m_BtnMap));

	// 3. current 이벤트를 가져옴.
	GetKeyboardState(m_KeyCur);

	m_BtnCur[0] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) ? 1 : 0;	// Left Button
	m_BtnCur[1] = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) ? 1 : 0;	// Right Button
	m_BtnCur[2] = (GetAsyncKeyState(VK_MBUTTON) & 0x8000) ? 1 : 0;	// Middle Button

	//Keyboard
	for (int i = 0; i < MAX_INPUT_KEY; i++)
	{
		//SHORT	vKey = GetKeyboardState(nKey) & 0x8000;		// 현재의 키보드 상태를 읽어온다.
		BYTE vKey = m_KeyCur[i] & 0x80;

		m_KeyCur[i] = (vKey) ? 1 : 0;

		int iOld = m_KeyOld[i];
		int iCur = m_KeyCur[i];

		if (0 == iOld && 1 == iCur) m_KeyMap[i] = INPUT_DOWN;
		else if (1 == iOld && 0 == iCur) m_KeyMap[i] = INPUT_UP;
		else if (1 == iOld && 1 == iCur) m_KeyMap[i] = INPUT_PRESS;
		else m_KeyMap[i] = INPUT_NONE;
	}

	//Mouse
	for (int i = 0; i < MAX_INPUT_BTN; i++)
	{
		int iOld = m_BtnOld[i];
		int iCur = m_BtnCur[i];

		if (0 == iOld && 1 == iCur) m_BtnMap[i] = INPUT_DOWN;
		else if (1 == iOld && 0 == iCur) m_BtnMap[i] = INPUT_UP;
		else if (1 == iOld && 1 == iCur) m_BtnMap[i] = INPUT_PRESS;
		else m_BtnMap[i] = INPUT_NONE;
	}

	//Update Mouse
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(m_hWnd, &pt);

	m_vOld.x = m_vCur.x;
	m_vOld.y = m_vCur.y;

	m_vCur.x = float(pt.x);
	m_vCur.y = float(pt.y);

	m_vEps = m_vCur - m_vOld;
	m_vOld.z = m_vCur.z;

	//Double Click Check
	DWORD dwBtnCur = GetTickCount();

	for (int i = 0; i < MAX_INPUT_BTN; i++)
	{
		if (m_BtnMap[i] == INPUT_DOWN)
		{
			if (m_dwBtnCnt[i] == 1)
			{
				if ((dwBtnCur - m_dwBtnBegin[i]) >= m_dwTimeDC)
				{
					m_dwBtnCnt[i] = 0;
				}
			}

			++m_dwBtnCnt[i];

			if (m_dwBtnCnt[i] == 1)
			{
				m_dwBtnBegin[i] = dwBtnCur;
			}
		}

		if (m_BtnMap[i] == INPUT_UP)
		{
			if (m_dwBtnCnt[i] == 1)
			{
				if ((dwBtnCur - m_dwBtnBegin[i]) >= m_dwTimeDC)
				{
					m_dwBtnCnt[i] = 0;
				}
			}
			else if (m_dwBtnCnt[i] == 2)
			{
				if ((dwBtnCur - m_dwBtnBegin[i]) <= m_dwTimeDC)
				{
					m_BtnMap[i] = INPUT_DBCLC;
				}
				m_dwBtnCnt[i] = 0;
			}
		}
	}
}

CInputManager* CInputManager::GetInstance()
{
	if (!m_pInputManager)
		m_pInputManager = new CInputManager();

	return m_pInputManager;
}

BOOL CInputManager::KeyDown(int iKey)
{
	return m_KeyMap[iKey] == INPUT_DOWN;
}

BOOL CInputManager::KeyUp(int iKey)
{
	return m_KeyMap[iKey] == INPUT_UP;
}

BOOL CInputManager::KeyPress(int iKey)
{
	return m_KeyMap[iKey] == INPUT_PRESS;
}

BOOL CInputManager::KeyState(int iKey)
{
	return m_KeyMap[iKey];
}

BOOL CInputManager::BtnDown(int iBtn)
{
	return m_BtnMap[iBtn] == INPUT_DOWN;
}

BOOL CInputManager::BtnUp(int iBtn)
{
	return m_BtnMap[iBtn] == INPUT_UP;
}

BOOL CInputManager::BtnPress(int iBtn)
{
	return m_BtnMap[iBtn] == INPUT_PRESS;
}

BOOL CInputManager::BtnState(int iBtn)
{
	return m_BtnMap[iBtn];
}

D3DXVECTOR3 CInputManager::GetMousePos()
{
	return m_vCur;
}

D3DXVECTOR3 CInputManager::GetMouseEps()
{
	return m_vEps;
}

void CInputManager::AddZ(int d)
{
	m_vOld.z = m_vCur.z;
	m_vCur.z += float(d);
}

LRESULT CInputManager::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (WM_MOUSEWHEEL == msg)
	{
		int c = HIWORD(wParam);
		int d = LOWORD(wParam);

		AddZ(short(HIWORD(wParam)));
	}

	return TRUE;
}


