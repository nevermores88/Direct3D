#pragma once
class CBaseClass
{
protected:
	LPDIRECT3DDEVICE9 m_pdev;

public:
	CBaseClass();
	~CBaseClass();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release(){};

	virtual void Render(){};
	virtual void Update(){};

	LPD3DXEFFECT LoadShader(const char* szFileName);
	LPD3DXMESH LoadModel(const char* szFileName);
	LPDIRECT3DTEXTURE9 LoadTexture(const char* szFileName);
};

