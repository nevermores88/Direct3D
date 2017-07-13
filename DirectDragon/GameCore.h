#pragma once
#include <d3d9.h>
#include "Manager.h"

class CGameCore
{
private:
	CManager*	m_pManager;

protected:
	LPDIRECT3D9 m_pd3d;
	LPDIRECT3DDEVICE9 m_pdev;
	HWND m_hWnd;

protected:
	//¼ø¼ö °¡»óÇÔ¼ö´Ï±ñ ½Ì±ÛÅæ ¸ø¸¸µë....
	/*virtual void OnInit() = 0;
	virtual void OnRender() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnRelease() = 0;*/
	virtual void OnInit(){};
	virtual void OnRender(){};
	virtual void OnUpdate(){};
	virtual void OnRelease(){};


public:
	CGameCore();
	~CGameCore();

	void	Create();
	void	Release();

	HRESULT InitD3D(HWND hWnd);
	void Render();
	void Update();
	void Cleanup();

	CManager*	GetManager(){ return m_pManager; }
};

