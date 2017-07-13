#include "stdafx.h"
#include "Ex09_Text02.h"


CEx09_Text02::CEx09_Text02()
{
}


CEx09_Text02::~CEx09_Text02()
{
}

void CEx09_Text02::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	DataClear();

	m_dwExType = dwExType;
	m_pdev = pdev;

	//m_pFont = new CD3DFont(L"Times New Roman", 16, 0);
	//m_pFont->InitDeviceObjects(m_pdev);
	//m_pFont->RestoreDeviceObjects();
}

void CEx09_Text02::Release()
{
	/*if (m_pdev && m_pFont)
	{
	m_pFont->InvalidateDeviceObjects();
	m_pFont->DeleteDeviceObjects();
	delete m_pFont;
	}*/

	DataClear();
}

void CEx09_Text02::OnRender()
{
	//if (m_pFont)
	{
		//if (m_pFont)
		{
			//wchar_t wsTxt[64];
			//mbstowcs(wsTxt, m_szFPS, strlen(m_szFPS));
			//m_pFont->DrawText(20, 20, 0xffffffff, wsTxt);
		}
			//m_pFont->DrawText(20, 20, 0xffffffff, m_szFPS);
	}
}

void CEx09_Text02::OnUpdate()
{
	//if (m_pFont)
	{
		float fTimeDelta = GetTimeDelta();

		m_dwFrameCnt++;
		m_fTimeElapsed += fTimeDelta;

		if (m_fTimeElapsed >= 1.0f)
		{
			m_fFPS = (float)m_dwFrameCnt / m_fTimeElapsed;

			sprintf_s(m_szFPS, sizeof(m_szFPS), "FPS: %.2f", m_fFPS);

			m_fTimeElapsed = 0.0f;
			m_dwFrameCnt = 0;
		}
	}
}

void CEx09_Text02::DataClear()
{
	m_pdev = NULL;

	//m_pFont = NULL;
	m_dwFrameCnt = 0;
	m_fTimeElapsed = 0.0f;
	m_fFPS = 0.0f;
	memset( m_szFPS, 0, sizeof(m_szFPS));

	m_fLastTime = 0.0f;
}

float CEx09_Text02::GetTimeDelta()
{
	float fCurTime = (float)timeGetTime();
	float	fTimeDelta = (fCurTime - m_fLastTime) * 0.001f;

	m_fLastTime = fCurTime;
	return fTimeDelta;
}
