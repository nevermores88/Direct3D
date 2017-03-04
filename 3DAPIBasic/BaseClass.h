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
};



