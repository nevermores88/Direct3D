#pragma once
class CEx07_NormalMapping : public CBaseClass
{
	LPD3DXMESH	m_pMesh;
	LPD3DXEFFECT m_pShader;

	LPDIRECT3DTEXTURE9 m_pTexDM;
	LPDIRECT3DTEXTURE9 m_pTexSM;
	LPDIRECT3DTEXTURE9 m_pTexNM;

	float m_fRotY;

public:
	CEx07_NormalMapping();
	~CEx07_NormalMapping();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

