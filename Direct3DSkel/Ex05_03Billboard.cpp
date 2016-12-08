#include "stdafx.h"
#include "Ex05_03Billboard.h"


CEx05_03Billboard::CEx05_03Billboard()
{
	m_pWater = NULL;
	m_pCam = NULL;

	memset(m_pTexBillboard, 0, sizeof(m_pTexBillboard));

	m_dwMouseX = 0;
	m_dwMouseY = 0;
	m_bBillboard = TRUE;
	m_bWireframe = FALSE;
}


CEx05_03Billboard::~CEx05_03Billboard()
{
}

HRESULT CEx05_03Billboard::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	//������Ʈ ����
	m_pWater = new CEx05_02Water();
	m_pWater->Create(pdev, 64, 64, 100);

	m_pCam = new CEx05_02Camera();

	//�ؽ��� ����
	D3DXCreateTextureFromFile(m_pdev, L"Ex05_03/tree01S.dds", &m_pTexBillboard[0]);
	D3DXCreateTextureFromFile(m_pdev, L"Ex05_03/tree02S.dds", &m_pTexBillboard[1]);
	D3DXCreateTextureFromFile(m_pdev, L"Ex05_03/tree35S.dds", &m_pTexBillboard[2]);
	D3DXCreateTextureFromFile(m_pdev, L"Ex05_03/explosion.jpg", &m_pTexBillboard[3]);

	//������ ���콺 ��ġ ����
	POINT pt;
	GetCursorPos(&pt);
	m_dwMousePos[0] = pt.x; m_dwMousePos[1] = pt.y;

	return S_OK;
}

void CEx05_03Billboard::Release()
{
	if (m_pWater)
	{
		m_pWater->Release();
		delete m_pWater;
		m_pWater = NULL;
	}

	if (m_pCam)
	{
		delete m_pCam;
		m_pCam = NULL;
	}

	for (int i = 0; i < 4; i++)
	{
		if (m_pTexBillboard[i])
		{
			m_pTexBillboard[i]->Release();
			m_pTexBillboard[i] = NULL;
		}
	}
}

void CEx05_03Billboard::Render()
{
	if (m_pdev)
	{
		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);

		m_pWater->Render();

		// ����ä���� ����ؼ� �����ؽ�ó ȿ���� ����
		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

		m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pdev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pdev->SetRenderState(D3DRS_ALPHAREF, 0x08);
		m_pdev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

		struct UV
		{
			float u, v;
		};

		//������ ����
		static Vertex vtx[4] =
		{
			{ -1, 0, 0, 0x80ffffff, 0, 1 },
			{ -1, 4, 0, 0x80ffffff, 0, 0 },
			{ 1, 0, 0, 0x80ffffff, 1, 1 },
			{ 1, 4, 0, 0x80ffffff, 1, 0 }
		};

		static UV tblUV[16][4] =
		{
			{ { 0.00f, 0.25f }, { 0.00f, 0.00f }, { 0.25f, 0.25f }, { 0.25f, 0.00f } },
			{ { 0.25f, 0.25f }, { 0.25f, 0.00f }, { 0.50f, 0.25f }, { 0.50f, 0.00f } },
			{ { 0.50f, 0.25f }, { 0.50f, 0.00f }, { 0.75f, 0.25f }, { 0.75f, 0.00f } },
			{ { 0.75f, 0.25f }, { 0.75f, 0.00f }, { 1.00f, 0.25f }, { 1.00f, 0.00f } },

			{ { 0.00f, 0.50f }, { 0.00f, 0.25f }, { 0.25f, 0.50f }, { 0.25f, 0.25f } },
			{ { 0.25f, 0.50f }, { 0.25f, 0.25f }, { 0.50f, 0.50f }, { 0.50f, 0.25f } },
			{ { 0.50f, 0.50f }, { 0.50f, 0.25f }, { 0.75f, 0.50f }, { 0.75f, 0.25f } },
			{ { 0.75f, 0.50f }, { 0.75f, 0.25f }, { 1.00f, 0.50f }, { 1.00f, 0.25f } },

			{ { 0.00f, 0.75f }, { 0.00f, 0.50f }, { 0.25f, 0.75f }, { 0.25f, 0.50f } },
			{ { 0.25f, 0.75f }, { 0.25f, 0.50f }, { 0.50f, 0.75f }, { 0.50f, 0.50f } },
			{ { 0.50f, 0.75f }, { 0.50f, 0.50f }, { 0.75f, 0.75f }, { 0.75f, 0.50f } },
			{ { 0.75f, 0.75f }, { 0.75f, 0.50f }, { 1.00f, 0.75f }, { 1.00f, 0.50f } },

			{ { 0.00f, 1.00f }, { 0.00f, 0.75f }, { 0.25f, 1.00f }, { 0.25f, 0.75f } },
			{ { 0.25f, 1.00f }, { 0.25f, 0.75f }, { 0.50f, 1.00f }, { 0.50f, 0.75f } },
			{ { 0.50f, 1.00f }, { 0.50f, 0.75f }, { 0.75f, 1.00f }, { 0.75f, 0.75f } },
			{ { 0.75f, 1.00f }, { 0.75f, 0.75f }, { 1.00f, 1.00f }, { 1.00f, 0.75f } },
		};

		D3DXMATRIX mtBillboard;
		D3DXMatrixIdentity(&mtBillboard);

		static int nStep = 0;
		static DWORD	nTick = 0;

		if ((GetTickCount() - nTick) > 100)
		{
			nStep = (nStep++) % 16;
			nTick = GetTickCount();
		}

		m_pdev->SetTexture(1, NULL);
		m_pdev->SetFVF(Vertex::FVF);

		//Y�� ������
		if (m_bBillboard)
		{
			// Y�� ȸ������� _11, _13, _31, _33�� ��Ŀ� ȸ������ ����
			// ī�޶��� Y�� ȸ����İ��� �о ������� ����� X,Z���� ������
			// Y�� ȸ�� ������ ����� ����� �ִ�
			mtBillboard._11 = m_pCam->GetViewMatrix()->_11;
			mtBillboard._13 = m_pCam->GetViewMatrix()->_13;
			mtBillboard._31 = m_pCam->GetViewMatrix()->_31;
			mtBillboard._33 = m_pCam->GetViewMatrix()->_33;
			//memcpy(&mtBillboard, m_pCam->GetViewMatrix(), sizeof(mtBillboard));
			D3DXMatrixInverse(&mtBillboard, NULL, &mtBillboard);
		}

		// �������� ��ǥ�� �ٲ㰡�� ��´�
		m_pdev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pdev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		for (int z = 0; z <= 40; z += 5)
		{
			for (int x = 0; x <= 40; x += 5)
			{
				mtBillboard._41 = x - 20;
				mtBillboard._42 = 0;
				mtBillboard._43 = z -20;
				m_pdev->SetTexture(0, m_pTexBillboard[(x + z) % 4]);
				if ((x + z) % 4 == 3)
					continue;
				else
				{
					vtx[0].u = 0;
					vtx[0].v = 1;
					vtx[1].u = 0;
					vtx[1].v = 0;
					vtx[2].u = 1;
					vtx[2].v = 1;
					vtx[3].u = 1;
					vtx[3].v = 0;
					m_pdev->SetTransform(D3DTS_WORLD, &mtBillboard);
					m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtx, sizeof(Vertex));
				}
			}
		}

		//��ü ������(���� �̹���)
		m_pdev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
		m_pdev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		m_pdev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		if (m_bBillboard)
		{
			mtBillboard = *m_pCam->GetViewMatrix();
			D3DXMatrixInverse(&mtBillboard, NULL, &mtBillboard);
		}

		for (int z = 40; z >= 0; z -= 5)
		{
			for (int x = 40; x >= 0; x -= 5)
			{
				mtBillboard._41 = x - 20;
				mtBillboard._42 = 0;
				mtBillboard._43 = z - 20;
				m_pdev->SetTexture(0, m_pTexBillboard[(x + z) % 4]);
				if ((x + z) % 4 == 3)
				{
					vtx[0].u = tblUV[nStep][0].u;
					vtx[0].v = tblUV[nStep][0].v;
					vtx[1].u = tblUV[nStep][1].u;
					vtx[1].v = tblUV[nStep][1].v;
					vtx[2].u = tblUV[nStep][2].u;
					vtx[2].v = tblUV[nStep][2].v;
					vtx[3].u = tblUV[nStep][3].u;
					vtx[3].v = tblUV[nStep][3].v;

					m_pdev->SetTransform(D3DTS_WORLD, &mtBillboard);
					m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtx, sizeof(Vertex));
				}
			}
		}

		D3DXMatrixIdentity(&m_mtWorld);
		m_pdev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pdev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		m_pdev->SetTransform(D3DTS_WORLD, &m_mtWorld);
	}
}

void CEx05_03Billboard::Update()
{
	if (m_pdev)
	{
		/**-----------------------------------------------------------------------------
		* ���콺 �Է�ó��
		*------------------------------------------------------------------------------
		*/
		POINT pt;
		float fDelta = 0.001f;

		GetCursorPos(&pt);
		int dx = pt.x - m_dwMousePos[0];
		int dy = pt.y - m_dwMousePos[1];

		m_pCam->RotateLocalX(dy * fDelta);	// ���콺�� Y�� ȸ������ 3D world��  X�� ȸ����
		m_pCam->RotateLocalY(dx * fDelta);	// ���콺�� X�� ȸ������ 3D world��  Y�� ȸ����

		// ���콺�� �������� �߾����� �ʱ�ȭ
		//	SetCursor( NULL );	// ���콺�� ��Ÿ���� �ʰ� �ʴ�.
		RECT	rc;

		GetClientRect(g_pGame->GetHWND(), &rc);
		pt.x = (rc.right - rc.left) / 2;
		pt.y = (rc.bottom - rc.top) / 2;
		ClientToScreen(g_pGame->GetHWND(), &pt);
		SetCursorPos(pt.x, pt.y);
		m_dwMousePos[0] = pt.x;
		m_dwMousePos[1] = pt.y;

		/**-----------------------------------------------------------------------------
		* Ű���� �Է� ó��
		*------------------------------------------------------------------------------
		*/
		if (GetAsyncKeyState('A')) m_pCam->MoveLocalZ(0.5f);
		if (GetAsyncKeyState('Z')) m_pCam->MoveLocalZ(-0.5f);

		D3DXMATRIXA16*	pMatView = m_pCam->GetViewMatrix();		// ī�޶� ����� ��´�.
		m_pdev->SetTransform(D3DTS_VIEW, pMatView);
	}
}
