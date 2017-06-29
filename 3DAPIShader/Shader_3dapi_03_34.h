//---------------------------------------------------------------------------------------------------
//
//Description : Specular Mapping 02
//
//---------------------------------------------------------------------------------------------------

#pragma once
class CShader_3dapi_03_34 : public CBaseClass
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
			u = _u, v = _v;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1) };
	};

	LPD3DXEFFECT m_pShader;

	int m_iVertexNum;
	Vertex	m_pVertices[4];
	LPDIRECT3DVERTEXDECLARATION9	m_pFVF;

	LPDIRECT3DTEXTURE9		m_pDiffuseTex;
	LPDIRECT3DTEXTURE9		m_pNormalTex;
	LPDIRECT3DTEXTURE9		m_pSpecularTex;

	DWORD	m_dwTFactor;

public:
	CShader_3dapi_03_34();
	~CShader_3dapi_03_34();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

