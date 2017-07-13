#pragma once
class CShader_3dapi_27 : public CBaseClass
{
	struct Vertex
	{
		float x, y, z;
		float u, v;
		//D3DCOLOR color;

		Vertex(){}
		Vertex(float _x, float _y, float _z, float _u, float _v/*, D3DCOLOR _color*/)
		{
			x = _x;  y = _y; z = _z;
			u = _u; v = _v;
			//color = _color;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_TEX1 /*| D3DFVF_DIFFUSE*/) };
	};

	Vertex	m_pVertex[4];
	LPDIRECT3DTEXTURE9			m_pTex;

public:
	CShader_3dapi_27();
	~CShader_3dapi_27();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();
};

