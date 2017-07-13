#pragma once
class CEx17_Diffuse : public CBaseClass
{

	LPD3DXMESH		m_pTeapot;

	LPDIRECT3DVERTEXSHADER9	m_pDiffuseShader;
	LPD3DXCONSTANTTABLE			m_pDiffuseConstTable;

	D3DXHANDLE	m_hdViewMatrix;
	D3DXHANDLE	m_hdViewProjMatrix;
	D3DXHANDLE	m_hdAmbientMtrl;
	D3DXHANDLE	m_hdDiffuseMtrl;
	D3DXHANDLE	m_hdLightDir;

public:
	CEx17_Diffuse();
	~CEx17_Diffuse();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

