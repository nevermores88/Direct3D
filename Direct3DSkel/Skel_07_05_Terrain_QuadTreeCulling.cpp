#include "stdafx.h"
#include "Skel_07_05_Terrain_QuadTreeCulling.h"
#include "Skel_Camera.h"
#include "Skel_ZFrustum.h"
#include "Skel_Terrain.h"

CSkel_07_05_Terrain_QuadTreeCulling::CSkel_07_05_Terrain_QuadTreeCulling()
{
}


CSkel_07_05_Terrain_QuadTreeCulling::~CSkel_07_05_Terrain_QuadTreeCulling()
{
}

HRESULT CSkel_07_05_Terrain_QuadTreeCulling::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	D3DXVECTOR3 vScale;
	vScale.x = vScale.z = 1.0f; vScale.y = 0.1f;

	LPSTR lpTex[4] = { "Chapter07/Ex05/tile2.tga", "", "", "" };

	m_pCamera = new CSkel_Camera();
	m_pFrustum = new CSkel_ZFrustum();
	m_pTerrain = new CSkel_Terrain();
	m_pTerrain->Create(m_pdev, &vScale, "Chapter07/Ex05/map129.bmp", lpTex);

	POINT pt;
	GetCursorPos(&pt);
	m_dwMouseX = pt.x;
	m_dwMouseY = pt.y;

	m_bWireFrame = false;
	m_bHideFrustum = true;
	m_bLockFrustum = false;

	D3DXMatrixPerspectiveFovLH(&m_mtFrustumProj, D3DX_PI / 4.0f, 2.03f, 1.0f, 200.0f);

	return S_OK;
}

void CSkel_07_05_Terrain_QuadTreeCulling::Release()
{
	if (m_pHeightTex)
	{
		m_pHeightTex->Release();
		m_pHeightTex = NULL;
	}

	if (m_pDiffuseTex)
	{
		m_pDiffuseTex->Release();
		m_pDiffuseTex = NULL;
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

	if (m_pCamera)
	{
		delete m_pCamera;
	}

	if (m_pFrustum)
	{
		delete m_pFrustum;
	}

	if (m_pTerrain)
	{
		delete m_pTerrain;
	}

	if (m_pHeigtMapVectors)
	{
		delete[] m_pHeigtMapVectors;
	}
}

void CSkel_07_05_Terrain_QuadTreeCulling::Render()
{
	if (m_pdev)
	{
		m_pdev->SetTexture(0, m_pDiffuseTex);
		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

		m_pdev->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);

		m_pdev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

		//DrawMesh();

		m_pTerrain->Draw(m_pFrustum);

		if (!m_bHideFrustum)
			m_pFrustum->Draw(m_pdev);
	}
}

void CSkel_07_05_Terrain_QuadTreeCulling::Update()
{
	if (m_pdev)
	{
		SetupLights();

		ProcessKey();
		ProcessMouse();

		D3DXMATRIX mtView;
		D3DXMATRIX mtViewProj;
		m_pdev->GetTransform(D3DTS_VIEW, &mtView);

		mtViewProj = mtView * m_mtFrustumProj;

		if (!m_bLockFrustum)
			m_pFrustum->Make(&mtViewProj);

		//ProcessFrustumCull();
	}
}

void CSkel_07_05_Terrain_QuadTreeCulling::SetupLights()
{
	D3DMATERIAL9	mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));

	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	m_pdev->SetMaterial(&mtrl);

	D3DXVECTOR3 vLightDir;
	D3DLIGHT9	light;

	ZeroMemory(&light, sizeof(D3DLIGHT9));

	light.Type = D3DLIGHT_DIRECTIONAL;

	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 0.0f;

	light.Range = 1000.0f;

	vLightDir = D3DXVECTOR3(1, 1, 1);
	vLightDir = D3DXVECTOR3(cosf(GetTickCount() / 350.0f), 1.0f, sinf(GetTickCount() / 350.0f));

	D3DXVec3Normalize(&vLightDir, &vLightDir);

	m_pdev->SetLight(0, &light);
	m_pdev->LightEnable(0, true);
	m_pdev->SetRenderState(D3DRS_LIGHTING, true);

	m_pdev->SetRenderState(D3DRS_AMBIENT, 0x00909090);
}

void CSkel_07_05_Terrain_QuadTreeCulling::ProcessMouse()
{
	POINT pt;
	float fDelta = 0.001f;	// ���콺�� �ΰ���, �� ���� Ŀ������ ���� �����δ�.

	GetCursorPos(&pt);
	int dx = pt.x - m_dwMouseX;
	int dy = pt.y - m_dwMouseY;

	m_pCamera->RotateLocalX(dy * fDelta);
	m_pCamera->RotateLocalY(dx * fDelta);
	D3DXMATRIX* pViewMatrix = m_pCamera->GetViewMatrix();
	m_pdev->SetTransform(D3DTS_VIEW, pViewMatrix);

	//���콺�� �������� �߾����� �ʱ�ȭ
	//SetCursor(NULL); //���콺�� ��Ÿ���� �ʰ� �ϴ�.
	RECT rc;
	GetClientRect(g_pGame->GetHWND(), &rc);
	pt.x = (rc.right - rc.left) / 2;
	pt.y = (rc.bottom - rc.top) / 2;
	ClientToScreen(g_pGame->GetHWND(), &pt);
	SetCursorPos(pt.x, pt.y);
	m_dwMouseX = pt.x;
	m_dwMouseY = pt.y;
}

void CSkel_07_05_Terrain_QuadTreeCulling::ProcessKey()
{
	if (GetAsyncKeyState('W'))
		m_pCamera->MoveLocalZ(0.5f);

	if (GetAsyncKeyState('S'))
		m_pCamera->MoveLocalZ(-0.5f);

	if (GetAsyncKeyState('A'))
		m_pCamera->MoveLocalX(-0.5f);

	if (GetAsyncKeyState('D'))
		m_pCamera->MoveLocalX(0.5f);

	if (GetAsyncKeyState(VK_LBUTTON))
		m_bWireFrame = true;

	if (GetAsyncKeyState(VK_RBUTTON))
		m_bWireFrame = false;

	if (GetAsyncKeyState(VK_NUMPAD1))
		m_bHideFrustum = true;

	if (GetAsyncKeyState(VK_NUMPAD2))
		m_bHideFrustum = false;

	if (GetAsyncKeyState(VK_NUMPAD3))
		m_bLockFrustum = true;

	if (GetAsyncKeyState(VK_NUMPAD4))
		m_bLockFrustum = false;
}

void CSkel_07_05_Terrain_QuadTreeCulling::ProcessFrustumCull()
{
	DWORD indices[4];		// �ӽ÷� ������ �ε��� ����
	bool bCullRet[4];			// �ӽ÷� ������ frustum culling�����

	Index idx;
	Index *pIdx;

	if (FAILED(m_pIB->Lock(0, (m_dwXHeight - 1)*(m_dwZHeight - 1) * 2 * sizeof(Index), (void**)&pIdx, 0)))
		return;

	m_iTrianglesNum = 0;

	for (DWORD z = 0; z < m_dwZHeight - 1; z++)
	{
		for (DWORD x = 0; x < m_dwXHeight - 1; x++)
		{
			indices[0] = (z*m_dwXHeight + x);			// ���� ���
			indices[1] = (z*m_dwXHeight + x + 1);			// ���� ���
			indices[2] = ((z + 1)*m_dwXHeight + x);		// ���� �ϴ�
			indices[3] = ((z + 1)*m_dwXHeight + x + 1);		// ���� �ϴ�

			bCullRet[0] = m_pFrustum->IsIn(&m_pHeigtMapVectors[indices[0]]);	// ������� ������ Frustum�ȿ� �ִ°�?
			bCullRet[1] = m_pFrustum->IsIn(&m_pHeigtMapVectors[indices[1]]);	// ������� ������ Frustum�ȿ� �ִ°�?
			bCullRet[2] = m_pFrustum->IsIn(&m_pHeigtMapVectors[indices[2]]);	// �����ϴ� ������ Frustum�ȿ� �ִ°�?

			if (bCullRet[0] | bCullRet[1] | bCullRet[2])	// ���߿� �ϳ��� frustum�ȿ� ������ �������Ѵ�.
			{
				idx._0 = indices[0];
				idx._1 = indices[1];
				idx._2 = indices[2];
				*pIdx++ = idx;

				m_iTrianglesNum++;		//�������� �ﰢ�� ���� ����
			}

			bCullRet[2] = m_pFrustum->IsIn(&m_pHeigtMapVectors[indices[2]]);	// �����ϴ� ������ Frustum�ȿ� �ִ°�?
			bCullRet[1] = m_pFrustum->IsIn(&m_pHeigtMapVectors[indices[1]]);	// ������� ������ Frustum�ȿ� �ִ°�?
			bCullRet[3] = m_pFrustum->IsIn(&m_pHeigtMapVectors[indices[3]]); // �����ϴ� ������ Frustum�ȿ� �ִ°�?

			if (bCullRet[2] | bCullRet[1] | bCullRet[3])	// ���߿� �ϳ��� frustum�ȿ� ������ �������Ѵ�.
			{
				idx._0 = indices[2];
				idx._1 = indices[1];
				idx._2 = indices[3];
				*pIdx++ = idx;

				m_iTrianglesNum++;
			}
		}
	}

	m_pIB->Unlock();
}
