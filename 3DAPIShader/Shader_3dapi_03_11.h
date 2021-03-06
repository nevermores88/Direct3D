#pragma once

#include "Field.h"
#include "Mesh.h"
#include "RenderTarget.h"

class CShader_3dapi_03_11 : public CBaseClass
{
	struct Vertex
	{
		float x, y, z;
		D3DCOLOR color;
		float u, v;

		Vertex(){}
		Vertex(float _x, float _y, float _z, D3DCOLOR _color, float _u, float _v)
		{
			x = _x; y = _y; z = _z;
			color = _color;
			u = _u; v = _v;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1) };
	};

public:
	//����
	CField* m_pField;

	//����
	int m_iNumTree;
	CMesh* m_pTreeMesh;
	D3DXMATRIX* m_pTreeMatrices;
	
	//����Ÿ��
	IRenderTarget* m_pRenderTarget;

	//���̴�
	LPD3DXEFFECT m_pShader;
	LPDIRECT3DVERTEXDECLARATION9 m_pFVF;

	Vertex m_pVertex[4];
	LPDIRECT3DTEXTURE9 m_pTex;

public:
	CShader_3dapi_03_11();
	~CShader_3dapi_03_11();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();

	void RenderScene();
};

