#include "stdafx.h"
#include "Temp.h"


CTemp::CTemp()
{
}


CTemp::~CTemp()
{
}

void CTemp::Create(LPDIRECT3DDEVICE9 pdev)
{
	m_pdev = pdev;

	m_pdev->CreateVertexBuffer(3 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&m_pVB, 0);

	Vertex* vertices;
	m_pVB->Lock(0, 0, (void**)&vertices, 0);
	m_Vertex[0] = Vertex(-1, -1, 0, D3DXCOLOR(1, 0, 0, 1));
	m_Vertex[1] = Vertex(0, 1, 0, D3DXCOLOR(0, 1, 0, 1));
	m_Vertex[2] = Vertex(1, -1, 0, D3DXCOLOR(0, 0, 1, 1));
	m_pVB->Unlock();
}

void CTemp::Release()
{

}

void CTemp::OnRender()
{
	if (m_pdev)
	{
		m_pdev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
		//m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, m_Vertex, sizeof(Vertex));
	}
}

void CTemp::OnUpdate()
{

}
