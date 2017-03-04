#pragma once
class CEx09_UVAnimation : public CBaseClass
{
	LPD3DXMESH m_pMesh;
	LPD3DXEFFECT m_pShader;

	LPDIRECT3DTEXTURE9 m_pTexDM;
	LPDIRECT3DTEXTURE9 m_pTexSM;

	float m_fRotY;

public:
	CEx09_UVAnimation();
	~CEx09_UVAnimation();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

