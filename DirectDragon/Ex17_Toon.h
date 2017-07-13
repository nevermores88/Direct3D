#pragma once
class CEx17_Toon : public CBaseClass
{
	LPDIRECT3DVERTEXSHADER9	m_pToonShader;
	LPD3DXCONSTANTTABLE			m_pToonConstTable;

	LPD3DXMESH		m_pMeshes[4];
	D3DXVECTOR4	m_MeshColors[4];
	D3DXMATRIX		m_mtWorldMatrices[4];

	LPDIRECT3DTEXTURE9		m_pTex;

	D3DXHANDLE	m_hdWorldView;
	D3DXHANDLE	m_hdWorldViewProj;
	D3DXHANDLE	m_hdColor;
	D3DXHANDLE	m_hdLightDir;

	D3DXMATRIX m_mtView;

public:
	CEx17_Toon();
	~CEx17_Toon();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

