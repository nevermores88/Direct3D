#include "stdafx.h"
#include "Shader_3dapi_03_12.h"


CShader_3dapi_03_12::CShader_3dapi_03_12()
{
}


CShader_3dapi_03_12::~CShader_3dapi_03_12()
{
}

HRESULT CShader_3dapi_03_12::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	HRESULT hr = 0;

	D3DXFONT_DESC hFont =
	{
		16, 0
		, FW_NORMAL
		, 1
		, FALSE
		, HANGUL_CHARSET
		, OUT_DEFAULT_PRECIS
		, NONANTIALIASED_QUALITY
		, FF_DONTCARE
		, "Arial"
	};

	if (FAILED(D3DXCreateFontIndirect(m_pdev, &hFont, &m_pFont)))
		return E_FAIL;

	m_pSprite = new CSprite();
	m_pSprite->Create(m_pdev, "Sprite/Shader.fx");

	D3DXIMAGE_INFO imgInfo;
	D3DSURFACE_DESC	surfDesc;
	LPDIRECT3DTEXTURE9	 pTex = NULL;

	hr = D3DXCreateTextureFromFileEx(m_pdev,
		"Ex03_12/mario.png",
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		1, 0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,
		D3DX_FILTER_NONE,
		0x00FFFFFF,
		&imgInfo,
		NULL,
		&pTex);

	pTex->GetLevelDesc(0, &surfDesc);

	m_stTexture.m_iImgWidth = imgInfo.Width;
	m_stTexture.m_iImgHeight = imgInfo.Height;
	m_stTexture.m_iImgDepth = imgInfo.Depth;
	m_stTexture.m_iTexWidth = surfDesc.Width;
	m_stTexture.m_iTexHeight = surfDesc.Height;
	m_stTexture.m_pTex = pTex;

	return S_OK;
}

void CShader_3dapi_03_12::Release()
{
	if (m_pFont)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}

	if (m_stTexture.m_pTex)
	{
		m_stTexture.m_pTex->Release();
		m_stTexture.m_pTex = NULL;
	}

	if (m_pSprite)
	{
		m_pSprite->Release();
		m_pSprite = NULL;
	}
}

void CShader_3dapi_03_12::Render()
{
	if (m_pdev)
	{
		{
			RECT rect1 = { 60, 0, 80, 32 };
			D3DXVECTOR2 vTrn1(160.0f, 120.0f);
			D3DXVECTOR2 vTrn2(160.0f, 180.0f + 8.0f*32.0f);
			D3DXVECTOR2 vScale1(10.0f, 10.0f);
			D3DXVECTOR2 vScale2(9.0f, -3.0f);
			/*D3DXVECTOR2 vRot(200.0f, 200.0f + 8.0f*32.0f);*/
			D3DXVECTOR2 vRot(160.0f + 100.0f, 120 + 160.0f);
			static float t = 0;
			t += 0.01f;
			//그림자
			m_pSprite->DrawEx(&m_stTexture, &rect1, &vScale2, NULL, 0, &vTrn2, 0x88333333, TRUE);

			//캐릭터
			m_pSprite->DrawEx(&m_stTexture, &rect1, &vScale1, &vRot, t, &vTrn1, 0xFFFFFFFF, FALSE);
		}

		{
			RECT rect1 = { 80, 0, 100, 32 };
			D3DXVECTOR2 vTrn1(420.0f, 120.0f);
			D3DXVECTOR2 vTrn2(420.0f, 180.0f + 8.0f*32.0f);
			D3DXVECTOR2 vScale1(-10.0f, 10.0f);
			D3DXVECTOR2 vScale2(-9.0f, -3.0f);
			D3DXVECTOR2 vRot(200.0f, 200.0f + 8.0f*32.0f);

			//그림자
			m_pSprite->DrawEx(&m_stTexture, &rect1, &vScale2, NULL, 0, &vTrn2, 0x88333333, TRUE);

			//캐릭터
			m_pSprite->DrawEx(&m_stTexture, &rect1, &vScale1, &vRot, 0, &vTrn1, 0xFFFFFFFF, FALSE);
		}

		/*m_pdev->SetFVF(VertexRHW::FVF);
		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_pRHW, sizeof(VertexRHW));*/
	}
}

void CShader_3dapi_03_12::Update()
{
	if (m_pdev)
	{

	}
}
