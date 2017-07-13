#pragma once
class CEx17_ToonOutline : public CBaseClass
{
	//Å÷
	LPDIRECT3DVERTEXSHADER9 m_pToonShader;
	LPD3DXCONSTANTTABLE	m_pToonConstTbl;

	LPD3DXMESH m_pMeshes[4];
	D3DXMATRIX m_mtWolrds[4];
	D3DXVECTOR4 m_MeshColors[4];

	LPDIRECT3DTEXTURE9 m_pTex;

	D3DXHANDLE m_hdToonWorldView;
	D3DXHANDLE m_hdToonWorldViewProj;
	D3DXHANDLE m_hdToonColor;
	D3DXHANDLE m_hdToonLightDir;

	//½Ç·ç¿§
	//CSilhouetteEdges m_pMeshOutlines[4];
	LPDIRECT3DVERTEXSHADER9 m_pOutlineShader;
	LPD3DXCONSTANTTABLE	m_pOutlineConstTbl;

	D3DXHANDLE m_hdOutlineWorldView;
	D3DXHANDLE m_hdOutlineProj;

	D3DXMATRIX m_mtView;

public:
	CEx17_ToonOutline();
	~CEx17_ToonOutline();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

class CSilhouetteEdges
{

	LPDIRECT3DDEVICE9 m_pdev;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;
	LPDIRECT3DVERTEXDECLARATION9 m_pDecl;

	UINT m_nNumVerts;
	UINT m_nNumFaces;

public:
	struct EdgeVertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 faceNormal1;
		D3DXVECTOR3 faceNormal2;
	};

	struct MeshVertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 normal;
		static const DWORD FVF;
	};

	void Create(LPDIRECT3DDEVICE9 pdev, LPD3DXMESH pMesh, LPD3DXBUFFER pAdjBuffer);
	void Release();

	void Render();

};