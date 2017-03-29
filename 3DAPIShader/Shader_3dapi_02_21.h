#pragma once

#include "Field.h"
#include "Mesh.h"
#include "RenderTarget.h"

class CShader_3dapi_02_21 : public CBaseClass
{
	struct Vertex
	{
		float x, y, z, w;
		D3DCOLOR color;
		float u, v;

		Vertex(){}
		Vertex(float _x, float _y, float _z, D3DCOLOR _color, float _u, float _v)
		{
			x = _x; y = _y; z = _z; w = 1;
			color = _color;
			u = _u; v = _v;
		}

		enum { FVF = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1) };
	};

	//ÁöÇü
	CField* m_pField;

	//³ª¹«
	int m_iNumTree;
	CMesh* m_pTreeMesh;
	D3DXMATRIX* m_pTreeMatrices;

	//·»´õÅ¸°Ù
	IRenderTarget* m_pRenderTarget;

	//¼ÎÀÌ´õ
	LPDIRECT3DPIXELSHADER9 m_pShader;
	LPD3DXCONSTANTTABLE	m_pConstTbl;

	LPDIRECT3DVERTEXDECLARATION9 m_pFVF;

	Vertex m_pVertex[4];
	LPDIRECT3DTEXTURE9 m_pTex;

	LPD3DXMESH m_pTeapot;

public:
	CShader_3dapi_02_21();
	~CShader_3dapi_02_21();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();

	void RenderScene();
};

