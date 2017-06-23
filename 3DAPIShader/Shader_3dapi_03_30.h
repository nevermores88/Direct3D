//---------------------------------------------------------------------------------------------------
//
//Description : Bump Mapping 03
//
//---------------------------------------------------------------------------------------------------

#pragma once
class CShader_3dapi_03_30 : public CBaseClass
{
	struct Vertex
	{
		D3DXVECTOR3 p;
		D3DXVECTOR3 n;
		float u, v;
		D3DXVECTOR3 t;

		Vertex(){}
		Vertex(float _x, float _y, float _z, float _u, float _v)
		{
			p.x = _x; p.y = _y; p.z = _z;
			u = _u, v = _v;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2 | D3DFVF_TEXCOORDSIZE3(1)) };
	};

	LPD3DXEFFECT m_pShader;

	int m_iVertexNum;
	Vertex*	m_pVertices;
	LPDIRECT3DVERTEXDECLARATION9	m_pFVF;

	LPDIRECT3DTEXTURE9		m_pNormalTex;

	D3DXVECTOR3	m_vLightDir;

public:
	CShader_3dapi_03_30();
	~CShader_3dapi_03_30();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

