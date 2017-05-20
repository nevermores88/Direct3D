#include "stdafx.h"
#include "Sprite.h"


CSprite::CSprite()
{
}


CSprite::~CSprite()
{
}

int CSprite::DrawEx(void* _pTex, RECT* _pRect, D3DXVECTOR2* _pScale, D3DXVECTOR2* _pRot, float _fRot, D3DXVECTOR2* _pTrn, DWORD _dwColor, int _iMono)
{
	HRESULT hr = 0;

	int iMono = 0;
	D3DXCOLOR dwColor = 0xFFFFFFFF;
	D3DXVECTOR2 vScale(1, 1);
	D3DXVECTOR2 vRot(0, 0);
	D3DXVECTOR2 vTrn(0, 0);

	RECT rect = { 0, 0, 2048, 2048 };

	D3DXVECTOR2 uv0(0, 0);
	D3DXVECTOR2 uv1(1, 1);
	float fRectWidth = 0.0f;
	float fRectHeight = 0.0f;

	float fPosL = 0;
	float fPosT = 0;
	float fPosR = 0;
	float fPosB = 0;

	StTexture* pTex = (StTexture*)_pTex;

	if (!pTex)
		return 0;

	//1. 입력값 복사
	dwColor = _dwColor;
	iMono = _iMono;

	//1.1 이미지 소스 1 영역 복사
	if (_pRect)
	{
		rect.left = _pRect->left;
		rect.right = _pRect->right;
		rect.top = _pRect->top;
		rect.bottom = _pRect->bottom;
	}
	else
	{
		rect.right = pTex->m_iImgWidth;
		rect.bottom = pTex->m_iImgHeight;
	}

	//영역 재 지정
	if (rect.left < 0)	rect.left = 0;
	if (rect.right > pTex->m_iImgWidth) rect.right = pTex->m_iImgWidth;
	if (rect.top < 0)	rect.top = 0;
	if (rect.bottom > pTex->m_iImgHeight) rect.bottom = pTex->m_iImgHeight;

	//영역을 잘못 설정한 경우
	if (rect.top >= rect.bottom || rect.left >= rect.right)
		return 0;

	//uv 설정
	uv0.x = float(rect.left /*- 0.5f*/) / float(pTex->m_iTexWidth);
	uv0.y = float(rect.top /*- 0.5f*/) / float(pTex->m_iTexHeight);
	uv1.x = float(rect.right) / float(pTex->m_iTexWidth);
	uv1.y = float(rect.bottom) / float(pTex->m_iTexHeight);

	//2. Scaling, Rot, Position Setup
	if (_pScale) vScale = *_pScale;
	if (_pRot) vRot = *_pRot;
	if (_pTrn) vTrn = *_pTrn;

	fRectWidth = float(rect.right - rect.left);
	fRectHeight = float(rect.bottom - rect.top);

	//Scaling 적용
	if (vScale.x >= 0.0f)
	{
		fPosL = vTrn.x;
		fPosR = fPosL + fRectWidth * vScale.x;
	}
	else
	{
		fPosR = vTrn.x;
		fPosL = fPosR - fRectWidth*vScale.x;
	}

	if (vScale.y >= 0.0f)
	{
		fPosT = vTrn.y;
		fPosB = fPosT + fRectHeight*vScale.y;
	}
	else
	{
		fPosB = vTrn.y;
		fPosT = fPosB - fRectHeight*vScale.y;
	}

	//정점 설정
	m_pVertices[0].p = D3DXVECTOR2(fPosL, fPosT);
	m_pVertices[1].p = D3DXVECTOR2(fPosR, fPosT);
	m_pVertices[2].p = D3DXVECTOR2(fPosR, fPosB);
	m_pVertices[3].p = D3DXVECTOR2(fPosL, fPosB);

	m_pVertices[0].w = 1;
	m_pVertices[1].w = 1;
	m_pVertices[2].w = 1;
	m_pVertices[3].w = 1;

	m_pVertices[0].u = uv0.x; m_pVertices[0].v = uv0.y;
	m_pVertices[1].u = uv1.x; m_pVertices[1].v = uv0.y;
	m_pVertices[2].u = uv1.x; m_pVertices[2].v = uv1.y;
	m_pVertices[3].u = uv0.x; m_pVertices[3].v = uv1.y;

	//Test
	/*RECT rc;
	GetClientRect(g_pGame->GetHWND(), &rc);

	float fX = float(rc.right - rc.left);
	float fY = float(rc.bottom - rc.top);

	m_pVertices[0].p = D3DXVECTOR2(0, fY);
	m_pVertices[1].p = D3DXVECTOR2(0, 0);
	m_pVertices[2].p = D3DXVECTOR2(fX, fY);
	m_pVertices[3].p = D3DXVECTOR2(fX, 0);

	m_pVertices[0].u = 0; m_pVertices[0].v = 1.0f;
	m_pVertices[1].u = 0; m_pVertices[1].v = 0.0f;
	m_pVertices[2].u = 1.0f; m_pVertices[2].v = 1.0f;
	m_pVertices[3].u = 1.0f; m_pVertices[3].v = 0.0f;*/

	//회전 설정
	if (_pRot)
	{
		float fCos = cosf(-_fRot);
		float fSin = sinf(-_fRot);

		D3DXVECTOR2 t;

		for (int i = 0; i < 4; ++i)
		{
			t = m_pVertices[i].p - vRot;

			m_pVertices[i].p.x = t.x * fCos - t.y * fSin + vRot.x;
			m_pVertices[i].p.y = t.x * fSin + t.y * fCos + vRot.y;
		}
	}

	m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pdev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pdev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	// 단색일 경우 부드럽게 처리함
	if (iMono)
	{
		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	}
	else
	{
		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	}

	m_pdev->SetTexture(0, pTex->m_pTex);

	m_pdev->SetVertexDeclaration(m_pFVF);

	m_pShader->SetTechnique("Tech");
	m_pShader->SetInt("m_iMono", iMono);
	m_pShader->SetVector("m_Color", (D3DXVECTOR4*)&dwColor);

	m_pShader->Begin(NULL, 0);
	m_pShader->BeginPass(0);
	
	m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_pVertices, sizeof(Vertex));

	m_pShader->EndPass();
	m_pShader->End();


	m_pdev->SetTexture(0, NULL);
	m_pdev->SetTexture(1, NULL);

	m_pdev->SetVertexDeclaration(NULL);
	m_pdev->SetVertexShader(NULL);
	m_pdev->SetPixelShader(NULL);
	m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return 1;
}

int CSprite::OnResetDevice()
{
	return 1;
}

void CSprite::OnLostDevice()
{

}

HRESULT CSprite::Create(LPDIRECT3DDEVICE9 pdev, char* szFileName)
{
	m_pdev = pdev;

	D3DDEVICE_CREATION_PARAMETERS		parameters;
	m_pdev->GetCreationParameters(&parameters);
	m_hWnd = parameters.hFocusWindow;

	LPD3DXBUFFER	pError = NULL;
	DWORD dwFlag = 0;

	HRESULT hr = 0;

#if defined(_DEBUG) || defined(DEBUG)
	dwFlag |= D3DXSHADER_DEBUG;
#endif

	hr = D3DXCreateEffectFromFile(m_pdev, szFileName, NULL, NULL, dwFlag, NULL, &m_pShader, &pError);
	if (FAILED(hr))
	{
		MessageBox(GetActiveWindow(), (char*)pError->GetBufferPointer(), "Error", 0);

		return E_FAIL;
	}

	DWORD dwFVF = Vertex::FVF;
	D3DVERTEXELEMENT9	pVertexElement[MAX_FVF_DECL_SIZE];
	D3DXDeclaratorFromFVF(dwFVF, pVertexElement);
	m_pdev->CreateVertexDeclaration(pVertexElement, &m_pFVF);

	return S_OK;
}

void CSprite::Release()
{
	if (m_pShader)
	{
		m_pShader->Release();
		m_pShader = NULL;
	}

	if (m_pFVF)
	{
		m_pFVF->Release();
		m_pFVF = NULL;
	}
}
