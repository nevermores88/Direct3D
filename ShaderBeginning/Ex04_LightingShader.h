#pragma once
class CEx04_LightingShader : public CBaseClass
{
	LPD3DXMESH	m_pSphere;
	LPD3DXEFFECT m_pShader;

	float m_fRotY;

public:
	CEx04_LightingShader();
	~CEx04_LightingShader();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

