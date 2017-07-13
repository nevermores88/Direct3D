#include "stdafx.h"
#include "Ex09_Text01.h"

CEx09_Text01::CEx09_Text01()
{
}


CEx09_Text01::~CEx09_Text01()
{
}

void CEx09_Text01::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	DataClear();

	m_dwExType = dwExType;
	m_pdev = pdev;

	//LOGFONT	lf;
	D3DXFONT_DESC lf;
	ZeroMemory(&lf, sizeof(lf));
	lf.CharSet = DEFAULT_CHARSET;
	wcscpy_s(lf.FaceName, L"Times New Roman");
	lf.Height = 25;
	lf.Italic = FALSE;
	lf.Width = 12;
	lf.Weight = 500;

	if (FAILED(D3DXCreateFontIndirect(m_pdev, &lf, &m_pFont)))
	{
		::MessageBox(0, L"D3DXCreateFontIndirect() - FAILED", 0, 0);
		::PostQuitMessage(0);
	}

	m_fLastTime = (float)timeGetTime();
}

void CEx09_Text01::Release()
{

}

void CEx09_Text01::OnRender()
{
	if (m_pdev)
	{
		RECT rect = { 0, 0, Width, Height };
		m_pFont->DrawTextA(NULL, m_szFPS, -1, &rect, DT_TOP | DT_LEFT, 0xffffffff);
	}
}

void CEx09_Text01::OnUpdate()
{
	if (m_pdev)
	{
		float fTimeDelta = g_pGame->GetManager()->GetTimeManager()->m_fDeltaTime;//GetTimeDelta();

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

void CEx09_Text01::DataClear()
{
	m_pdev = NULL;

	m_pFont = NULL;

	m_dwFrameCnt = 0;
	m_fTimeElapsed = 0.0f;
	m_fFPS = 0.0f;
	memset(m_szFPS, 0, sizeof(m_szFPS));
}

float CEx09_Text01::GetTimeDelta()
{
	float fCurTime = (float)timeGetTime();
	float	fTimeDelta = (fCurTime - m_fLastTime) * 0.001f;

	m_fLastTime = fCurTime;
	return fTimeDelta;
}
