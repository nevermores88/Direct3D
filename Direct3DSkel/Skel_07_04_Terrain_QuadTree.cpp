#include "stdafx.h"
#include "Skel_07_04_Terrain_QuadTree.h"
#include "Skel_Camera.h"
#include "Skel_ZFrustum.h"
#include "Skel_Terrain.h"

CSkel_07_04_Terrain_QuadTree::CSkel_07_04_Terrain_QuadTree()
{
}


CSkel_07_04_Terrain_QuadTree::~CSkel_07_04_Terrain_QuadTree()
{
}

HRESULT CSkel_07_04_Terrain_QuadTree::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	D3DXVECTOR3 vScale;
	vScale.x = vScale.z = 1.0f; vScale.y = 0.1f;

	LPSTR lpTex[4] = { "Chapter07/Ex04/tile2.tga", "", "", "" };

	m_pCamera = new CSkel_Camera();
	m_pFrustum = new CSkel_ZFrustum();
	m_pTerrain = new CSkel_Terrain();
	m_pTerrain->Create(m_pdev, &vScale, "Chapter07/Ex04/map129.bmp", lpTex);

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

void CSkel_07_04_Terrain_QuadTree::Release()
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

void CSkel_07_04_Terrain_QuadTree::Render()
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

		m_pTerrain->Draw();

		if (!m_bHideFrustum)
			m_pFrustum->Draw(m_pdev);
	}
}

void CSkel_07_04_Terrain_QuadTree::Update()
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

//void CSkel_07_04_Terrain_QuadTree::InitTexture()
//{
//	HRESULT hr = 0;
//
//	hr = D3DXCreateTextureFromFileEx(
//		m_pdev,
//		"Chapter07/Ex01/map128.bmp",
//		D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0,
//		D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0,
//		NULL, NULL, &m_pHeightTex);
//
//	if (FAILED(hr))
//		return;
//
//	hr = D3DXCreateTextureFromFile(m_pdev, "Chapter07/Ex01/tile2.tga", &m_pDiffuseTex);
//
//	if (FAILED(hr))
//		return;
//}
//
//void CSkel_07_04_Terrain_QuadTree::InitVertexBuffer()
//{
//	HRESULT hr = 0;
//
//	D3DSURFACE_DESC	desc;
//	D3DLOCKED_RECT	d3drc;
//
//	m_pHeightTex->GetLevelDesc(0, &desc);
//	m_dwXHeight = desc.Width;
//	m_dwZHeight = desc.Height;
//	m_pHeigtMapVectors = new D3DXVECTOR3[m_dwXHeight*m_dwZHeight];
//
//	hr = m_pdev->CreateVertexBuffer(m_dwXHeight*m_dwZHeight*sizeof(Vertex),
//		0, Vertex::FVF, D3DPOOL_DEFAULT, &m_pVB, NULL);
//
//	if (FAILED(hr))
//		return;
//
//	m_pHeightTex->LockRect(0, &d3drc, NULL, D3DLOCK_READONLY);
//
//	VOID* pVertices;
//
//	m_pVB->Lock(0, m_dwXHeight*m_dwZHeight*sizeof(Vertex), (void**)&pVertices, 0);
//
//	Vertex v;
//	Vertex* pV = (Vertex*)pVertices;
//
//	for (DWORD z = 0; z < m_dwZHeight; z++)
//	{
//		for (DWORD x = 0; x < m_dwXHeight; x++)
//		{
//			v.p.x = (float)x - m_dwXHeight / 2.0f;
//			v.p.z = -((float)z - m_dwZHeight / 2.0f);
//
//			v.p.y = (float)((float)(*((LPDWORD)d3drc.pBits + x + z*(d3drc.Pitch / 4)) & 0x000000ff)) / 10.0f;
//			v.n = v.p;
//			D3DXVec3Normalize(&v.n, &v.n);
//
//			v.t.x = (float)x / (m_dwXHeight - 1);
//			v.t.y = (float)z / (m_dwZHeight - 1);
//			*pV++ = v;
//
//			m_pHeigtMapVectors[z*m_dwXHeight + x] = v.p;
//		}
//	}
//
//	m_pVB->Unlock();
//
//	m_pHeightTex->UnlockRect(0);
//}
//
//void CSkel_07_04_Terrain_QuadTree::InitIndexBuffer()
//{
//	m_pdev->CreateIndexBuffer((m_dwXHeight - 1) * (m_dwZHeight - 1) * 2 * sizeof(Index),
//		0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIB, NULL);
//
//	Index i;
//	Index* pI;
//
//	m_pIB->Lock(0, (m_dwXHeight - 1) * (m_dwZHeight - 1) * 2 * sizeof(Index), (void**)&pI, 0);
//
//	for (DWORD z = 0; z < m_dwZHeight - 1; z++)
//	{
//		for (DWORD x = 0; x < m_dwXHeight - 1; x++)
//		{
//			i._0 = (z*m_dwXHeight + x);
//			i._1 = (z*m_dwXHeight + x + 1);
//			i._2 = ((z + 1)*m_dwXHeight + x);
//
//			*pI++ = i;
//
//			i._0 = ((z + 1)*m_dwXHeight + x);
//			i._1 = (z*m_dwXHeight + x + 1);
//			i._2 = ((z + 1)*m_dwXHeight + x + 1);
//
//			*pI++ = i;
//		}
//	}
//
//	m_pIB->Unlock();
//}

void CSkel_07_04_Terrain_QuadTree::SetupLights()
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

//void CSkel_07_04_Terrain_QuadTree::DrawMesh()
//{
//	if (m_pdev)
//	{
//		//m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
//
//		D3DXMatrixIdentity(&m_mtAni);
//
//		m_pdev->SetRenderState(D3DRS_FILLMODE, m_bWireFrame ? D3DFILL_WIREFRAME : D3DFILL_SOLID);
//		m_pdev->SetTransform(D3DTS_WORLD, &m_mtAni);
//		m_pdev->SetStreamSource(0, m_pVB, 0, sizeof(Vertex));
//		m_pdev->SetFVF(Vertex::FVF);
//		m_pdev->SetIndices(m_pIB);
//		m_pdev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwXHeight*m_dwZHeight, 0, m_iTrianglesNum);
//	}
//}

void CSkel_07_04_Terrain_QuadTree::ProcessMouse()
{
	POINT pt;
	float fDelta = 0.001f;	// 마우스의 민감도, 이 값이 커질수록 많이 움직인다.

	GetCursorPos(&pt);
	int dx = pt.x - m_dwMouseX;
	int dy = pt.y - m_dwMouseY;

	m_pCamera->RotateLocalX(dy * fDelta);
	m_pCamera->RotateLocalY(dx * fDelta);
	D3DXMATRIX* pViewMatrix = m_pCamera->GetViewMatrix();
	m_pdev->SetTransform(D3DTS_VIEW, pViewMatrix);

	//마우스를 윈도우의 중앙으로 초기화
	//SetCursor(NULL); //마우스를 나타나지 않게 하다.
	RECT rc;
	GetClientRect(g_pGame->GetHWND(), &rc);
	pt.x = (rc.right - rc.left) / 2;
	pt.y = (rc.bottom - rc.top) / 2;
	ClientToScreen(g_pGame->GetHWND(), &pt);
	SetCursorPos(pt.x, pt.y);
	m_dwMouseX = pt.x;
	m_dwMouseY = pt.y;
}

void CSkel_07_04_Terrain_QuadTree::ProcessKey()
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

void CSkel_07_04_Terrain_QuadTree::ProcessFrustumCull()
{
	DWORD indices[4];		// 임시로 저장할 인덱스 정보
	bool bCullRet[4];			// 임시로 저장할 frustum culling결과값

	Index idx;
	Index *pIdx;

	if (FAILED(m_pIB->Lock(0, (m_dwXHeight - 1)*(m_dwZHeight - 1) * 2 * sizeof(Index), (void**)&pIdx, 0)))
		return;

	m_iTrianglesNum = 0;

	for (DWORD z = 0; z < m_dwZHeight - 1; z++)
	{
		for (DWORD x = 0; x < m_dwXHeight - 1; x++)
		{
			indices[0] = (z*m_dwXHeight + x);			// 좌측 상단
			indices[1] = (z*m_dwXHeight + x + 1);			// 우측 상단
			indices[2] = ((z + 1)*m_dwXHeight + x);		// 좌측 하단
			indices[3] = ((z + 1)*m_dwXHeight + x + 1);		// 우측 하단

			bCullRet[0] = m_pFrustum->IsIn(&m_pHeigtMapVectors[indices[0]]);	// 좌측상단 정점이 Frustum안에 있는가?
			bCullRet[1] = m_pFrustum->IsIn(&m_pHeigtMapVectors[indices[1]]);	// 우측상단 정점이 Frustum안에 있는가?
			bCullRet[2] = m_pFrustum->IsIn(&m_pHeigtMapVectors[indices[2]]);	// 좌측하단 정점이 Frustum안에 있는가?

			if (bCullRet[0] | bCullRet[1] | bCullRet[2])	// 셋중에 하나라도 frustum안에 있으면 렌더링한다.
			{
				idx._0 = indices[0];
				idx._1 = indices[1];
				idx._2 = indices[2];
				*pIdx++ = idx;

				m_iTrianglesNum++;		//렌더링할 삼각형 개수 증가
			}

			bCullRet[2] = m_pFrustum->IsIn(&m_pHeigtMapVectors[indices[2]]);	// 좌측하단 정점이 Frustum안에 있는가?
			bCullRet[1] = m_pFrustum->IsIn(&m_pHeigtMapVectors[indices[1]]);	// 우측상단 정점이 Frustum안에 있는가?
			bCullRet[3] = m_pFrustum->IsIn(&m_pHeigtMapVectors[indices[3]]); // 우측하단 정점이 Frustum안에 있는가?

			if (bCullRet[2] | bCullRet[1] | bCullRet[3])	// 셋중에 하나라도 frustum안에 있으면 렌더링한다.
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
