#pragma once
class CEx19_Effect03 : public CBaseClass
{
	LPD3DXEFFECT m_pShader;

	LPD3DXMESH	m_pMeshes[4];
	D3DXMATRIX m_mtWorlds[4];
	D3DXVECTOR4 m_vMeshColors[4];

	LPDIRECT3DTEXTURE9		m_pTex;

	D3DXHANDLE	m_hdWorldViewMatrix;
	D3DXHANDLE	m_hdWorldViewProjMatrix;
	D3DXHANDLE	m_hdColor;
	D3DXHANDLE	m_hdLightDir;
	D3DXHANDLE	m_hdTex;

	D3DXHANDLE	m_hdTech;

public:
	CEx19_Effect03();
	~CEx19_Effect03();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

