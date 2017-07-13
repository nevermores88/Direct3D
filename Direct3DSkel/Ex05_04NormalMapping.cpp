#include "stdafx.h"
#include "Ex05_04NormalMapping.h"

CEx05_04NormalMapping::CEx05_04NormalMapping()
{
}


CEx05_04NormalMapping::~CEx05_04NormalMapping()
{
}

HRESULT CEx05_04NormalMapping::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	//���� ����
	Vertex vertices[] =
	{
		{ -1, 1, 0, 0xffffffff, 0, 0 },		/// v0
		{ 1, 1, 0, 0xffffffff, 1, 0 },		/// v1
		{ -1, -1, 0, 0xffffffff, 0, 1 },		/// v2
		{ 1, -1, 0, 0xffffffff, 1, 1 },		/// v3
	};

	if (FAILED(m_pdev->CreateVertexBuffer(4 * sizeof(Vertex), 0, Vertex::FVF, D3DPOOL_DEFAULT, &m_pVB, NULL)))
		return E_FAIL;

	void* pVertices;

	if (FAILED(m_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
		return E_FAIL;
	memcpy(pVertices, vertices, sizeof(vertices));
	m_pVB->Unlock();


	//�ؽ��� ����
	/// ���� �ؽ�ó
	if (FAILED(D3DXCreateTextureFromFile(m_pdev, "Ex05_04/env2.bmp", &m_pTexDiffuse)))
		return E_FAIL;

	/// ������
	if (FAILED(D3DXCreateTextureFromFile(m_pdev, "Ex05_04/normal.bmp", &m_pTexNormal)))
		return E_FAIL;


	return S_OK;
}

void CEx05_04NormalMapping::Release()
{
	if (m_pVB)
	{
		m_pVB->Release();
		m_pVB = NULL;
	}

	if (m_pTexDiffuse)
	{
		m_pTexDiffuse->Release();
		m_pTexDiffuse = NULL;
	}

	if (m_pTexNormal)
	{
		m_pTexNormal->Release();
		m_pTexNormal = NULL;
	}
}

//DWORD VectortoRGBA(D3DXVECTOR3* v, FLOAT fHeight)
//{
//	DWORD r = (DWORD)(127.0f * v->x + 128.0f);
//	DWORD g = (DWORD)(127.0f * v->y + 128.0f);
//	DWORD b = (DWORD)(127.0f * v->z + 128.0f);
//	DWORD a = (DWORD)(255.0f * fHeight);
//
//	return((a << 24L) + (r << 16L) + (g << 8L) + (b << 0L));
//};

void CEx05_04NormalMapping::Render()
{
	if (m_pdev)
	{
		/// ������ ������ �����Ƿ�, ��������� ����.
		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);

		m_pdev->SetTexture(0, m_pTexNormal);		/// 0�� �ؽ��� ���������� �ؽ��� ����(������)
		m_pdev->SetTexture(1, m_pTexDiffuse);		/// 1�� �ؽ��� ���������� �ؽ��� ����(�����)
		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	/// 0�� �ؽ�ó ���������� Ȯ�� ����
		m_pdev->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	/// 1�� �ؽ�ó ���������� Ȯ�� ����
		m_pdev->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);	/// 0�� �ؽ�ó : 0�� �ؽ�ó �ε��� ���
		m_pdev->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);	/// 1�� �ؽ�ó : 0�� �ؽ�ó �ε��� ���
	
		DWORD dwFactor = d3d::VectortoRGBA(&m_vLight, 0.0f);	/// ���͸� RGB��
		m_pdev->SetRenderState(D3DRS_TEXTUREFACTOR, dwFactor);	/// RGB�� ��ȯ�� ���͸� TextureFactor������ ���

		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);		/// �ؽ�ó�� RGB�� �������͸� ����
		m_pdev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DOTPRODUCT3);	/// 
		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

		m_pdev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MULTIPLYADD);	/// ����� �ؽ�ó�� �������� ��� ���
		m_pdev->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pdev->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
		m_pdev->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		m_pdev->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

		m_pdev->SetTransform(D3DTS_WORLD, &m_mtAni);
		m_pdev->SetStreamSource(0, m_pVB, 0, sizeof(Vertex));
		m_pdev->SetFVF(Vertex::FVF);
		m_pdev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

void CEx05_04NormalMapping::Update()
{
	if (m_pdev)
	{
		D3DXMatrixIdentity(&m_mtAni);

		POINT pt;
		GetCursorPos(&pt);			/// Ŀ���� ��ũ�� ��ǥ�� ��´�.
		ScreenToClient(g_pGame->GetHWND(), &pt);	/// ��ũ�� ��ǥ�� Ŭ���̾�Ʈ ��ǥ��� �ٲ۴�.

		const D3DVIEWPORT9* vp = g_pGame->GetViewPort();
		
		m_vLight.x = -(((2.0f * pt.x) / vp->Width) - 1);
		m_vLight.y = -(((2.0f*pt.y) / vp->Height) - 1);
		m_vLight.z = 0.0f;

		if (D3DXVec3Length(&m_vLight) > 1.0f)
			D3DXVec3Normalize(&m_vLight, &m_vLight);
			else
			m_vLight.z = sqrtf(1.0f - m_vLight.x*m_vLight.x - m_vLight.y*m_vLight.y);

		//D3DXVec3Normalize(&m_vLight, &m_vLight);
	}
}
