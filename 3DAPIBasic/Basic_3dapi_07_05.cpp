#include "stdafx.h"
#include "Basic_3dapi_07_05.h"


CBasic_3dapi_07_05::CBasic_3dapi_07_05()
{
}


CBasic_3dapi_07_05::~CBasic_3dapi_07_05()
{
}

HRESULT CBasic_3dapi_07_05::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pVertices1[0] = Vertex1(-8.f, 6.f, 10.01f, D3DXCOLOR(1, 1, 1, 1));
	m_pVertices1[1] = Vertex1(4.f, 6.f, 10.01f, D3DXCOLOR(1, 1, 1, 0));
	m_pVertices1[2] = Vertex1(4.f, -4.f, 10.01f, D3DXCOLOR(1, 1, 1, 0));
	m_pVertices1[3] = Vertex1(-8.f, -4.f, 10.01f, D3DXCOLOR(1, 1, 1, 1));

	m_pVertices2[0] = Vertex2(-4.f, 4.f, 5.f, D3DXCOLOR(1, 1, 1, 0), 0.f, 0.f);
	m_pVertices2[1] = Vertex2(8.f, 4.f, 5.f, D3DXCOLOR(1, 1, 1, 1), 1.f, 0.f);
	m_pVertices2[2] = Vertex2(8.f, -6.f, 5.f, D3DXCOLOR(1, 1, 1, 1), 1.f, 1.f);
	m_pVertices2[3] = Vertex2(-4.f, -6.f, 5.f, D3DXCOLOR(1, 1, 1, 0), 0.f, 1.f);

	D3DXCreateTextureFromFile(pdev, "Ex07_05/env3_alpha.png", &m_pTex);

	m_dwFunc = D3DCMP_ALWAYS;
	m_iRef = 128;

	return S_OK;
}

void CBasic_3dapi_07_05::Release()
{
	if (m_pTex)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
}

void CBasic_3dapi_07_05::Render()
{
	if (m_pdev)
	{
		m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);

		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		DWORD v;
		m_pdev->GetRenderState(D3DRS_ALPHAFUNC, &v);
		m_pdev->GetRenderState(D3DRS_ALPHAREF, &v);

		// 비교 함수 설정
		if (::GetAsyncKeyState('1') & 0x8000)	m_dwFunc = D3DCMP_NEVER;
		if (::GetAsyncKeyState('2') & 0x8000)	m_dwFunc = D3DCMP_LESS;
		if (::GetAsyncKeyState('3') & 0x8000)	m_dwFunc = D3DCMP_EQUAL;
		if (::GetAsyncKeyState('4') & 0x8000)	m_dwFunc = D3DCMP_LESSEQUAL;
		if (::GetAsyncKeyState('5') & 0x8000)	m_dwFunc = D3DCMP_GREATER;
		if (::GetAsyncKeyState('6') & 0x8000)	m_dwFunc = D3DCMP_NOTEQUAL;
		if (::GetAsyncKeyState('7') & 0x8000)	m_dwFunc = D3DCMP_GREATEREQUAL;
		if (::GetAsyncKeyState('8') & 0x8000)	m_dwFunc = D3DCMP_ALWAYS;

		if (::GetAsyncKeyState(VK_ADD))
		{
			++m_iRef;
			if (m_iRef > 255)	m_iRef = 255;
		}

		if (::GetAsyncKeyState(VK_SUBTRACT))
		{
			--m_iRef;

			if (m_iRef < 0)	m_iRef = 0;
		}

		// Debug: 윈도우 타이틀에 함수와 참조 값을 출력
		char	sMsg[128] = { 0 };
		sprintf(sMsg, "%ld %ld", m_dwFunc, m_iRef);
		SetWindowText(g_pGame->GetHWND(), sMsg);

		// 알파 테스트를 진행한다.
		m_pdev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pdev->SetRenderState(D3DRS_ALPHAFUNC, (DWORD)m_dwFunc);
		m_pdev->SetRenderState(D3DRS_ALPHAREF, (DWORD)m_iRef);

		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		m_pdev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

		m_pdev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pdev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		m_pdev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

		// 알파 블렌딩과 알파 테스트는 전혀 상관이 없다.
		m_pdev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pdev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		m_pdev->SetTexture(0, NULL);
		m_pdev->SetFVF(Vertex1::FVF);
		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_pVertices1, sizeof(Vertex1));


		m_pdev->SetTexture(0, m_pTex);
		m_pdev->SetFVF(Vertex2::FVF);
		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_pVertices2, sizeof(Vertex2));

		m_pdev->SetTexture(0, NULL);

		m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pdev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pdev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		m_pdev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
}

void CBasic_3dapi_07_05::Update()
{

}
