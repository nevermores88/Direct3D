#include "stdafx.h"
#include "Shader_3dapi_03_11.h"


CShader_3dapi_03_11::CShader_3dapi_03_11()
{
}


CShader_3dapi_03_11::~CShader_3dapi_03_11()
{
}

HRESULT CShader_3dapi_03_11::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	//지형
	m_pField = new CField();
	if (m_pField)
	{
		if (FAILED(m_pField->Create(m_pdev, "Ex03_11/Field_Height10.raw", "Ex03_11/map_diffuse.png", "Ex03_11/map_dtail.png")))
			return E_FAIL;
	}

	//나무
	m_pTreeMesh = new CMesh();
	m_pTreeMesh->Create(m_pdev, "Ex03_11/pine04.x", "Ex03_11");

	m_iNumTree = 80;
	m_pTreeMatrices = new D3DXMATRIX[m_iNumTree];

	float fW= 16.0f;

	D3DXMATRIX mtR, mtY, mtX;

	for (int i = 0; i < m_iNumTree; ++i)
	{
		FLOAT fAngleT = -20.f + rand() % 41;
		FLOAT fAngleP = 0.f + rand() % 360;

		fAngleT *= (D3DX_PI / 180.F);
		fAngleP *= (D3DX_PI / 180.F);

		D3DXMatrixRotationY(&mtY, fAngleP);
		D3DXMatrixRotationX(&mtX, fAngleT);

		mtR = mtY * mtX;

		D3DXMatrixIdentity(&m_pTreeMatrices[i]);
		float x = 0;
		float y = 0;
		float z = 0;
		x = rand() % 65 * fW;
		z = rand() % 65 * fW;

		D3DXVECTOR3 vcIn(x, y, z);
		D3DXVECTOR3 vcOut;
		if (SUCCEEDED(m_pField->GetHeight(&vcOut, &vcIn)))
		{
			y = vcOut.y;
		}

		float fScl = 1;
		fScl = 10.f + rand() % 31;
		fScl *= .35f;

		m_pTreeMatrices[i]._11 = fScl;
		m_pTreeMatrices[i]._22 = fScl;
		m_pTreeMatrices[i]._33 = fScl;

		m_pTreeMatrices[i] *= mtR;

		m_pTreeMatrices[i]._41 = x;
		m_pTreeMatrices[i]._42 = y;
		m_pTreeMatrices[i]._43 = z;
	}

	return S_OK;
}

void CShader_3dapi_03_11::Release()
{
	if (m_pField)
	{
		delete m_pField;
		m_pField = NULL;
	}
}

void CShader_3dapi_03_11::Render()
{
	if (m_pdev)
	{
		//m_pdev->SetRenderState(D3DRS_LIGHTING, TRUE);
		D3DXMATRIX mtIdentity;

		m_pField->Render();

		for (int i = 0; i < m_iNumTree; i++)
		{
			m_pdev->SetTransform(D3DTS_WORLD, &m_pTreeMatrices[i]);
			m_pTreeMesh->Render();
		}

		D3DXMatrixIdentity(&mtIdentity);
		m_pdev->SetTransform(D3DTS_WORLD, &mtIdentity);
		m_pdev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	}
}

void CShader_3dapi_03_11::Update()
{
	if (m_pdev)
	{

	}
}