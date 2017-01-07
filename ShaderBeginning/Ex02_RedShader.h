#pragma once
class CEx02_RedShader : public CBaseClass
{
	LPD3DXMESH	m_pSphere;
	LPD3DXEFFECT m_pShader;   //컴파일안해도 되는 쉐이더 코드인듯

public:
	CEx02_RedShader();
	~CEx02_RedShader();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

