#pragma once

#define MAX_INPUT_KEY 256
#define MAX_INPUT_BTN 8

#define g_pInput CInputManager::GetInstance()

class CInputManager
{
public:
	enum InputState
	{
		INPUT_NONE = 0,
		INPUT_DOWN = 1,
		INPUT_UP = 2,
		INPUT_PRESS = 3,
		INPUT_DBCLC = 4
	};

private:
	static CInputManager* m_pInputManager;

	HWND m_hWnd;

	BYTE m_KeyCur[MAX_INPUT_KEY];
	BYTE m_KeyOld[MAX_INPUT_KEY];
	BYTE m_KeyMap[MAX_INPUT_KEY];

	BYTE m_BtnCur[MAX_INPUT_BTN];	
	BYTE m_BtnOld[MAX_INPUT_BTN];	
	BYTE m_BtnMap[MAX_INPUT_BTN];

	D3DXVECTOR3 m_vCur;		//ÈÙ¸¶¿ì½º z
	D3DXVECTOR3 m_vOld;
	D3DXVECTOR3 m_vEps;

	DWORD m_dwTimeDC;				//Double Click Time Interval
	DWORD m_dwBtnBegin[MAX_INPUT_BTN];	//Double Click Start
	DWORD m_dwBtnCnt[MAX_INPUT_BTN];		//Double Click Cnt

public:
	CInputManager();
	~CInputManager();

	void Create(HWND hWnd);
	void Release();
	void DataClear();

	void Update();

	static CInputManager* GetInstance();

	BOOL KeyDown(int iKey);
	BOOL KeyUp(int iKey);
	BOOL KeyPress(int iKey);
	BOOL KeyState(int iKey);

	BOOL BtnDown(int iBtn);
	BOOL BtnUp(int iBtn);
	BOOL BtnPress(int iBtn);
	BOOL BtnState(int iBtn);

	D3DXVECTOR3 GetMousePos();
	D3DXVECTOR3 GetMouseEps();

	void AddZ(int d);

	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

