#pragma once
class CEx09_Text03 : public CBaseClass
{
	const int Width = 640;
	const int Height = 480;

	LPD3DXMESH				m_pMeshTxt;

	D3DXVECTOR3			m_LightDir;
	D3DXCOLOR				m_LightColor;
	D3DLIGHT9				m_Light;

	D3DMATERIAL9			m_Mtrl;

	float							m_fYRot;

	D3DXMATRIX				m_YMat;
	D3DXMATRIX				m_TMat;
	D3DXMATRIX				m_WorldMat;

public:
	CEx09_Text03();
	~CEx09_Text03();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();

	void DataClear();
};

