#pragma once
class CMesh
{
	LPDIRECT3DDEVICE9		m_pdev;

	LPD3DXMESH	m_pMesh;
	D3DMATERIAL9*	m_pMtrls;
	LPDIRECT3DTEXTURE9* m_pTextures;
	DWORD m_dwNumMtrl;

public:
	CMesh();
	~CMesh();

	HRESULT Create(LPDIRECT3DDEVICE9 pdev, char* xFileName, char* szPath = NULL);
	void Release();

	void Render(bool bUseMaterial = TRUE, bool bUseFixed = TRUE);
	void Update();
};

