#pragma once
class CEx06_ToonShader : public CBaseClass
{
	LPD3DXMESH	m_pMesh;
	LPD3DXEFFECT m_pShader;

	float m_fRotY;

public:
	CEx06_ToonShader();
	~CEx06_ToonShader();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

