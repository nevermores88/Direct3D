#include "stdafx.h"
#include "Ex07_Blending02.h"


CEx07_Blending02::CEx07_Blending02()
{
}


CEx07_Blending02::~CEx07_Blending02()
{
}

void CEx07_Blending02::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;

	m_pdev->CreateVertexBuffer(6 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&m_pBackDropVB,
		0);

	Vertex* v;
	m_pBackDropVB->Lock(0, 0, (void**)&v, 0);

	v[0] = Vertex(-10.0f, -10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[1] = Vertex(-10.0f, 10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = Vertex(10.0f, 10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[3] = Vertex(-10.0f, -10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[4] = Vertex(10.0f, 10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[5] = Vertex(10.0f, -10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	m_pBackDropVB->Unlock();

	//박스 만드는 곳
	m_pCube = new CCube();
	if ( m_pCube )
		m_pCube->Create(m_pdev);

	D3DXCreateTextureFromFile(
		m_pdev,
		L"Ex07/cratewalpha.dds",
		&m_pCrateTex);

	D3DXCreateTextureFromFile(
		m_pdev,
		L"Ex07/lobbyxpos.jpg",
		&m_pBackDropTex);
}

void CEx07_Blending02::Release()
{
	if (m_pCube)
	{
		m_pCube->Release();
		delete m_pCube;
	}

	if (m_pCrateTex)
		m_pCrateTex->Release();

	if (m_pBackDropVB)
		m_pBackDropVB->Release();

	if (m_pBackDropTex)
		m_pBackDropTex->Release();

	DataClear();
}

void CEx07_Blending02::OnRender()
{
	if (m_pdev)
	{
		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);

		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

		m_pdev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		m_pdev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		
		m_pdev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pdev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		//draw back drop
		D3DXMATRIX matIdentity;
		D3DXMatrixIdentity(&matIdentity);
		m_pdev->SetStreamSource(0, m_pBackDropVB, 0, sizeof(Vertex));
		m_pdev->SetFVF(Vertex::FVF);
		m_pdev->SetTexture(0, m_pBackDropTex);
		m_pdev->SetTransform(D3DTS_WORLD, &matIdentity);
		m_pdev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

		//draw cube;
		m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		
		if ( m_pCube )
			m_pCube->Draw(&m_CubeMat, NULL, m_pCrateTex);

		m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}
}

void CEx07_Blending02::OnUpdate()
{
	if (m_pdev)
	{
		D3DXMATRIX	xRot;
		D3DXMatrixRotationX(&xRot, D3DX_PI*0.2f);

		static float y = 0.0f;
		D3DXMATRIX yRot;
		D3DXMatrixRotationY(&yRot, y);

		y += 0.01f;

		D3DXMatrixMultiply(&m_CubeMat, &xRot, &yRot);

	}
}

void CEx07_Blending02::DataClear()
{
	m_pdev = NULL;

	m_pCrateTex = NULL;
	memset( m_CubeMat, 0, sizeof( m_CubeMat));

	m_pBackDropVB = NULL;
	m_pBackDropTex = NULL;
}
