#include "stdafx.h"
#include "Ex08_IndexBuffer.h"


CEx08_IndexBuffer::CEx08_IndexBuffer()
{
}


CEx08_IndexBuffer::~CEx08_IndexBuffer()
{
}

HRESULT CEx08_IndexBuffer::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	//정점정보
	Vertex vertices[] =
	{
		{ -1, 1, 1, 0xffff0000 },
		{1, 1, 1, 0xff00ff00},
		{1, 1, -1, 0xff0000ff},
		{-1, 1, -1, 0xffffff00},

		{-1, -1, 1, 0xff00ffff},
		{1, -1, 1, 0xffff00ff},
		{1, -1, -1, 0xff000000},
		{-1, -1, -1, 0xffffffff},
	};

	if (FAILED(m_pdev->CreateVertexBuffer(8 * sizeof(Vertex), 0, Vertex::FVF, D3DPOOL_DEFAULT, &m_pVB, NULL)))
		return E_FAIL;

	VOID* pVertices;

	if (FAILED(m_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
		return E_FAIL;

	memcpy(pVertices, vertices, sizeof(vertices));

	m_pVB->Unlock();

	//인덱스 정보
	Index indices[] =
	{
		{ 0, 1, 2 }, {0, 2, 3},
		{ 4, 6, 7 }, {4, 7, 6},
		{ 0, 3, 7 }, {0, 7, 4},
		{ 1, 5, 6 }, {1, 6, 2},
		{ 3, 2, 6 }, {3, 6, 7},
		{ 0, 4, 5 }, {0, 5, 1}
	};

	if (FAILED(m_pdev->CreateIndexBuffer(12 * sizeof(Index), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIB, NULL)))
		return E_FAIL;

	VOID* pIndices;
	if (FAILED(m_pIB->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		return E_FAIL;

	memcpy(pIndices, indices, sizeof(indices));

	m_pIB->Unlock();

	return S_OK;
}

void CEx08_IndexBuffer::Release()
{
	if (m_pVB)
		m_pVB->Release();

	if (m_pIB)
		m_pIB->Release();
}

void CEx08_IndexBuffer::Render()
{
	if (m_pdev)
	{
		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);

		//월드행렬 설정
		D3DXMATRIX mtWorld;
		D3DXMatrixIdentity(&mtWorld);
		D3DXMatrixRotationY(&mtWorld, timeGetTime() / 500.0f);
		m_pdev->SetTransform(D3DTS_WORLD, &mtWorld);

		m_pdev->SetStreamSource(0, m_pVB, 0, sizeof(Vertex));
		m_pdev->SetFVF(Vertex::FVF);
		m_pdev->SetIndices(m_pIB);
		m_pdev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
	}
}

void CEx08_IndexBuffer::Update()
{

}
