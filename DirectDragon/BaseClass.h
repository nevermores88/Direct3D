#pragma once
class CBaseClass
{
protected:
	LPDIRECT3DDEVICE9 m_pdev;

	DWORD		m_dwExType;

public:
	CBaseClass();
	~CBaseClass();

	virtual void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType) = 0;
	virtual void Release() = 0;

	virtual void OnRender() = 0;
	virtual void OnUpdate() = 0;
};

