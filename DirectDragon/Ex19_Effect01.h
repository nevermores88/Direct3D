#pragma once
class CEx19_Effect01 : public CBaseClass
{
	LPD3DXEFFECT m_pShader;

	LPD3DXMESH	m_pMesh;
	vector<D3DMATERIAL9> m_vMtrls;
	vector<LPDIRECT3DTEXTURE9> m_vTextures;

	LPDIRECT3DTEXTURE9		m_pTex;

	D3DXHANDLE	m_hdWorldMatrix;
	D3DXHANDLE	m_hdViewMatrix;
	D3DXHANDLE	m_hdProjMatrix;
	D3DXHANDLE	m_hdTex;

	D3DXHANDLE	m_hdTech;

public:
	CEx19_Effect01();
	~CEx19_Effect01();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

