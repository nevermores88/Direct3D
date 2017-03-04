#include "stdafx.h"
#include "Basic_3dapi_07_06.h"


CBasic_3dapi_07_06::CBasic_3dapi_07_06()
{
}


CBasic_3dapi_07_06::~CBasic_3dapi_07_06()
{
}

HRESULT CBasic_3dapi_07_06::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pVertices[0] = Vertex(-21, -21, 0, 0x66FF00FF);
	m_pVertices[1] = Vertex(-21, 21, 0, 0x66FF00FF);
	m_pVertices[2] = Vertex(21, 21, 0, 0x66FF00FF);
	m_pVertices[3] = Vertex(21, -21, 0, 0x66FF00FF);

	D3DXCreateTeapot(m_pdev, &m_Teapot, NULL);

	return S_OK;
}

void CBasic_3dapi_07_06::Release()
{
	if (m_Teapot)
	{
		m_Teapot->Release();
		m_Teapot = NULL;
	}
}

void CBasic_3dapi_07_06::Render()
{
	if (m_pdev)
	{
		static	D3DXMATRIX	mtIdentity(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

		D3DXMATRIX	mtWorld;
		D3DXMatrixScaling(&mtWorld, 5, 5, 5);

		DWORD dwFunc = 0;
		m_pdev->GetRenderState(D3DRS_ZFUNC, &dwFunc);

		// 깊이만 쓴다.
		//m_pdev->SetRenderState(D3DRS_COLORWRITEENABLE, 0);
		m_pdev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_pdev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		m_pdev->SetFVF(Vertex::FVF);
		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_pVertices, sizeof(Vertex));

		// 스텐실을 기록한다.
		//	m_pdev->SetRenderState(D3DRS_STENCILENABLE, TRUE);
		//	m_pdev->SetRenderState(D3DRS_STENCILREF, 0X1);
		//	m_pdev->SetRenderState(D3DRS_STENCILMASK, 0X0000FFFF);
		//	m_pdev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS );
		//	m_pdev->SetRenderState(D3DRS_STENCILZFAIL,     D3DSTENCILOP_KEEP);
		//	m_pdev->SetRenderState(D3DRS_STENCILFAIL,      D3DSTENCILOP_KEEP);
		//	m_pdev->SetRenderState(D3DRS_STENCILPASS,      D3DSTENCILOP_REPLACE);

		m_pdev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		m_pdev->SetRenderState(D3DRS_COLORWRITEENABLE, 0XF);
		m_pdev->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATER);

		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pdev->SetTransform(D3DTS_WORLD, &mtWorld);
		m_Teapot->DrawSubset(0);

		m_pdev->SetTransform(D3DTS_WORLD, &mtIdentity);

		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		// 깊이가 같고, 스텐실 레퍼런스가 0X1인 부분만 색칠한다.
		//	m_pdev->SetRenderState(D3DRS_STENCILENABLE, TRUE);
		//	m_pdev->SetRenderState(D3DRS_STENCILREF, 0X1);
		//	m_pdev->SetRenderState(D3DRS_STENCILMASK, 0X0000FFFF);
		//	m_pdev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
		//	m_pdev->SetRenderState(D3DRS_STENCILZFAIL,     D3DSTENCILOP_KEEP);
		//	m_pdev->SetRenderState(D3DRS_STENCILFAIL,      D3DSTENCILOP_KEEP);
		//	m_pdev->SetRenderState(D3DRS_STENCILPASS,      D3DSTENCILOP_KEEP);

		//	m_pdev->SetRenderState(D3DRS_ZFUNC,  D3DCMP_EQUAL);
		//	m_pdev->SetFVF(VtxD::FVF);
		//	m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_Plan, sizeof(VtxD));

		m_pdev->SetRenderState(D3DRS_STENCILENABLE, FALSE);
		m_pdev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		m_pdev->SetRenderState(D3DRS_ZFUNC, dwFunc);

	}
}

void CBasic_3dapi_07_06::Update()
{

}
