#include "stdafx.h"
#include "Skel_07_01_Terrain.h"
#include "Skel_Camera.h"

CSkel_07_01_Terrain::CSkel_07_01_Terrain()
{
}


CSkel_07_01_Terrain::~CSkel_07_01_Terrain()
{
}

HRESULT CSkel_07_01_Terrain::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	InitTexture();
	InitVertexBuffer();
	InitIndexBuffer();

	m_pCamera = new CSkel_Camera();

	POINT pt;
	GetCursorPos(&pt);
	m_dwMouseX = pt.x;
	m_dwMouseY = pt.y;

	return S_OK;
}

void CSkel_07_01_Terrain::Release()
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
}

void CSkel_07_01_Terrain::Render()
{
	if (m_pdev)
	{
		m_pdev->SetTexture(0, m_pDiffuseTex);
		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

		m_pdev->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);

		m_pdev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

		DrawMesh();
	}
}

void CSkel_07_01_Terrain::Update()
{
	if (m_pdev)
	{
		SetupLights();

		ProcessKey();
		ProcessMouse();

		D3DXMATRIX mtView;
		D3DXMATRIX mtProj;
		D3DXMATRIX mtViewProj;
		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);

		mtViewProj = mtView * mtProj;
	}
}

void CSkel_07_01_Terrain::InitTexture()
{
	HRESULT hr = 0;

	hr = D3DXCreateTextureFromFileEx(
		m_pdev,
		"Chapter07/Ex01/map128.bmp",
		D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0,
		D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0,
		NULL, NULL, &m_pHeightTex);

	if (FAILED(hr))
		return;

	hr = D3DXCreateTextureFromFile(m_pdev, "Chapter07/Ex01/tile2.tga", &m_pDiffuseTex);

	if (FAILED(hr))
		return;
}

void CSkel_07_01_Terrain::InitVertexBuffer()
{
	HRESULT hr = 0;

	D3DSURFACE_DESC	desc;
	D3DLOCKED_RECT	d3drc;

	m_pHeightTex->GetLevelDesc(0, &desc);
	m_dwXHeight = desc.Width;
	m_dwZHeight = desc.Height;

	hr = m_pdev->CreateVertexBuffer(m_dwXHeight*m_dwZHeight*sizeof(Vertex),
		0, Vertex::FVF, D3DPOOL_DEFAULT, &m_pVB, NULL);

	if (FAILED(hr))
		return;

	m_pHeightTex->LockRect(0, &d3drc, NULL, D3DLOCK_READONLY);

	VOID* pVertices;

	m_pVB->Lock(0, m_dwXHeight*m_dwZHeight*sizeof(Vertex), (void**)&pVertices, 0);

	Vertex v;
	Vertex* pV = (Vertex*)pVertices;

	for (DWORD z = 0; z < m_dwZHeight; z++)
	{
		for (DWORD x = 0; x < m_dwXHeight; x++)
		{
			v.p.x = (float)x - m_dwXHeight / 2.0f;
			v.p.z = -((float)z - m_dwZHeight / 2.0f);

			v.p.y = (float)((float)(*((LPDWORD)d3drc.pBits + x + z*(d3drc.Pitch / 4)) & 0x000000ff)) / 10.0f;
			v.n = v.p;
			D3DXVec3Normalize(&v.n, &v.n);

			v.t.x = (float)x / (m_dwXHeight - 1);
			v.t.y = (float)z / (m_dwZHeight - 1);
			*pV++ = v;
		}
	}

	m_pVB->Unlock();

	m_pHeightTex->UnlockRect(0);
}

void CSkel_07_01_Terrain::InitIndexBuffer()
{
	m_pdev->CreateIndexBuffer((m_dwXHeight - 1) * (m_dwZHeight - 1) * 2 * sizeof(Index),
		0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIB, NULL);

	Index i;
	Index* pI;

	m_pIB->Lock(0, (m_dwXHeight - 1) * (m_dwZHeight - 1) * 2 * sizeof(Index), (void**)&pI, 0);

	for (DWORD z = 0; z < m_dwZHeight - 1; z++)
	{
		for (DWORD x = 0; x < m_dwXHeight - 1; x++)
		{
			i._0 = (z*m_dwXHeight + x);
			i._1 = (z*m_dwXHeight + x + 1);
			i._2 = ((z + 1)*m_dwXHeight + x);

			*pI++ = i;

			i._0 = ((z + 1)*m_dwXHeight + x);
			i._1 = (z*m_dwXHeight + x + 1);
			i._2 = ((z + 1)*m_dwXHeight + x + 1);

			*pI++ = i;
		}
	}

	m_pIB->Unlock();
}

void CSkel_07_01_Terrain::SetupLights()
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

void CSkel_07_01_Terrain::DrawMesh()
{
	if (m_pdev)
	{
		//m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		D3DXMatrixIdentity(&m_mtAni);
		m_pdev->SetTransform(D3DTS_WORLD, &m_mtAni);
		m_pdev->SetStreamSource(0, m_pVB, 0, sizeof(Vertex));
		m_pdev->SetFVF(Vertex::FVF);
		m_pdev->SetIndices(m_pIB);
		m_pdev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwXHeight*m_dwZHeight,
			0, (m_dwXHeight - 1)*(m_dwZHeight - 1) * 2);
	}
}

void CSkel_07_01_Terrain::ProcessMouse()
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

void CSkel_07_01_Terrain::ProcessKey()
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
		m_pdev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	if (GetAsyncKeyState(VK_RBUTTON))
		m_pdev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}
