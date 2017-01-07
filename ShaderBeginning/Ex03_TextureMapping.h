#pragma once
class CEx03_TextureMapping : public CBaseClass
{
	LPD3DXMESH	m_pSphere;
	LPD3DXEFFECT m_pShader;
	LPDIRECT3DTEXTURE9 m_pTex;

	float m_fRotY;
	ID3DXFont*              gpFont;

public:
	CEx03_TextureMapping();
	~CEx03_TextureMapping();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

