//---------------------------------------------------------------------------------------------------
//
//Description : Transform
//Original File : s0v_04_transform.zip
//
//---------------------------------------------------------------------------------------------------

#pragma once
class CShader_3dapi_01_06 : public CBaseClass
{
	struct Vertex
	{
		D3DXVECTOR3 p;
		D3DCOLOR color;

		Vertex(){}
		Vertex(float _x, float _y, float _z, D3DCOLOR _color)
		{
			p.x = _x; p.y = _y; p.z = _z;
			color = _color;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE) };
	};

	LPDIRECT3DVERTEXSHADER9	m_pVertexShader;
	LPDIRECT3DVERTEXDECLARATION9	m_pFVF;

	Vertex m_pVertices[4];

public:
	CShader_3dapi_01_06();
	~CShader_3dapi_01_06();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

