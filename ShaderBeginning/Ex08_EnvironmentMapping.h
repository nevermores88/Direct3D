#pragma once
class CEx08_EnvironmentMapping : public CBaseClass
{
	LPD3DXMESH	m_pMesh;
	LPD3DXEFFECT m_pShader;

	LPDIRECT3DTEXTURE9 m_pTexDM;
	LPDIRECT3DTEXTURE9 m_pTexSM;
	LPDIRECT3DTEXTURE9 m_pTexNM;
	LPDIRECT3DCUBETEXTURE9 m_pSnowENV;

	float m_fRotY;

public:
	CEx08_EnvironmentMapping();
	~CEx08_EnvironmentMapping();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

