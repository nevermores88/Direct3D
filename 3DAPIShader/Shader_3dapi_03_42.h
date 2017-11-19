//---------------------------------------------------------------------------------------------------
//
//Description : Environment Mapping 06
//Original File : ht25_env2_cube4.zip
//
//---------------------------------------------------------------------------------------------------

#pragma once

class CMesh;

class CShader_3dapi_03_42 : public CBaseClass
{
	struct Vertex
	{
		D3DXVECTOR3 p;
		D3DXVECTOR3 n;

		Vertex(){}
		Vertex(float _x, float _y, float _z, float _nx, float _ny, float _nz)
		{
			p.x = _x; p.y = _y; p.z = _z;
			n.x = _nx; n.y = _ny; n.z = _nz;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_NORMAL) };
	};

	LPD3DXEFFECT m_pShader;
	LPDIRECT3DVERTEXDECLARATION9	m_pFVF;

	CMesh*	m_pMesh;
	CMesh*	m_pSkyBox;
	LPDIRECT3DCUBETEXTURE9		m_pCubeTex;

	D3DXMATRIX m_mtWorld;
	D3DXMATRIX m_mtRot;

	LPD3DXRenderToEnvMap	m_pRenderEnvMap;

	int m_iEnvOpt;

public:
	CShader_3dapi_03_42();
	~CShader_3dapi_03_42();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();

	void SetupCubeViewMatrix(D3DXMATRIX* pOut, DWORD dwFace);

	void RenderScene();
	void RenderScene(D3DXMATRIX* pView, D3DXMATRIX* pProj);
};

