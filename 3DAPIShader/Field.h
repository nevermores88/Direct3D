#pragma once
class CField
{
	struct Vertex
	{
		D3DXVECTOR3 p;
		D3DXVECTOR3 n;
		DWORD color;
		float u0, v0;
		float u1, v1;

		Vertex(){}
		Vertex(float _x, float _y, float _z, float _nx, float _ny, float _nz, float _u0, float _v0, float _u1, float _v1, DWORD _color = 0xffffffff)
		{
			p.x = _x; p.y = _y; p.z = _z;
			n.x = _nx; n.y = _ny; n.z = _nz;
			color = _color;
			_u0 = u0, _v0 = v0;
			u1 = _u1, v1 = _v1;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2) };
	};

	struct Index
	{
		WORD m[3];

		Index(){ m[0] = 0, m[1] = 1, m[2] = 2; }
		Index(WORD _0, WORD _1, WORD _2){ m[0] = _0, m[1] = _1, m[2] = _2; }
	};

	LPDIRECT3DDEVICE9 m_pdev;
	Vertex* m_pVertex;
	Index* m_pIndex;

	int m_TileN;
	int m_TileW;
	int m_nVtx;
	int m_nFce;
	float m_fHscl;
	float m_fUV;

	LPDIRECT3DTEXTURE9	m_pTxDif;
	LPDIRECT3DTEXTURE9	m_pTxDet;

public:
	CField();
	~CField();

	HRESULT	Create(LPDIRECT3DDEVICE9 pdev, char* sRaw, char* sTexDif, char* sTexDet);
	void	Release();

	void	Update();
	void	Render();

	int		GetHeight(D3DXVECTOR3* pOut, const D3DXVECTOR3* pIn);

	void	SetupNormal();
	void	CalculateNormal(D3DXVECTOR3* pOut, D3DXVECTOR3* v0, D3DXVECTOR3* v1, D3DXVECTOR3* v2);
};

