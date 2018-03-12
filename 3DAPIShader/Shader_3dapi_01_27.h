//---------------------------------------------------------------------------------------------------
//
//Description :  Toon Shading 04
//Original File : s0v_10_toon2_diffuse+toon.zip
//
//---------------------------------------------------------------------------------------------------

#pragma once
class CShader_3dapi_01_27 : public CBaseClass
{
	struct Vertex
	{
		D3DXVECTOR3 p;
		D3DXVECTOR3 n;
		float u, v;

		Vertex(){}
		Vertex(float _x, float _y, float _z, float _nx, float _ny, float _nz, float _u, float _v)
		{
			p.x = _x; p.y = _y; p.z = _z;
			n.x = _nx; n.y = _ny; n.z = _nz;
			u= _u; v = _v;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1) };
	};

	LPDIRECT3DVERTEXSHADER9	m_pVertexShader;
	LPDIRECT3DVERTEXDECLARATION9	m_pFVF;

	LPDIRECT3DTEXTURE9 m_pDiffuseTex;
	LPDIRECT3DTEXTURE9 m_pToonTex;

	D3DXMATRIX m_mtWorld;

	int m_iVertexNum;
	Vertex* m_pVertices;

public:
	CShader_3dapi_01_27();
	~CShader_3dapi_01_27();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

