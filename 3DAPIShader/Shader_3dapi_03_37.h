//---------------------------------------------------------------------------------------------------
//
//Description : Environment Mapping 01
//
//---------------------------------------------------------------------------------------------------

#pragma once

class CMesh;

class CShader_3dapi_03_37 : public CBaseClass
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
	LPDIRECT3DTEXTURE9		m_pEnvTex;

	D3DXMATRIX m_mtWorld;
	D3DXMATRIX m_mtRot;

public:
	CShader_3dapi_03_37();
	~CShader_3dapi_03_37();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

