#include "stdafx.h"
#include "Ex17_Diffuse.h"


CEx17_Diffuse::CEx17_Diffuse()
{
}


CEx17_Diffuse::~CEx17_Diffuse()
{
}

void CEx17_Diffuse::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;

	D3DXCreateTeapot(m_pdev, &m_pTeapot, 0);
	
	LPD3DXBUFFER	pShader = NULL;
	LPD3DXBUFFER	pError = NULL;

	HRESULT hr = 0;

	hr = D3DXCompileShaderFromFileA(
		"Ex17/diffuse.txt",
		0, 0,
		"Main",
		"vs_1_1",
		D3DXSHADER_DEBUG,
		&pShader,
		&pError,
		&m_pDiffuseConstTable);

	// output any error messages
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
		&m_pDiffuseShader);

	if (FAILED(hr))
	{
		::MessageBox(0, L"CreateVertexShader - FAILED", 0, 0);
		return;
	}

	m_hdViewMatrix = m_pDiffuseConstTable->GetConstantByName(0, "ViewMatrix");
	m_hdViewProjMatrix = m_pDiffuseConstTable->GetConstantByName(0, "ViewProjMatrix");
	m_hdAmbientMtrl = m_pDiffuseConstTable->GetConstantByName(0, "AmbientMtrl");
	m_hdDiffuseMtrl = m_pDiffuseConstTable->GetConstantByName(0, "DiffuseMtrl");
	m_hdLightDir = m_pDiffuseConstTable->GetConstantByName(0, "LightDirection");

	m_pDiffuseConstTable->SetDefaults(m_pdev);
	
}

void CEx17_Diffuse::Release()
{

}

void CEx17_Diffuse::OnRender()
{
	if (m_pdev)
	{
		D3DXVECTOR4 dirToLight(-0.57f, 0.57f, -0.57f, 0.0f);
		m_pDiffuseConstTable->SetVector(m_pdev, m_hdLightDir, &dirToLight);

		D3DXVECTOR4 ambientMtrl(0.0f, 0.0f, 1.0f, 1.0f);
		D3DXVECTOR4 diffuseMtrl(0.0f, 0.0f, 1.0f, 1.0f);

		m_pDiffuseConstTable->SetVector(m_pdev, m_hdAmbientMtrl, &ambientMtrl);
		m_pDiffuseConstTable->SetVector(m_pdev, m_hdDiffuseMtrl, &diffuseMtrl);
		

		m_pdev->SetVertexShader(m_pDiffuseShader);
		m_pTeapot->DrawSubset(0);
	}
}

void CEx17_Diffuse::OnUpdate()
{
	if (m_pdev)
	{
		static float angle = (3.0f * D3DX_PI) / 2.0f;
		static float height = 3.0f;

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

		D3DXMATRIX view = g_pGame->GetViewMatrix();

		D3DXMatrixLookAtLH(&view, &position, &target, &up);

		m_pDiffuseConstTable->SetMatrix(m_pdev, m_hdViewMatrix, &view);

		D3DXMATRIX viewProj = view * g_pGame->GetProjMatrix();

		m_pDiffuseConstTable->SetMatrix(m_pdev, m_hdViewProjMatrix, &viewProj);
	}
}
