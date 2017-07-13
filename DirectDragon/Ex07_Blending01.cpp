#include "stdafx.h"
#include "Ex07_Blending01.h"


CEx07_Blending01::CEx07_Blending01()
{
}


CEx07_Blending01::~CEx07_Blending01()
{
}

void CEx07_Blending01::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;
	m_TeapotMtrl.Ambient = D3DXCOLOR(1.f, 0.0f, 0.0f, 1.0f);
	m_TeapotMtrl.Diffuse = D3DXCOLOR(1.f, 0.0f, 0.0f, 0.5f);
	m_TeapotMtrl.Specular = D3DXCOLOR(1.f, 0.0f, 0.0f, 1.0f);
	m_TeapotMtrl.Emissive = D3DXCOLOR(0.f, 0.0f, 0.0f, 1.0f);

	m_BkGndMtrl.Ambient = D3DXCOLOR(1.f, 1.0f, 1.0f, 1.0f);
	m_BkGndMtrl.Diffuse = D3DXCOLOR(1.f, 1.0f, 1.0f, 1.0f);
	m_BkGndMtrl.Specular = D3DXCOLOR(1.f, 1.0f, 1.0f, 1.0f);
	m_BkGndMtrl.Emissive = D3DXCOLOR(1.f, 1.0f, 1.0f, 1.0f);

	D3DXCreateTeapot(m_pdev, &m_pTeapot, 0);

	m_pdev->CreateVertexBuffer(6 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&m_pBkGndQuad,
		0);

	Vertex* v;
	m_pBkGndQuad->Lock(0, 0, (void**)&v, 0);

	v[0] = Vertex(-5.0f, -5.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[1] = Vertex(-5.0f, 5.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = Vertex(5.0f, 5.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[3] = Vertex(-5.0f, -5.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[4] = Vertex(5.0f, 5.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[5] = Vertex(5.0f, -5.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	m_pBkGndQuad->Unlock();

	D3DXCreateTextureFromFile(
		m_pdev,
		L"Ex07/crate.jpg",
		&m_pBkGndTex);
}

void CEx07_Blending01::Release()
{

}

void CEx07_Blending01::OnRender()
{
	if (m_pdev)
	{
		m_pdev->SetLight(0, &m_Light);
		m_pdev->LightEnable(0, true);

		m_pdev->SetRenderState(D3DRS_NORMALIZENORMALS, true);
		m_pdev->SetRenderState(D3DRS_SPECULARENABLE, true);

		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

		//Material Alpha 에서 Alpha 값을 가져옴 (D3DTA_DIFFUSE 일때)
		//Texture Alpha 채널에서 Alpha 값을 가져옴 (D3DTA_TEXTURE 일때)
		//디폴트는 D3DTA_DIFFUSE -> D3DTA_TEXTURE
		m_pdev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		m_pdev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);

		m_pdev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pdev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		//m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		// Draw the background
		D3DXMATRIX W;
		D3DXMatrixIdentity(&W);
		m_pdev->SetFVF(Vertex::FVF);
		m_pdev->SetStreamSource(0, m_pBkGndQuad, 0, sizeof(Vertex));
		m_pdev->SetMaterial(&m_BkGndMtrl);
		m_pdev->SetTexture(0, m_pBkGndTex);
		m_pdev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

		// Draw the teapot
		m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		//D3DXMatrixScaling(&W, 1.5f, 1.5f, 1.5f);
		//m_pdev->SetTransform(D3DTS_WORLD, &W);
		m_pdev->SetMaterial(&m_TeapotMtrl);
		m_pdev->SetTexture(0, 0);
		m_pTeapot->DrawSubset(0);

		m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	}
}

void CEx07_Blending01::OnUpdate()
{
	if (m_pdev)
	{
		::ZeroMemory(&m_Light, sizeof(m_Light));
		m_Light.Type = D3DLIGHT_DIRECTIONAL;
		m_Light.Diffuse = D3DXCOLOR(1.f, 1.0f, 1.0f, 1.0f);
		m_Light.Specular = D3DXCOLOR(1.f, 1.0f, 1.0f, 1.0f) *0.2f;
		m_Light.Ambient = D3DXCOLOR(1.f, 1.0f, 1.0f, 1.0f) *0.6f;
		m_Light.Direction = D3DXVECTOR3(0.707f, 0.0f, 0.707f);

		if (::GetAsyncKeyState('1') & 0x8000f)
			m_TeapotMtrl.Diffuse.a += 0.01f;
		if (::GetAsyncKeyState('2') & 0x8000f)
			m_TeapotMtrl.Diffuse.a -= 0.01f;

		if (m_TeapotMtrl.Diffuse.a > 1.0f)
			m_TeapotMtrl.Diffuse.a = 1.0f;
		if (m_TeapotMtrl.Diffuse.a < 0.0f)
			m_TeapotMtrl.Diffuse.a = 0.0f;
	}
}
