#pragma once
class CField
{
public:
	struct VtxNDUV2
	{
		D3DXVECTOR3	p;
		D3DXVECTOR3	n;
		DWORD		d;
		FLOAT		u0, v0;
		FLOAT		u1, v1;

		VtxNDUV2() : p(0, 0, 0), n(0, 0, 0), d(0xFFFFFFFF){}

		VtxNDUV2(FLOAT X, FLOAT Y, FLOAT Z
			, FLOAT nX, FLOAT nY, FLOAT nZ
			, FLOAT U0, FLOAT V0
			, FLOAT U1, FLOAT V1
			, DWORD D = 0xffffffff) : p(X, Y, Z)
			, n(nX, nY, nZ)
			, u0(U0), v0(V0)
			, u1(U1), v1(V1)
			, d(D){}

		enum	{ FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2), };
	};

	struct VtxIdx
	{
		union	{ struct	{ WORD a;	WORD b;	WORD c; };	WORD m[3]; };

		VtxIdx()							{ a = 0;	  b = 1;		 c = 2; }
		VtxIdx(WORD A, WORD B, WORD C)		{ a = A;    b = B;		 c = C; }
		VtxIdx(WORD* R)						{ a = R[0]; b = R[1];	 c = R[2]; }
		operator WORD* ()					{ return (WORD *)&a; }
		operator CONST WORD* () const		{ return (CONST WORD *) &a; }
	};

	LPDIRECT3DDEVICE9 m_pDev;
	VtxNDUV2*	m_pVtx;
	VtxIdx*		m_pFce;

	int			m_TileN;
	FLOAT		m_TileW;
	int			m_nVtx;
	int			m_nFce;
	float		m_fHscl;
	float		m_fUV;

	LPDIRECT3DTEXTURE9	m_pTxDif;
	LPDIRECT3DTEXTURE9	m_pTxDet;

public:
	CField();
	~CField();

	INT	Create(LPDIRECT3DDEVICE9 pDev, char* sRaw, char* sTexDif, char* sTexDet);
	void	Destroy();

	INT	FrameMove();
	void	Render();

	INT	GetHeight(D3DXVECTOR3* pOut, const D3DXVECTOR3* pIn);

	void	SetupNormal();
	void	CalculateNormal(D3DXVECTOR3* pOut, D3DXVECTOR3* v0, D3DXVECTOR3* v1, D3DXVECTOR3* v2);

};

