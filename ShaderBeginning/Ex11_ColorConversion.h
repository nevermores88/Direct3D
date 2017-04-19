#pragma once
class CEx11_ColorConversion : public CBaseClass
{
	LPD3DXMESH	m_pTeapot;

	LPD3DXEFFECT	m_pEnvironmentMappingShader;
	LPD3DXEFFECT	m_pNoEffect;
	LPD3DXEFFECT	m_pGrayScale;
	LPD3DXEFFECT	m_pSepia;

	LPDIRECT3DTEXTURE9 m_pTexDM;
	LPDIRECT3DTEXTURE9 m_pTexSM;
	LPDIRECT3DTEXTURE9 m_pTexNM;
	LPDIRECT3DCUBETEXTURE9 m_pSnowENV;

	LPDIRECT3DTEXTURE9		m_pRenderTarget;

	LPDIRECT3DVERTEXDECLARATION9	m_pQuadDecl;
	LPDIRECT3DVERTEXBUFFER9	m_pQuadVB;
	LPDIRECT3DINDEXBUFFER9	m_pQuadIB;

	int m_iProcessIdx;

public:
	CEx11_ColorConversion();
	~CEx11_ColorConversion();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();

private:
	void InitFullScreenQuad();
};

