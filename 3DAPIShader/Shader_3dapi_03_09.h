#pragma once
class CShader_3dapi_03_09 : public CBaseClass
{
	LPD3DXEFFECT m_pShader;
	LPDIRECT3DVERTEXDECLARATION9 m_pFVF;

	LPD3DXMESH	m_pMesh;
	LPDIRECT3DTEXTURE9 m_pTex;

	D3DXMATRIX m_mtWorld0;
	D3DXMATRIX m_mtWorld1;
	D3DXMATRIX m_mtRot;

public:
	CShader_3dapi_03_09();
	~CShader_3dapi_03_09();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

