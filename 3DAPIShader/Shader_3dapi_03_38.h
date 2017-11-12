//---------------------------------------------------------------------------------------------------
//
//Description : Environment Mapping 02
//Original File : ht25_env1_sphere2.zip, ht25_env1_sphere3.zip
//
//---------------------------------------------------------------------------------------------------

#pragma once

class CMesh;

class CShader_3dapi_03_38 : public CBaseClass
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
	LPDIRECT3DTEXTURE9		m_pSphereTex;

	D3DXMATRIX m_mtWorld;
	D3DXMATRIX m_mtRot;

	//환경
	LPD3DXRenderToEnvMap	m_pRenderEnv;

	CMesh*	m_pSkyBox;

	//스프라이트
	LPD3DXSPRITE	m_pSprite;

public:
	CShader_3dapi_03_38();
	~CShader_3dapi_03_38();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();

	void RenderScene();
	void RenderScene(D3DXMATRIX* pView, D3DXMATRIX* pProj);
	void RenderSphereTex();

	void Init3DEnvrionment();
	void T_SetupCubeViewMatrix(D3DXMATRIX* pViewMatrix, DWORD dwFace);
};

