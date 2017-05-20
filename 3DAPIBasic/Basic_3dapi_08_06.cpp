#include "stdafx.h"
#include "Basic_3dapi_08_06.h"


CBasic_3dapi_08_06::CBasic_3dapi_08_06()
{
}


CBasic_3dapi_08_06::~CBasic_3dapi_08_06()
{
}

HRESULT CBasic_3dapi_08_06::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	iCase = 1;

	D3DXMATRIX mtZ, mtX;
	D3DXMatrixRotationX(&mtX, D3DXToRadian(90));
	D3DXMatrixRotationZ(&mtZ, D3DXToRadian(-30));

	m_mtObj = mtX * mtZ;
	D3DXCreateCylinder(m_pdev, 60, 60, 250, 40, 20, &m_pMesh, NULL);

	D3DDEVICE_CREATION_PARAMETERS parameters;
	m_pdev->GetCreationParameters(&parameters);

	RECT rc;
	GetClientRect(parameters.hFocusWindow, &rc);

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

	//필드
	float	fW = 150;

	m_pFieldVertices[0] = VertexColor(-fW, 0, -fW);
	m_pFieldVertices[1] = VertexColor(-fW, 0, fW);
	m_pFieldVertices[2] = VertexColor(fW, 0, fW);
	m_pFieldVertices[3] = VertexColor(fW, 0, -fW);

	return S_OK;
}

void CBasic_3dapi_08_06::Release()
{

}

void CBasic_3dapi_08_06::Render()
{
	if (m_pdev)
	{
		//필드
		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pdev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		m_pdev->SetTexture(0, NULL);
		m_pdev->SetFVF(VertexColor::FVF);

		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_pFieldVertices, sizeof(VertexColor));

		//실린더
		static D3DXMATRIX mtI(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pdev->SetRenderState(D3DRS_FOGENABLE, FALSE);

		if (iCase == 0)
		{
			m_pdev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			m_pdev->SetTransform(D3DTS_WORLD, &m_mtObj);

			m_pMesh->DrawSubset(0);

			m_pdev->SetTransform(D3DTS_WORLD, &mtI);
			m_pdev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
		else if (iCase == 1)
		{
			m_pdev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

			// 1. 깊이 색상 버퍼 비활성화
			m_pdev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			m_pdev->SetRenderState(D3DRS_COLORWRITEENABLE, FALSE);

			// 2. 앞면을 그리기 위한 스텐실 설정
			m_pdev->SetRenderState(D3DRS_STENCILENABLE, TRUE);		// 스텐실 활성화
			m_pdev->SetRenderState(D3DRS_STENCILREF, 0x1);
			m_pdev->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
			m_pdev->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
			m_pdev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);	// 테스트는 무조건 통과
			m_pdev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
			m_pdev->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
			m_pdev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);	// 스텐실 값을 1 증가

			// 3. 앞면 렌더링
			m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			m_pdev->SetTransform(D3DTS_WORLD, &m_mtObj);
			m_pMesh->DrawSubset(0);

			// 4. 뒷면 스텐실 설정
			m_pdev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
			m_pdev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
			m_pdev->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
			m_pdev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_DECR);	// 스텐실 값을 1 감소

			// 5. 뒷면 렌더링
			m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
			m_pdev->SetTransform(D3DTS_WORLD, &m_mtObj);
			m_pMesh->DrawSubset(0);

			// 6. 원래 대로 환원
			m_pdev->SetRenderState(D3DRS_COLORWRITEENABLE, 0xF);
			m_pdev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			m_pdev->SetTransform(D3DTS_WORLD, &mtI);

			// 7. 화면 전체를 그린다.
			m_pdev->SetRenderState(D3DRS_STENCILENABLE, TRUE);
			m_pdev->SetRenderState(D3DRS_STENCILREF, 0x1);
			m_pdev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL);	// 1 ≤ 스텐실 버퍼 값
			m_pdev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

			m_pdev->SetFVF(VertexRHW::FVF);
			m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_pRHW, sizeof(VertexRHW));

			// 8. 스텐실을 비활성화
			m_pdev->SetRenderState(D3DRS_STENCILENABLE, FALSE);

			// 원통을 그린다.
			m_pdev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			//m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
			m_pdev->SetTransform(D3DTS_WORLD, &m_mtObj);
			m_pMesh->DrawSubset(0);
			m_pdev->SetTransform(D3DTS_WORLD, &mtI);
			m_pdev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
	}
}

void CBasic_3dapi_08_06::Update()
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

		if (::GetAsyncKeyState('1'))
		{
			iCase = 0;
		}
		else if (::GetAsyncKeyState('2'))
		{
			iCase = 1;
		}
	}
}
