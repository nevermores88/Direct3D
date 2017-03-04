#pragma once
class CShader_3dapi_03_12 : public CBaseClass
{
	struct LcTexture
	{
		int m_iImgWidth;		// Original Image Width
		int m_iImgHeight;		// Original Image Height
		int m_iImgDepth;		// Original Image Depth

		int m_iTexWidth;			//Texture Width
		int m_iTexHeight;		//Texture Height
		
		LPDIRECT3DTEXTURE9 m_pTex;

		LcTexture()
		{
			m_iImgWidth = 0;
			m_iImgHeight = 0;
			m_iImgDepth = 0;

			m_iTexWidth = 0;
			m_iTexHeight = 0;

			m_pTex = NULL;
		}
	};						   
	
	struct Vertex
	{
		D3DXVECTOR2 p;
		float z;
		float w;
		D3DXVECTOR2 t;

		Vertex(){}
		Vertex(float _x, float _y, float _z, float _w, float _tx, float _ty)
		{
			p.x = _x; p.y = _y;
			z = _z; w = _w;
			t.x = _tx; t.y = _ty;
		}

		enum { FVF = (D3DFVF_XYZRHW | D3DFVF_TEX1) };
	};

	LPD3DXEFFECT m_pShader;
	LPDIRECT3DVERTEXDECLARATION9 m_pFVF;

	Vertex m_pVertices[4];

	int m_iScreenWidth;
	int m_iScreenHeight;

public:
	CShader_3dapi_03_12();
	~CShader_3dapi_03_12();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

