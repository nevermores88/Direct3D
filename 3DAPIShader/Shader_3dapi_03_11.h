#pragma once

#include "Field.h"
#include "Mesh.h"

class CShader_3dapi_03_11 : public CBaseClass
{
public:
	//지형
	CField* m_pField;

	//나무
	int m_iNumTree;
	CMesh* m_pTreeMesh;
	D3DXMATRIX* m_pTreeMatrices;

	LPD3DXEFFECT m_pShader;
	
public:
	CShader_3dapi_03_11();
	~CShader_3dapi_03_11();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

