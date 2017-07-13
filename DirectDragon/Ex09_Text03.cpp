#include "stdafx.h"
#include "Ex09_Text03.h"


CEx09_Text03::CEx09_Text03()
{
}


CEx09_Text03::~CEx09_Text03()
{
}

void CEx09_Text03::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	DataClear();

	m_dwExType = dwExType;
	m_pdev = pdev;

	HDC hdc = CreateCompatibleDC(0);
	HFONT	hFont;
	HFONT	hFontOld;

	LOGFONT lf;
	ZeroMemory(&lf, sizeof(LOGFONT));

	lf.lfHeight = 25;    // in logical units
	lf.lfWidth = 12;    // in logical units
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = 500;   // boldness, range 0(light) - 1000(bold)
	lf.lfItalic = false;
	lf.lfUnderline = false;
	lf.lfStrikeOut = false;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = 0;
	lf.lfClipPrecision = 0;
	lf.lfQuality = 0;
	lf.lfPitchAndFamily = 0;
	wcscpy_s(lf.lfFaceName, L"Times New Roman"); // font style

	hFont = CreateFontIndirect(&lf);
	hFontOld = (HFONT)SelectObject(hdc, hFont);

	D3DXCreateText(m_pdev, hdc, L"Direct3D", 0.001f, 0.01f, &m_pMeshTxt, 0, 0);

	SelectObject(hdc, hFontOld);
	DeleteObject(hFont);
	DeleteDC(hdc);
}

void CEx09_Text03::Release()
{

}

void CEx09_Text03::OnRender()
{
	if (m_pdev)
	{
		m_pdev->SetLight(0, &m_Light);
		m_pdev->LightEnable(0, true);

		m_pdev->SetRenderState(D3DRS_NORMALIZENORMALS, true);
		m_pdev->SetRenderState(D3DRS_SPECULARENABLE, true);

		m_pdev->SetTransform(D3DTS_WORLD, &m_WorldMat);

		m_pdev->SetMaterial(&m_Mtrl);
		
		if (m_pMeshTxt)
			m_pMeshTxt->DrawSubset(0);
	}
}

void CEx09_Text03::OnUpdate()
{
	if (m_pdev)
	{
		m_LightDir = D3DXVECTOR3(0.0f, -0.5f, 1.0f);
		m_LightColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		ZeroMemory(&m_Light, sizeof(m_Light));
		m_Light.Type = D3DLIGHT_DIRECTIONAL;
		m_Light.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		m_Light.Diffuse = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);
		m_Light.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		m_Light.Direction = m_LightDir;

		ZeroMemory(&m_Mtrl, sizeof(m_Mtrl));
		m_Mtrl.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f)*0.6f;
		m_Mtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		m_Mtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f)*0.3f;
		m_Mtrl.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
		m_Mtrl.Power = 5.0f;

		D3DXMatrixRotationY(&m_YMat, m_fYRot);
		m_fYRot += 0.01f;

		if (m_fYRot >= 6.28f)
			m_fYRot = 0.0f;

		D3DXMatrixTranslation(&m_TMat, -1.6f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&m_WorldMat, &m_TMat, &m_YMat);
	}
}

void CEx09_Text03::DataClear()
{
	m_pdev = NULL;
	m_pMeshTxt = NULL;

	memset(&m_LightDir, 0, sizeof(m_LightDir));
	memset(&m_LightColor, 0, sizeof(m_LightColor));
	memset(&m_Light, 0, sizeof(m_Light));

	memset(&m_Mtrl, 0, sizeof(m_Mtrl));

	m_fYRot = 0.0f;

	memset(m_YMat, 0, sizeof(m_YMat));
	memset(m_TMat, 0, sizeof(m_TMat));
	memset(m_WorldMat, 0, sizeof(m_WorldMat));
}
