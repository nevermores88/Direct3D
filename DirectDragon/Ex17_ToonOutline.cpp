#include "stdafx.h"
#include "Ex17_ToonOutline.h"


CEx17_ToonOutline::CEx17_ToonOutline()
{
}


CEx17_ToonOutline::~CEx17_ToonOutline()
{
}

void CEx17_ToonOutline::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;

	LPD3DXBUFFER pAdjBuffer[4] = { 0, 0, 0, 0 };

	D3DXCreateTeapot(m_pdev, &m_pMeshes[0], &pAdjBuffer[0]);
	D3DXCreateSphere(m_pdev, 1.0f, 20, 20, &m_pMeshes[1], &pAdjBuffer[1]);
	D3DXCreateTorus(m_pdev, 0.5f, 1.0f, 20, 20, &m_pMeshes[2], &pAdjBuffer[2]);
	D3DXCreateCylinder(m_pdev, 0.5f, 0.5f, 2.0f, 20, 20, &m_pMeshes[3], &pAdjBuffer[3]);

	D3DXMatrixTranslation(&m_mtWolrds[0], 0.0f, 2.0f, 0.0f);
	D3DXMatrixTranslation(&m_mtWolrds[1], 0.0f, -2.0f, 0.0f);
	D3DXMatrixTranslation(&m_mtWolrds[2], -3.0f, 0.0f, 0.0f);
	D3DXMatrixTranslation(&m_mtWolrds[3], 3.0f, 0.0f, 0.0f);

	m_MeshColors[0] = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	m_MeshColors[1] = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_MeshColors[2] = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_MeshColors[3] = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);

	for (int i = 0; i < 4; i++)
	{
		if (pAdjBuffer[i])
		{
			pAdjBuffer[i]->Release();
			pAdjBuffer[i] = NULL;
		}
	}

	//Compile Toon Shader
	LPD3DXBUFFER pToonCompiledCode = 0;
	LPD3DXBUFFER pToonErrorBuffer = 0;

	if (FAILED(D3DXCompileShaderFromFile(L"Ex17/toon.txt", 0, 0, "Main", "vs_1_1", D3DXSHADER_DEBUG, &pToonCompiledCode, &pToonErrorBuffer, &m_pToonConstTbl)))
		return;

	if (pToonErrorBuffer)
	{
		::MessageBox(0, (wchar_t*)pToonErrorBuffer->GetBufferPointer(), 0, 0);
		pToonErrorBuffer->Release();
		return;
	}

	if (FAILED(m_pdev->CreateVertexShader((DWORD*)pToonCompiledCode->GetBufferPointer(), &m_pToonShader)))
	{
		::MessageBox(0, L"CreateVertexShader - FAILED", 0, 0);
		return;
	}

	pToonCompiledCode->Release();

	//Compile Outline Shader
	LPD3DXBUFFER pOutlineCompiledCode = 0;
	LPD3DXBUFFER pOutlineErrorBuffer = 0;

	if (FAILED(D3DXCompileShaderFromFile(L"Ex17/outline.fx", 0, 0, "Main", "vs_1_1", D3DXSHADER_DEBUG, &pOutlineCompiledCode, &pOutlineErrorBuffer, &m_pOutlineConstTbl)))
	{
		wchar_t errorMsg[1024];
		MultiByteToWideChar(CP_ACP, MB_COMPOSITE, (LPCCH)pOutlineErrorBuffer->GetBufferPointer(), -1, errorMsg, 1024);
		OutputDebugString(errorMsg);
		//::MessageBox(0, (wchar_t*)pOutlineErrorBuffer->GetBufferPointer(), 0, 0);
		pToonErrorBuffer->Release();
		return;
	}

	if (FAILED(m_pdev->CreateVertexShader((DWORD*)pOutlineCompiledCode->GetBufferPointer(), &m_pOutlineShader)))
	{
		::MessageBox(0, L"CreateVertexShader - FAILED", 0, 0);
		return;
	}

	//Load Textures
	D3DXCreateTextureFromFile(m_pdev, L"Ex17/toonshade.bmp", &m_pTex);

	m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	//Get Handles
	m_hdToonWorldView = m_pToonConstTbl->GetConstantByName(0, "WorldViewMatrix");
	m_hdToonWorldViewProj = m_pToonConstTbl->GetConstantByName(0, "WorldViewProjMatrix");
	m_hdToonColor = m_pToonConstTbl->GetConstantByName(0, "Color");
	m_hdToonLightDir = m_pToonConstTbl->GetConstantByName(0, "LightDirection");

	m_hdOutlineWorldView = m_pOutlineConstTbl->GetConstantByName(0, "WorldViewMatrix");
	m_hdOutlineProj = m_pOutlineConstTbl->GetConstantByName(0, "ProjMatrix");

	//Light Direction
	D3DXVECTOR4 lightDir(-0.57f, 0.57f, -0.57f, 0.0f);
	m_pToonConstTbl->SetVector(m_pdev, m_hdToonLightDir, &lightDir);

	m_pToonConstTbl->SetDefaults(m_pdev);
	m_pOutlineConstTbl->SetDefaults(m_pdev);
}

void CEx17_ToonOutline::Release()
{
	for (int i = 0; i < 4; i++)
	{
		if (m_pMeshes[i])
		{
			m_pMeshes[i]->Release();
			m_pMeshes[i] = NULL;
		}
	}

	if (m_pTex)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}

	if (m_pToonShader)
	{
		m_pToonShader->Release();
		m_pToonShader = NULL;
	}

	if (m_pToonConstTbl)
	{
		m_pToonConstTbl->Release();
		m_pToonConstTbl = NULL;
	}

	if (m_pOutlineShader)
	{
		m_pOutlineShader->Release();
		m_pOutlineShader = NULL;
	}

	if (m_pOutlineConstTbl)
	{
		m_pOutlineConstTbl->Release();
		m_pOutlineConstTbl = NULL;
	}
}

void CEx17_ToonOutline::OnRender()
{
	if (m_pdev)
	{
		//Draw Cartoon
		m_pdev->SetVertexShader(m_pToonShader);
		m_pdev->SetTexture(0, m_pTex);

		D3DXMATRIX mtWorldView;
		D3DXMATRIX mtWorldViewProj;
		
		D3DXMATRIX mtView;
		D3DXMATRIX mtProj = g_pGame->GetProjMatrix();
		g_pCamManager->GetCamera()->GetViewMatrix(&mtView);

		for (int i = 0; i < 4; i++)
		{
			mtWorldView = m_mtWolrds[i] * m_mtView;
			mtWorldViewProj = m_mtWolrds[i] * m_mtView * mtProj;

			m_pToonConstTbl->SetMatrix(m_pdev, m_hdToonWorldView, &mtWorldView);
			m_pToonConstTbl->SetMatrix(m_pdev, m_hdToonWorldViewProj, &mtWorldViewProj);
			m_pToonConstTbl->SetVector(m_pdev, m_hdToonColor, &m_MeshColors[i]);

			m_pMeshes[i]->DrawSubset(0);
		}

		//Draw Outlines
		m_pdev->SetVertexShader(m_pOutlineShader);
		m_pdev->SetTexture(0, 0);

		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		for (int i = 0; i < 4; i++)
		{
			mtWorldView = m_mtWolrds[i] * m_mtView;

			m_pOutlineConstTbl->SetMatrix(m_pdev, m_hdOutlineWorldView, &mtWorldView);
			m_pOutlineConstTbl->SetMatrix(m_pdev, m_hdOutlineProj, &mtProj);

			//
		}

		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}

void CEx17_ToonOutline::OnUpdate()
{
	if (m_pdev)
	{
		static float angle = (3.0f * D3DX_PI) / 2.0f;
		static float height = 5.0f;

		float fTimeDelta = g_pGame->GetManager()->GetTimeManager()->GetTimeDelta();

		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
			angle -= 0.5f * fTimeDelta;

		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
			angle += 0.5f * fTimeDelta;

		if (::GetAsyncKeyState(VK_UP) & 0x8000f)
			height += 5.0f * fTimeDelta;

		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
			height -= 5.0f * fTimeDelta;

		D3DXVECTOR3 position(cosf(angle) * 7.0f, height, sinf(angle) * 7.0f);
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

		//D3DXMATRIX view = g_pGame->GetViewMatrix();

		D3DXMatrixLookAtLH(&m_mtView, &position, &target, &up);
	}
}
