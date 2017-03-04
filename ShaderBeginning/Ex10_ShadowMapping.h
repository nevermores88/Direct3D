#pragma once
class CEx10_ShadowMapping : public CBaseClass
{
	LPD3DXEFFECT m_pCreateShadowShader;
	LPD3DXEFFECT m_pApplyShadowShader;
	
	LPD3DXMESH m_pTorusMesh;
	LPD3DXMESH m_pDiscMesh;

	LPDIRECT3DTEXTURE9 m_pShadowRenderTarget;
	LPDIRECT3DSURFACE9 m_pShadowDepthStencil;

	float m_fRotY;

public:
	CEx10_ShadowMapping();
	~CEx10_ShadowMapping();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

