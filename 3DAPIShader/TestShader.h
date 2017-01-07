#pragma once
class CTestShader : public CBaseClass
{
	LPD3DXEFFECT m_pShader;
	LPDIRECT3DVERTEXDECLARATION9 m_pFVF;

	LPD3DXMESH	m_pMesh1;
	LPD3DXMESH	m_pMesh2;

	D3DXMATRIX m_mtWorld;

public:
	CTestShader();
	~CTestShader();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

