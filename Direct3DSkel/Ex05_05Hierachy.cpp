#include "stdafx.h"
#include "Ex05_05Hierachy.h"


CEx05_05Hierachy::CEx05_05Hierachy()
{
}


CEx05_05Hierachy::~CEx05_05Hierachy()
{
}

HRESULT CEx05_05Hierachy::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	//정점버퍼 관련
	Vertex vertices[] =
	{
		{ -1, 1, 1, 0xffff0000 },		/// v0
		{ 1, 1, 1, 0xff00ff00 },		/// v1
		{ 1, 1, -1, 0xff0000ff },		/// v2
		{ -1, 1, -1, 0xffffff00 },		/// v3

		{ -1, -1, 1, 0xff00ffff },		/// v4
		{ 1, -1, 1, 0xffff00ff },		/// v5
		{ 1, -1, -1, 0xff000000 },		/// v6
		{ -1, -1, -1, 0xffffffff },		/// v7
	};

	if (FAILED(m_pdev->CreateVertexBuffer(8 * sizeof(Vertex), D3DUSAGE_WRITEONLY, Vertex::FVF, D3DPOOL_DEFAULT, &m_pVB, NULL)))
		return E_FAIL;

	VOID* pVertices;
	if (FAILED(m_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
		return E_FAIL;
	memcpy(pVertices, vertices, sizeof(vertices));
	m_pVB->Unlock();

	//인덱스버퍼 관련
	Index indices[] = 
	{
		{ 0, 1, 2 }, { 0, 2, 3 },	/// 윗면
		{ 4, 6, 5 }, { 4, 7, 6 },	/// 아랫면
		{ 0, 3, 7 }, { 0, 7, 4 },	/// 왼면
		{ 1, 5, 6 }, { 1, 6, 2 },	/// 오른면
		{ 3, 2, 6 }, { 3, 6, 7 },	/// 앞면
		{ 0, 4, 5 }, { 0, 5, 1 }	/// 뒷면
	};

	if(FAILED(m_pdev->CreateIndexBuffer(12 * sizeof(Index), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIB, NULL)))
	{
		return E_FAIL;
	}

	VOID* pIndices;
	if (FAILED(m_pIB->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		return E_FAIL;
	memcpy(pIndices, indices, sizeof(indices));
	m_pIB->Unlock();

	return S_OK;
}

void CEx05_05Hierachy::Release()
{
	if (m_pVB)
	{
		m_pVB->Release();
		m_pVB = NULL;
	}

	if (m_pIB)
	{
		m_pIB->Release();
		m_pIB = NULL;
	}
}

void CEx05_05Hierachy::Render()
{
	if (m_pdev)
	{
		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);

		D3DXMATRIX mtWorld;

		mtWorld = m_mtRParent * m_mtTMParent;
		DrawMesh(&mtWorld);

		mtWorld = m_mtRChild*m_mtTMChild * m_mtRParent*m_mtTMParent;
		DrawMesh(&mtWorld);
	}
}

void CEx05_05Hierachy::Update()
{
	if (m_pdev)
	{
		D3DXMatrixIdentity(&m_mtTMParent);
		static float y = 0.0f;
		static float z = 0.0f;
		y += 0.05f;
		D3DXMatrixRotationY(&m_mtRParent, y);
		//D3DXMatrixRotationY(&m_mtRParent, GetTickCount() / 500.0f);

		z += 0.05f;
		D3DXMatrixTranslation(&m_mtTMChild, 3, 3, 3);
		D3DXMatrixRotationZ(&m_mtRChild, z);
		//D3DXMatrixRotationZ(&m_mtRChild, GetTickCount() / 500.0f);
	}
}

void CEx05_05Hierachy::DrawMesh(D3DXMATRIX* pMat)
{
	m_pdev->SetTransform(D3DTS_WORLD, pMat);
	m_pdev->SetStreamSource(0, m_pVB, 0, sizeof(Vertex));
	m_pdev->SetFVF(Vertex::FVF);
	m_pdev->SetIndices(m_pIB);
	m_pdev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
}
