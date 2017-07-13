#pragma once
class CMesh
{
	LPDIRECT3DDEVICE9	m_pdev;

	LPD3DXMESH	m_pMesh;
	std::vector<D3DMATERIAL9>	m_vMtrls;
	std::vector<LPDIRECT3DTEXTURE9>	m_vTextures;

	DWORD	m_dwNumMtrl;

public:
	CMesh();
	~CMesh();

	void Create(LPDIRECT3DDEVICE9 pdev, wchar_t* wsFileName, char* szTexPath);
	void Release();

	void OnRender();
	void OnUpdate();
};

