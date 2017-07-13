#pragma once
#include "Field.h"
#include "Mesh.h"
#include "RenderTarget.h"

class CShader_3dapi_19 : public CBaseClass
{
	struct Vertex
	{
		float x, y, z;
		float u, v;
		D3DCOLOR color;

		Vertex(){}
		Vertex(float _x, float _y, float _z, float _u, float _v, D3DCOLOR _color = 0)
		{
			x = _x;  y = _y; z = _z;
			u = _u; v = _v;
			color = _color;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_DIFFUSE) };
	};

	struct VtxRHWDUV1
	{
		D3DXVECTOR4	p;
		DWORD	d;
		FLOAT	u, v;

		VtxRHWDUV1() : p(0, 0, 0, 1), u(0), v(0), d(0xFFFFFFFF){}
		VtxRHWDUV1(FLOAT X, FLOAT Y
			, FLOAT U, FLOAT V
			, DWORD D = 0XFFFFFFFF) : p(X, Y, 0, 1), u(U), v(V), d(D){}

		enum { FVF = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1) };
	};

	/*LPDIRECT3DPIXELSHADER9	m_pShader;
	LPD3DXCONSTANTTABLE		m_pConstTable;
	LPDIRECT3DTEXTURE9			m_pTex;*/

	Vertex	m_Vertex[4];

	CField*	m_pField;

	CMesh*	m_pTreeMesh;
	int			m_nTreeNum;
	D3DXMATRIX*	m_matTrees;

	CMesh*	m_pSkyBox;

	//¼ÎÀÌ´õ°ü·Ã

	VtxRHWDUV1	m_pVtx[4];			// Vertex Buffer

	LPDIRECT3DVERTEXDECLARATION9		m_pFVF;						// Declarator
	LPDIRECT3DPIXELSHADER9					m_pShader;						// Pixel Shader
	LPD3DXCONSTANTTABLE						m_pConstTable;						// Constant Table

	LPDIRECT3DTEXTURE9							m_pTex;						// Texture 0

	//·»´õ Å¸±ê
	IRenderTarget*	m_pTrnd;

public:
	CShader_3dapi_19();
	~CShader_3dapi_19();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();

	void InitShader();
	void RenderShader();
};

