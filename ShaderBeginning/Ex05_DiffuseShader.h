#pragma once
class CEx05_DiffuseShader : public CBaseClass
{
	LPD3DXMESH	m_pSphere;
	LPD3DXEFFECT m_pShader;
	LPDIRECT3DTEXTURE9 m_pTexDM;
	LPDIRECT3DTEXTURE9 m_pTexSM;

	float m_fRotY;

public:
	CEx05_DiffuseShader();
	~CEx05_DiffuseShader();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

