#pragma once
class CEx05_07Skinning : public CBaseClass
{
	struct Vertex
	{
		float x, y, z;
		float b[3];		//blend weight
		DWORD index;	//blend index
		D3DCOLOR color;
		float u, v;

		Vertex(){}
		Vertex(float _x, float _y, float _z, float* _b, D3DCOLOR _color)
		{
			x = _x; y = _y; z = _z;
			color = _color;
		}

		/// D3DFVF_XYZB4 : 4개의 blend값 
		/// D3DFVF_LASTBETA_UBYTE4 : 마지막 DWORD index값은 unisgned byte형 4개를 나타냄
		enum { FVF = (D3DFVF_XYZB4| D3DFVF_LASTBETA_UBYTE4 | D3DFVF_DIFFUSE | D3DFVF_TEX1) };
	};

	struct Index
	{
		WORD _0, _1, _2;
	};

	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;

	LPDIRECT3DTEXTURE9		m_pTex;

	D3DXMATRIX	m_mt01;
	D3DXMATRIX m_mt02;

	bool m_bAnim;
	bool m_bWireframe;

public:
	CEx05_07Skinning();
	~CEx05_07Skinning();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();

	void Clear();
	void DrawMesh();
	void KeyInput();
};

