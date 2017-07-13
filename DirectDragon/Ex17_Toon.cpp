#include "stdafx.h"
#include "Ex17_Toon.h"


CEx17_Toon::CEx17_Toon()
{
}


CEx17_Toon::~CEx17_Toon()
{
}

void CEx17_Toon::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;

	D3DXCreateTeapot(m_pdev, &m_pMeshes[0], 0);
	D3DXCreateSphere(m_pdev, 1.0f, 20, 20, &m_pMeshes[1], 0);
	D3DXCreateTorus(m_pdev, 0.5f, 1.0f, 20, 20, &m_pMeshes[2], 0);
	D3DXCreateCylinder(m_pdev, 0.5f, 0.5f, 2.0f, 20, 20, &m_pMeshes[3], 0);

	D3DXMatrixTranslation(&m_mtWorldMatrices[0], 0.0f, 2.0f, 0.0f);
	D3DXMatrixTranslation(&m_mtWorldMatrices[1], 0.0f, -2.0f, 0.0f);
	D3DXMatrixTranslation(&m_mtWorldMatrices[2], -3.0f, 0.0f, 0.0f);
	D3DXMatrixTranslation(&m_mtWorldMatrices[3], 3.0f, 0.0f, 0.0f);

	m_MeshColors[0] = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	m_MeshColors[1] = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_MeshColors[2] = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_MeshColors[3] = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);

	HRESULT hr = 0;

	LPD3DXBUFFER	pShader = NULL;
	LPD3DXBUFFER	pError = NULL;

	hr = D3DXCompileShaderFromFile(
		L"Ex17/toon.txt",
		0, 0,
		"Main",
		"vs_1_1",
		D3DXSHADER_DEBUG,
		&pShader,
		&pError,
		&m_pToonConstTable);

	if (pError)
	{
		wchar_t errorMsg[1024];
		MultiByteToWideChar(CP_ACP, MB_COMPOSITE, (LPCCH)pError->GetBufferPointer(), -1, errorMsg, 1024);
		::MessageBox(0, errorMsg, 0, 0);
	}

	if (FAILED(hr))
	{
		::MessageBox(0, L"D3DXCompileShaderFromFile() - FAILED", 0, 0);
		return;
	}

	hr = m_pdev->CreateVertexShader(
		(DWORD*)pShader->GetBufferPointer(),
		&m_pToonShader);

	if (FAILED(hr))
	{
		::MessageBox(0, L"CreateVertexShader - FAILED", 0, 0);
		return;
	}

	m_hdWorldView = m_pToonConstTable->GetConstantByName(0, "WorldViewMatrix");
	m_hdWorldViewProj = m_pToonConstTable->GetConstantByName(0, "WorldViewProjMatrix");
	m_hdColor = m_pToonConstTable->GetConstantByName(0, "Color");
	m_hdLightDir = m_pToonConstTable->GetConstantByName(0, "LightDirection");

	D3DXCreateTextureFromFile(m_pdev, L"Ex17/toonshade.bmp", &m_pTex);

	

	D3DXVECTOR4 dirToLight(-0.57f, 0.57f, -0.57f, 0.0f);

	m_pToonConstTable->SetVector(
		m_pdev,
		m_hdLightDir,
		&dirToLight);

	//m_pToonConstTable->SetDefaults(m_pdev);
}

void CEx17_Toon::Release()
{

}

void CEx17_Toon::OnRender()
{
	if (m_pdev)
	{
		m_pdev->SetVertexShader(m_pToonShader);
		m_pdev->SetTexture(0, m_pTex);

		D3DXMATRIX mtWorldView;
		D3DXMATRIX mtWorldViewProj;

		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

		for (int i = 0; i < 4; i++)
		{
			mtWorldView = m_mtWorldMatrices[i] * m_mtView;
			mtWorldViewProj = m_mtWorldMatrices[i] * m_mtView*g_pGame->GetProjMatrix();

			m_pToonConstTable->SetMatrix(
				m_pdev,
				m_hdWorldView,
				&mtWorldView);

			m_pToonConstTable->SetMatrix(
				m_pdev,
				m_hdWorldViewProj,
				&mtWorldViewProj);

			m_pToonConstTable->SetVector(
				m_pdev,
				m_hdColor,
				&m_MeshColors[i]);

			m_pMeshes[i]->DrawSubset(0);
		}
	}
}

void CEx17_Toon::OnUpdate()
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

		/*m_pToonConstTable->SetMatrix(m_pdev, m_hdWorldView, &m_mtView);

		D3DXMATRIX viewProj = view*g_pGame->GetProjMatrix();

		m_pToonConstTable->SetMatrix(m_pdev, m_hdWorldViewProj, &viewProj);*/

		
	}
}
