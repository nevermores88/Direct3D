#include "stdafx.h"
#include "Ex05_07Skinning.h"


CEx05_07Skinning::CEx05_07Skinning()
{
}


CEx05_07Skinning::~CEx05_07Skinning()
{
}

HRESULT CEx05_07Skinning::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	Clear();

	m_bAnim = true;
	m_bWireframe = false;

	//�������� ����
	if (FAILED(m_pdev->CreateVertexBuffer(50 * 2 * sizeof(Vertex), 0, Vertex::FVF, D3DPOOL_DEFAULT, &m_pVB, NULL)))
		return E_FAIL;

	Vertex* pVertices;
	if (FAILED(m_pVB->Lock(0, 0, (void**)&pVertices, 0)))
		return E_FAIL;

	for (DWORD i = 0; i < 50; i++)
	{
		FLOAT theta = (2 * D3DX_PI*i) / (50 - 1);

		//�Ʒ��� ����
		pVertices[2 * i + 0].x = sinf(theta);
		pVertices[2 * i + 0].y = -1.0f;
		pVertices[2 * i + 0].z = cosf(theta);
		pVertices[2 * i + 0].b[0] = 1.0f;
		pVertices[2 * i + 0].b[1] = 0.0f;
		pVertices[2 * i + 0].b[2] = 0.0f;
		pVertices[2 * i + 0].index = 0x0000;			/// 0�� ����ġ�� 0�� ����� ������ 1.0��ŭ ����
		pVertices[2 * i + 0].color = 0xffffffff;
		pVertices[2 * i + 0].u = ((FLOAT)i) / (50 - 1);
		pVertices[2 * i + 0].v = 1.0f;

		//���� ����
		pVertices[2 * i + 1].x = sinf(theta);
		pVertices[2 * i + 1].y = 1.0f;
		pVertices[2 * i + 1].z = cosf(theta);
		pVertices[2 * i + 1].b[0] = 0.5f;
		pVertices[2 * i + 1].b[1] = 0.5f;
		pVertices[2 * i + 1].b[2] = 0.0f;
		pVertices[2 * i + 1].index = 0x0001;			/// 0�� ����ġ�� 1�� ����� ������ 0.5��ŭ ����
		pVertices[2 * i + 1].color = 0xff808080;
		pVertices[2 * i + 1].u = ((FLOAT)i) / (50 - 1);
		pVertices[2 * i + 1].v = 0.0f;
	}
	m_pVB->Unlock();

	//�ؽ��� ����
	if (FAILED(D3DXCreateTextureFromFile(m_pdev, "Ex05_07/lake.bmp", &m_pTex)))
		return E_FAIL;

	return S_OK;
}

void CEx05_07Skinning::Release()
{
	if (m_pTex)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}

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

void CEx05_07Skinning::Render()
{
	if (m_pdev)
	{
		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		/// matrix palette ���
		m_pdev->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE);
		/// blend weight�� 4��(��Ÿ �ƴ�! 4��!)
		m_pdev->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_3WEIGHTS);

		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

		/// 0�� ��Ʈ���� �ȷ�Ʈ�� �������(�Ʒ���)
		m_pdev->SetTransform(D3DTS_WORLDMATRIX(0), &m_mt01);
		/// 1�� ��Ʈ���� �ȷ�Ʈ�� ȸ�����(����)
		m_pdev->SetTransform(D3DTS_WORLDMATRIX(1), &m_mt02);

		if (m_bWireframe)
		{
			m_pdev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			m_pdev->SetTexture(0, NULL);
			
			DrawMesh();
		}
		else
		{
			m_pdev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			m_pdev->SetTexture(0, m_pTex);
			m_pdev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			m_pdev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			m_pdev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
			m_pdev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

			DrawMesh();
		}
	}
}

void CEx05_07Skinning::Update()
{
	if (m_pdev)
	{
		KeyInput();

		D3DXMatrixIdentity(&m_mt01);

		/// 0 ~ 2PI ����(0~360��) ���� ��ȭ��Ŵ Fixed Point��� ���

		if (m_bAnim)
		{
			DWORD d = GetTickCount() % ((int)((D3DX_PI * 2) * 1000));
			D3DXMatrixRotationY(&m_mt02, d / 1000.0f);
		}	
	}
}

void CEx05_07Skinning::Clear()
{
	m_pVB = NULL;
	m_pIB = NULL;
	m_pTex = NULL;
	m_bAnim = false;
	m_bWireframe = false;

	D3DXMatrixIdentity(&m_mt01);
	D3DXMatrixIdentity(&m_mt02);
}

void CEx05_07Skinning::DrawMesh()
{
	m_pdev->SetStreamSource(0, m_pVB, 0, sizeof(Vertex));
	m_pdev->SetFVF(Vertex::FVF);
	m_pdev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2 * 50 - 2);
}

void CEx05_07Skinning::KeyInput()
{
	static float fQElapsedTime = 0.0f;
	static float fEElapsedTime = 0.0f;

	fQElapsedTime += 0.01f;
	fEElapsedTime += 0.01f;

	if (GetAsyncKeyState('Q') && fQElapsedTime > 0.5f)
	{
		m_bWireframe = !m_bWireframe;
		fQElapsedTime = 0.0f;
	}

	if (GetAsyncKeyState('E') && fEElapsedTime > 0.5f)
	{
		m_bAnim = !m_bAnim;
		fEElapsedTime = 0.0f;
	}
}
