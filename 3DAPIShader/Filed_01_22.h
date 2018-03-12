#pragma once
class CFiled_01_22
{
public:
	struct VtxIdx
	{
		union	{ struct	{ WORD a;	WORD b;	WORD c; };	WORD m[3]; };

		VtxIdx()						{ a = 0;	  b = 1;		 c = 2; }
		VtxIdx(WORD A, WORD B, WORD C)	{ a = A;    b = B;		 c = C; }
		VtxIdx(WORD* R)					{ a = R[0]; b = R[1];	 c = R[2]; }
		operator WORD* ()				{ return (WORD *)&a; }
		operator const WORD* () const	{ return (const WORD *)&a; }
	};

	struct VtxDUV1
	{
		D3DXVECTOR3	p;
		DWORD		d;

		VtxDUV1()	{}
		VtxDUV1(FLOAT X, FLOAT Y, FLOAT Z, DWORD D = 0xFFFFFFFF) :p(X, Y, Z), d(D){}
		enum	{ FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE), };
	};

public:
	CFiled_01_22();
	~CFiled_01_22();

	INT		Create(LPDIRECT3DDEVICE9 pDev);
	void	Destroy();

	INT		FrameMove();
	void	Render();

protected:
	LPDIRECT3DDEVICE9	m_pDev;

	INT			m_iN;														// Number of tile for Width
	INT			m_iW;														// Width of tile for x;

	INT			m_iNvx;														// Vertex Number
	INT			m_iNix;														// Index Number
	INT			m_iVxS;														// Vertex Size
	DWORD		m_dFVF;


	VtxDUV1*	m_pVtx;
	VtxIdx*		m_pIdx;
};

