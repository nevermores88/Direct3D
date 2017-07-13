#pragma once
#include "IRenderTarget.h"

class CRenderTarget : public IRenderTarget
{
protected:
	LPDIRECT3DDEVICE9		m_pDev;		// Device

	INT						m_nType;		// 0: ID3DXRenderToSurface, 1: ID3DXRenderToEnvMap(Sphere) 2: CubeMap
	INT						m_iW;		// Width
	INT						m_iH;		// Height
	DWORD					m_dD;		// Depth Format

	LPD3DXRENDERTOSURFACE	m_pRts;		// Direct3D Render To Target

	LPDIRECT3DTEXTURE9		m_pTxP;		// Plan or Sphere Map Texture
	LPDIRECT3DSURFACE9		m_pSfc;		// Rendering용 텍스처 surface
	IDirect3DCubeTexture9*	m_pTxC;		// Cube Map Texture

public:
	CRenderTarget();
	virtual ~CRenderTarget();

	virtual	INT		Create(void* p1 = NULL, void* p2 = NULL, void* p3 = NULL, void* p4 = NULL);
	virtual	void	Destroy();

	virtual	INT		OnResetDevice();
	virtual	INT		OnLostDevice();

	virtual	INT		BeginScene(DWORD dClearMode = (0x1L | 0x2L | 0x4L), DWORD dClearColor = 0xFF006699);
	virtual	INT		EndScene();

	virtual	INT		GetWidth();
	virtual	INT		GetHeight();
	virtual	DWORD	GetDepth();

	virtual	void*	GetTexture() const;
	virtual	void*	GetSurface() const;

protected:
	INT		CreateRenderSurface();
};

