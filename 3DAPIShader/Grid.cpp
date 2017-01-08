#include "stdafx.h"
#include "Grid.h"


CGrid::CGrid()
{
}


CGrid::~CGrid()
{
}

HRESULT CGrid::Create(LPDIRECT3DDEVICE9 pdev)
{
	m_pdev = pdev;

	m_iVertexNum = 12;

	if (FAILED(m_pdev->CreateVertexBuffer(sizeof(Vertex::FVF)*m_iVertexNum, 0, Vertex::FVF, D3DPOOL_DEFAULT, &m_pVB, NULL)))
		return E_FAIL;

	Vertex* pVertices = NULL;

	if(FAILED(m_pVB->Lock(0, 0, (void**)&pVertices, 0)))
		return E_FAIL;

	// Y
	pVertices[0] = Vertex(-10000.f, 0.f, 0.f, 0xffAA0000);
	pVertices[1] = Vertex(0.f, 0.f, 0.f, 0xffAA0000);

	pVertices[2] = Vertex(0.f, 0.f, 0.f, 0xffFF0000);
	pVertices[3] = Vertex(10000.f, 0.f, 0.f, 0xffFF0000);

	// Y
	pVertices[4] = Vertex(0.f, -10000.f, 0.f, 0xff00AA00);
	pVertices[5] = Vertex(0.f, 0.f, 0.f, 0xff00AA00);

	pVertices[6] = Vertex(0.f, 0.f, 0.f, 0xff00FF00);
	pVertices[7] = Vertex(0.f, 10000.f, 0.f, 0xff00FF00);

	// Z
	pVertices[8] = Vertex(0.f, 0.F, -10000.f, 0xff0000AA);
	pVertices[9] = Vertex(0.f, 0.f, 0.f, 0xff0000AA);

	pVertices[10] = Vertex(0.f, 0.F, 0.f, 0xff0000FF);
	pVertices[11] = Vertex(0.f, 0.F, 10000.f, 0xff0000FF);

	m_pVB->Unlock();

	return 0;
}

void CGrid::Release()
{
	if (m_pVB)
	{
		m_pVB->Release();
		m_pVB = NULL;
	}
}

void CGrid::Render()
{
	if (m_pdev)
	{
		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pdev->SetStreamSource(0, m_pVB, 0, sizeof(Vertex));
		m_pdev->SetFVF(Vertex::FVF);
		m_pdev->DrawPrimitive(D3DPT_LINELIST, 0, 6);
	}
}

void CGrid::Update()
{
	if (m_pdev)
	{
		
	}
}
