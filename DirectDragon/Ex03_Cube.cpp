#include "stdafx.h"
#include "Ex03_Cube.h"


CEx03_Cube::CEx03_Cube()
{
}


CEx03_Cube::~CEx03_Cube()
{
}

void CEx03_Cube::Create( LPDIRECT3DDEVICE9 pdev, DWORD dwExType )
{
	m_dwExType = dwExType;

	m_pdev = pdev;
	VB = 0;
	IB = 0;

	m_pdev->CreateVertexBuffer(8 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&VB, 0);

	m_pdev->CreateIndexBuffer(36 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&IB, 0);

	Vertex* vertices;
	VB->Lock(0, 0, (void**)&vertices, 0);

	// vertices of a unit cube
	vertices[0] = Vertex(-1.0f, -1.0f, -1.0f);
	vertices[1] = Vertex(-1.0f, 1.0f, -1.0f);
	vertices[2] = Vertex(1.0f, 1.0f, -1.0f);
	vertices[3] = Vertex(1.0f, -1.0f, -1.0f);
	vertices[4] = Vertex(-1.0f, -1.0f, 1.0f);
	vertices[5] = Vertex(-1.0f, 1.0f, 1.0f);
	vertices[6] = Vertex(1.0f, 1.0f, 1.0f);
	vertices[7] = Vertex(1.0f, -1.0f, 1.0f);

	WORD* indices;
	IB->Lock(0, 0, (void**)&indices, 0);

	// front side
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	// back side
	indices[6] = 4; indices[7] = 6; indices[8] = 5;
	indices[9] = 4; indices[10] = 7; indices[11] = 6;

	// left side
	indices[12] = 4; indices[13] = 5; indices[14] = 1;
	indices[15] = 4; indices[16] = 1; indices[17] = 0;

	// right side
	indices[18] = 3; indices[19] = 2; indices[20] = 6;
	indices[21] = 3; indices[22] = 6; indices[23] = 7;

	// top
	indices[24] = 1; indices[25] = 5; indices[26] = 6;
	indices[27] = 1; indices[28] = 6; indices[29] = 2;

	// bottom
	indices[30] = 4; indices[31] = 0; indices[32] = 3;
	indices[33] = 4; indices[34] = 3; indices[35] = 7;

	IB->Unlock();
}

void CEx03_Cube::Release()
{
	m_pdev = NULL;
	VB->Release();
	IB->Release();
}

void CEx03_Cube::OnRender()
{
	if (m_pdev)
	{
		m_pdev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		//m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		m_pdev->SetStreamSource(0, VB, 0, sizeof(Vertex));
		m_pdev->SetIndices(IB);
		m_pdev->SetFVF(Vertex::FVF);
		m_pdev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

		m_pdev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}

void CEx03_Cube::OnUpdate()
{
	if (m_pdev)
	{
		D3DXMATRIX Rx, Ry;
		static float x = 0.0f;
		D3DXMatrixRotationX(&Rx, x);//3.14f / 4.0f);
		x += 0.01f;//GetTickCount();

		if (x >= 6.28f)
			x = 0.0f;

		static float y = 0.0f;
		D3DXMatrixRotationY(&Ry, y);
		y += 0.01f;//GetTickCount();

		if (y >= 6.28f)
			y = 0.0f;

		D3DXMATRIX p = Rx * Ry;
		m_pdev->SetTransform(D3DTS_WORLD, &p);
	}
}
