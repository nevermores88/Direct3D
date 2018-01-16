//---------------------------------------------------------------------------------------------------
//
//Description : Vertex Shader 03
//Original File : s0v_01_vertex03.zip
//
//---------------------------------------------------------------------------------------------------

#pragma once
class CShader_3dapi_01_03 : public CBaseClass
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

	Vertex m_pVertices[3];

public:
	CShader_3dapi_01_03();
	~CShader_3dapi_01_03();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

