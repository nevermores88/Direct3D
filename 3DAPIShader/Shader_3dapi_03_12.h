//---------------------------------------------------------------------------------------------------
//
//Description : 2D Sprite 01
//
//---------------------------------------------------------------------------------------------------
#pragma once

#include "Sprite.h"

class CShader_3dapi_03_12 : public CBaseClass
{	
	LPD3DXFONT	m_pFont;
	TCHAR	m_szMessage[512];

	StTexture	m_stTexture;
	CSprite*		m_pSprite;

public:
	CShader_3dapi_03_12();
	~CShader_3dapi_03_12();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

