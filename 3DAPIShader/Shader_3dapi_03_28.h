//---------------------------------------------------------------------------------------------------
//
//Description : Bump Mapping 01
//
//---------------------------------------------------------------------------------------------------

#pragma once
class CShader_3dapi_03_28 : public CBaseClass
{
	struct Vertex
	{
		D3DXVECTOR3 p;
		float u, v;

		Vertex(){}
		Vertex(float _x, float _y, float _z, float _u, float _v)
		{
			p.x = _x; p.y = _y; p.z = _z;
			u = _u, v = _v;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_TEX1) };
	};

	LPD3DXEFFECT m_pShader;

	Vertex	m_pVertices[4];

	LPDIRECT3DTEXTURE9		m_pDiffuseTex;
	LPDIRECT3DTEXTURE9		m_pNormalTex;

	DWORD	m_dwTFactor;

public:
	CShader_3dapi_03_28();
	~CShader_3dapi_03_28();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

