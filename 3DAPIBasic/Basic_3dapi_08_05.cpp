#include "stdafx.h"
#include "Basic_3dapi_08_05.h"


CBasic_3dapi_08_05::CBasic_3dapi_08_05()
{
}


CBasic_3dapi_08_05::~CBasic_3dapi_08_05()
{
}

HRESULT CBasic_3dapi_08_05::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pVertices = new Vertex[50 * 2];

	for (int i = 0; i < 50; i++)
	{
		FLOAT theta = (2 * D3DX_PI*i) / (50 - 1);
		m_pVertices[2 * i + 1].p = D3DXVECTOR3(sinf(theta), -1.0f, cosf(theta));
		m_pVertices[2 * i + 0].p = D3DXVECTOR3(sinf(theta), 1.0f, cosf(theta));

		m_pVertices[2 * i + 0].p *= 100;
		m_pVertices[2 * i + 1].p *= 100;

		m_pVertices[2 * i + 0].p.y += 80;
		m_pVertices[2 * i + 1].p.y += 80;
	}

	D3DDEVICE_CREATION_PARAMETERS		parameters;
	m_pdev->GetCreationParameters(&parameters);

	RECT rc;
	GetClientRect(g_pGame->GetHWND(), &rc);

	float fX = float(rc.right - rc.left);
	float fY = float(rc.bottom - rc.top);

	m_pRHW[0].p = D3DXVECTOR4(0, fY, 0.0f, 1.0f);
	m_pRHW[1].p = D3DXVECTOR4(0, 0, 0.0f, 1.0f);
	m_pRHW[2].p = D3DXVECTOR4(fX, fY, 0.0f, 1.0f);
	m_pRHW[3].p = D3DXVECTOR4(fX, 0, 0.0f, 1.0f);

	m_pRHW[0].color = 0xAfFF0000;
	m_pRHW[1].color = 0xAf00FF00;
	m_pRHW[2].color = 0xAf0000FF;
	m_pRHW[3].color = 0xAfFF00FF;

	return S_OK;
}

void CBasic_3dapi_08_05::Release()
{
	if (m_pVertices)
	{
		delete[] m_pVertices;
		m_pVertices = NULL;
	}
}

void CBasic_3dapi_08_05::Render()
{
	if (m_pdev)
	{
		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		m_pdev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pdev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pdev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		m_pdev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pdev->SetRenderState(D3DRS_FOGENABLE, FALSE);

		m_pdev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);			// Z-buffer를 갱신하지 않는다.
		m_pdev->SetRenderState(D3DRS_COLORWRITEENABLE, FALSE);			// 색상 버퍼를 갱신하지 않는다.

		//원통 앞면 설정
		m_pdev->SetRenderState(D3DRS_STENCILENABLE, TRUE);
		m_pdev->SetRenderState(D3DRS_STENCILREF, 0x1);
		m_pdev->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
		m_pdev->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
		m_pdev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);

		m_pdev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
		m_pdev->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
		m_pdev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);		//스텐실값 증가

		//원통 앞면 그리기
		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_pdev->SetTexture(0, NULL);
		m_pdev->SetFVF(Vertex::FVF);
		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2 * 50 - 2, m_pVertices, sizeof(Vertex));

		//원통 뒷면 설정
		m_pdev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);

		m_pdev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
		m_pdev->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
		m_pdev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_DECR);		//스텐실값 감소

		//원통 뒷면 그리기
		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		m_pdev->SetTexture(0, NULL);
		m_pdev->SetFVF(Vertex::FVF);
		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2 * 50 - 2, m_pVertices, sizeof(Vertex));

		//색상 버퍼 및 Z버퍼 갱신하도록 수정
		m_pdev->SetRenderState(D3DRS_COLORWRITEENABLE, 0xF);
		m_pdev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		//RHW로 화면 전체를 그린다.
		m_pdev->SetRenderState(D3DRS_STENCILENABLE, TRUE);
		m_pdev->SetRenderState(D3DRS_STENCILREF, 0x1);
		m_pdev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL);		//1 ≤ 스텐실 버퍼 값
		
		m_pdev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
		
		m_pdev->SetTexture(0, NULL);
		m_pdev->SetFVF(VertexRHW::FVF);
		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_pRHW, sizeof(VertexRHW));

		m_pdev->SetRenderState(D3DRS_STENCILENABLE, FALSE);

		/*m_pdev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pdev->SetTexture(0, NULL);
		m_pdev->SetFVF(Vertex::FVF);
		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2 * 50 - 2, m_pVertices, sizeof(Vertex));*/

	}
}

void CBasic_3dapi_08_05::Update()
{
	if (m_pdev)
	{
		if (::GetAsyncKeyState(VK_UP))
		{
			m_mtObj._43 += 1.0f;
		}
		else if (::GetAsyncKeyState(VK_DOWN))
		{
			m_mtObj._43 -= 1.0f;
		}
		else if (::GetAsyncKeyState(VK_RIGHT))
		{
			m_mtObj._41 += 1.0f;
		}
		else if (::GetAsyncKeyState(VK_LEFT))
		{
			m_mtObj._41 -= 1.0f;
		}
	}
}
