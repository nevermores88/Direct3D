#pragma once
class CEx09_Text01 : public CBaseClass
{
	const int Width = 640;
	const int Height = 480;

	LPD3DXFONT	m_pFont;
	
	DWORD	m_dwFrameCnt;
	float	m_fTimeElapsed;
	float	m_fFPS;
	char	m_szFPS[64];

	float	m_fLastTime;

public:
	CEx09_Text01();
	~CEx09_Text01();

public:
	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();

	void DataClear();
	float	GetTimeDelta();
};

