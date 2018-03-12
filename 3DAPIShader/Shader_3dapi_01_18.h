//---------------------------------------------------------------------------------------------------
//
//Description :  Lighting 07
//Original File : s0v_07_6texture2.zip
//
//---------------------------------------------------------------------------------------------------

#pragma once
class CShader_3dapi_01_18 : public CBaseClass
{
	struct Vertex
	{
		D3DXVECTOR3 p;
		D3DXVECTOR3 n;
		D3DCOLOR color;
		float u, v;

		Vertex(){}
		Vertex(float _x, float _y, float _z, float _nx, float _ny, float _nz, float _u, float _v, D3DCOLOR _color = 0xFFFFFFFF)
		{
			p.x = _x; p.y = _y; p.z = _z;
			n.x = _nx; n.y = _ny; n.z = _nz;
			u = _u; v = _v;
			color = _color;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1) };
	};

	LPDIRECT3DVERTEXSHADER9	m_pVertexShader;
	LPDIRECT3DVERTEXDECLARATION9	m_pFVF;

	LPDIRECT3DTEXTURE9 m_pTex;

	D3DXMATRIX m_mtWorld;

	int m_iVertexNum;
	Vertex* m_pVertices;

public:
	CShader_3dapi_01_18();
	~CShader_3dapi_01_18();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

