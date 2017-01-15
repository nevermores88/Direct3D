#pragma once
#include "IRenderTarget.h"

class CRenderTarget : public IRenderTarget
{
	LPDIRECT3DDEVICE9 m_pdev;

	int m_iType;		// 0: ID3DXRenderToSurface, 1: ID3DXRenderToEnvMap(Sphere) 2: CubeMap
	int m_iWidth;
	int m_iHeight;
	DWORD m_dwDepthFormat;		//Depth Format

	LPD3DXRENDERTOSURFACE m_pRts;

	LPDIRECT3DTEXTURE9 m_pTex;					// Plan or Sphere Map Texture
	LPDIRECT3DSURFACE9 m_pSurface;
	LPDIRECT3DCUBETEXTURE9* m_pCubeTex;		// Cube Map Texture

public:
	CRenderTarget();
	~CRenderTarget();

	virtual HRESULT Create(void* p1 = NULL, void* p2 = NULL, void* p3 = NULL, void* p4 = NULL);
	virtual void Release();

	virtual int BeginScene(DWORD dwClearMode = (0x1L | 0x2L | 0x4L), DWORD dwClearColor = 0xFF006699);
	virtual int EndScene();

	virtual int GetWidth();
	virtual int GetHeight();
	virtual DWORD GetDepth();

	virtual void* GetTexture() const;
	virtual void* GetSurface() const;

protected:
	int CreateRenderSurface();
};

int LcD3D_CreateRenderTarget(char* szCmd, IRenderTarget** pData, void* pdev, int iWidth = -1, int iHeight = -1);
