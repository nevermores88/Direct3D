//---------------------------------------------------------------------------------------------------
//
//Description : Vertex Shader 01
//Original File : s0v_01_vertex01.zip
//
//---------------------------------------------------------------------------------------------------

#pragma once
class CShader_3dapi_01_01 : public CBaseClass
{
	struct Vertex
	{
		D3DXVECTOR3 p;

		Vertex(){}
		Vertex(float _x, float _y, float _z)
		{
			p.x = _x; p.y = _y; p.z = _z;
		}

		enum { FVF = (D3DFVF_XYZ) };
	};

	LPDIRECT3DVERTEXSHADER9	m_pVertexShader;
	LPDIRECT3DVERTEXDECLARATION9	m_pFVF;

	Vertex m_pVertices[3];

public:
	CShader_3dapi_01_01();
	~CShader_3dapi_01_01();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

