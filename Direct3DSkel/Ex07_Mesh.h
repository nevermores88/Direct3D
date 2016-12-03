#pragma once

class CEx07_Mesh : public CBaseClass
{
	LPD3DXMESH m_pMesh;
	D3DMATERIAL9* m_Materials;
	LPDIRECT3DTEXTURE9* m_pTextures;
	DWORD m_dwNumMaterials;

public:
	CEx07_Mesh();
	~CEx07_Mesh();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

