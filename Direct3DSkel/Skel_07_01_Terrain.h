#pragma once

class CSkel_Camera;

class CSkel_07_01_Terrain : public CBaseClass
{
	struct Vertex
	{
		D3DXVECTOR3 p;
		D3DXVECTOR3 n;
		D3DXVECTOR2 t;

		Vertex(){}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1) };
	};

	struct Index
	{
		WORD _0, _1, _2;
	};

	LPDIRECT3DTEXTURE9		m_pHeightTex;
	LPDIRECT3DTEXTURE9		m_pDiffuseTex;

	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;

	D3DXMATRIX	m_mtAni;

	DWORD	m_dwXHeight;
	DWORD	m_dwZHeight;

	CSkel_Camera*	m_pCamera;

	DWORD	m_dwMouseX;
	DWORD m_dwMouseY;

public:
	CSkel_07_01_Terrain();
	~CSkel_07_01_Terrain();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();

	void InitTexture();
	void InitVertexBuffer();
	void InitIndexBuffer();

	void SetupLights();

	void DrawMesh();

	void ProcessMouse();
	void ProcessKey();
};

