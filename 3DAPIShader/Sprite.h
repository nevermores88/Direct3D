#pragma once

struct StTexture
{
	int m_iImgWidth;
	int m_iImgHeight;
	int m_iImgDepth;

	int m_iTexWidth;
	int m_iTexHeight;

	LPDIRECT3DTEXTURE9		m_pTex;

	StTexture()
	{
		m_iImgWidth = 0;
		m_iImgHeight = 0;
		m_iImgDepth = 0;

		m_iTexWidth = 0;
		m_iTexHeight = 0;

		m_pTex = NULL;
	}
};

interface ISprite
{
	virtual int DrawEx(void*pTex, RECT* pRect, D3DXVECTOR2* pScale, D3DXVECTOR2* pRot, float fRot, D3DXVECTOR2* pTrl, DWORD dwColor, int iMono) = 0;
	virtual int OnResetDevice() = 0;
	virtual void OnLostDevice() = 0;
};

class CSprite : public ISprite
{
	struct Vertex
	{
		D3DXVECTOR2 p;
		float z, w;
		float u, v;

		Vertex(){}
		Vertex(float _x, float _y, float _z = 0, float _w = 1, float _u = 0, float _v = 0)
		{
			p.x = _x; p.y = _y;
			z = _z; w = _w;
			u = _u; v = _v;
		}

		enum { FVF = (D3DFVF_XYZRHW | D3DFVF_TEX1) };
	};

	LPDIRECT3DDEVICE9	m_pdev;
	HWND	m_hWnd;

	LPD3DXEFFECT	m_pShader;
	LPDIRECT3DVERTEXDECLARATION9	m_pFVF;
	Vertex	m_pVertices[4];

	int m_iScreenWidth;
	int m_iScreenHeight;

public:
	CSprite();
	~CSprite();

	virtual int DrawEx(void* _pTex, RECT* _pRect, D3DXVECTOR2* _pScale, D3DXVECTOR2* _pRot, float _fRot, D3DXVECTOR2* _pTrn, DWORD _dwColor, int _iMono);
	virtual int OnResetDevice();
	virtual void OnLostDevice();

	HRESULT Create(LPDIRECT3DDEVICE9 pdev, char* szFileName);
	void Release();
};

