#include "stdafx.h"
#include "Ex18_MultiTexture.h"


CEx18_MultiTexture::CEx18_MultiTexture()
{
}


CEx18_MultiTexture::~CEx18_MultiTexture()
{
}

void CEx18_MultiTexture::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;

	m_pdev->CreateVertexBuffer(
		6 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&m_pQuadVB,
		0);

	Vertex* v = 0;

	m_pQuadVB->Lock(0, 0, (void**)&v, 0);

	v[0] = Vertex(-10.0f, -10.0f, 5.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[1] = Vertex(-10.0f, 10.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[2] = Vertex(10.0f, 10.0f, 5.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

	v[3] = Vertex(-10.0f, -10.0f, 5.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[4] = Vertex(10.0f, 10.0f, 5.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[5] = Vertex(10.0f, -10.0f, 5.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

	m_pQuadVB->Unlock();

	LPD3DXBUFFER	m_pShader = NULL;
	LPD3DXBUFFER	m_pErrBuffer = NULL;

	HRESULT hr = 0;

	hr = D3DXCompileShaderFromFile(
		L"Ex18/ps_multitex.txt",
		0, 0,
		"Main",
		"ps_2_0",
		D3DXSHADER_DEBUG,
		&m_pShader,
		&m_pErrBuffer,
		&m_pMultiTexCT);

	if (m_pErrBuffer)
	{
		wchar_t errorMsg[1024];
		MultiByteToWideChar(CP_ACP, MB_COMPOSITE, (LPCCH)m_pErrBuffer->GetBufferPointer(), -1, errorMsg, 1024);
		::MessageBox(0, errorMsg, 0, 0);
	}

	if (FAILED(hr))
	{
		::MessageBox(0, L"D3DXCompileShaderFromFile() - FAILED", 0, 0);
		return;
	}

	hr = m_pdev->CreatePixelShader(
		(DWORD*)m_pShader->GetBufferPointer(),
		&m_pMultiTexPS);

	if (FAILED(hr))
	{
		::MessageBox(0, L"CreateVertexShader - FAILED", 0, 0);
		return;
	}

	D3DXCreateTextureFromFile(m_pdev, L"Ex18/crate.bmp", &m_pBaseTex);
	D3DXCreateTextureFromFile(m_pdev, L"Ex18/spotlight.bmp", &m_pSpotLightTex);
	D3DXCreateTextureFromFile(m_pdev, L"Ex18/text.bmp", &m_pStringTex);

	m_hdBaseTex = m_pMultiTexCT->GetConstantByName(0, "BaseTex");
	m_hdSpotLight = m_pMultiTexCT->GetConstantByName(0, "SpotLightTex");
	m_hdStringTex = m_pMultiTexCT->GetConstantByName(0, "StringTex");

	UINT count;

	m_pMultiTexCT->GetConstantDesc(m_hdBaseTex, &m_BaseTexDesc, &count);
	m_pMultiTexCT->GetConstantDesc(m_hdSpotLight, &m_SpotLightTextDesc, &count);
	m_pMultiTexCT->GetConstantDesc(m_hdStringTex, &m_StringTexDesc, &count);
}

void CEx18_MultiTexture::Release()
{

}

void CEx18_MultiTexture::OnRender()
{
	if (m_pdev)
	{
		m_pdev->SetRenderState(D3DRS_LIGHTING, true);

		m_pdev->SetPixelShader(m_pMultiTexPS);
		m_pdev->SetFVF(Vertex::FVF);
		m_pdev->SetStreamSource(0, m_pQuadVB, 0, sizeof(Vertex));

		m_pdev->SetTexture(m_BaseTexDesc.RegisterIndex, m_pBaseTex);
		m_pdev->SetSamplerState(m_BaseTexDesc.RegisterIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(m_BaseTexDesc.RegisterIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(m_BaseTexDesc.RegisterIndex, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		m_pdev->SetTexture(m_SpotLightTextDesc.RegisterIndex, m_pSpotLightTex);
		m_pdev->SetSamplerState(m_SpotLightTextDesc.RegisterIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(m_SpotLightTextDesc.RegisterIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(m_SpotLightTextDesc.RegisterIndex, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		m_pdev->SetTexture(m_StringTexDesc.RegisterIndex, m_pStringTex);
		m_pdev->SetSamplerState(m_StringTexDesc.RegisterIndex, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(m_StringTexDesc.RegisterIndex, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(m_StringTexDesc.RegisterIndex, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	
		m_pdev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
	}
}

void CEx18_MultiTexture::OnUpdate()
{
	if (m_pdev)
	{
		static float angle = (3.0f * D3DX_PI) / 2.0f;
		static float radius = 20.0f;

		float fTimeDelta = g_pGame->GetManager()->GetTimeManager()->GetTimeDelta();

		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
			angle -= 0.5f * fTimeDelta;

		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
			angle += 0.5f * fTimeDelta;

		if (::GetAsyncKeyState(VK_UP) & 0x8000f)
			radius -= 2.0f * fTimeDelta;

		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
			radius += 2.0f * fTimeDelta;

		D3DXVECTOR3 position(cosf(angle) * radius, 0.0f, sinf(angle) * radius);
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		D3DXMATRIX V;
		D3DXMatrixLookAtLH(&V, &position, &target, &up);

		m_pdev->SetTransform(D3DTS_VIEW, &V);
	}
}
