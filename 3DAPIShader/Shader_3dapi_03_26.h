//---------------------------------------------------------------------------------------------------
//
//Description : Lighting Mapping 01
//
//---------------------------------------------------------------------------------------------------

#pragma once
class CShader_3dapi_03_26 : public CBaseClass
{
	struct Vertex
	{
		D3DXVECTOR3 p;
		D3DXVECTOR3 n;

		Vertex(){}
		Vertex(float _x, float _y, float _z, float _nx, float _ny, float _nz, float _u, float _v)
		{
			p.x = _x; p.y = _y; p.z = _z;
			n.x = _nx; n.y = _ny; n.z = _nz;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1) };
	};

	LPD3DXEFFECT m_pShader;

	Vertex	m_pVertices[4];

	LPDIRECT3DTEXTURE9		m_pDiffuseTex;
	LPDIRECT3DTEXTURE9		m_pLightTex;

	D3DXMATRIX m_mtWorld;
	D3DXMATRIX m_mtRot;

public:
	CShader_3dapi_03_26();
	~CShader_3dapi_03_26();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

