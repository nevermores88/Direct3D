#include "stdafx.h"
#include "Shader_3dapi_03_10.h"


CShader_3dapi_03_10::CShader_3dapi_03_10()
{
}


CShader_3dapi_03_10::~CShader_3dapi_03_10()
{
}

HRESULT CShader_3dapi_03_10::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pShader = LoadShader("Ex03_10/Shader.fx");
	if (!m_pShader)
		return E_FAIL;

	DWORD dwFVF = Vertex::FVF;
	D3DVERTEXELEMENT9 vertex_decl[MAX_FVF_DECL_SIZE];
	D3DXDeclaratorFromFVF(dwFVF, vertex_decl);
	if (FAILED(m_pdev->CreateVertexDeclaration(vertex_decl, &m_pFVF)))
		return E_FAIL;

	if (FAILED(D3DXCreateCylinder(m_pdev, 6, 6, 1000, 30, 5, &m_pMesh1, NULL)))
		return E_FAIL;

	if (FAILED(D3DXCreateCylinder(m_pdev, 10, 8, 1000, 30, 10, &m_pMesh2, NULL)))
		return E_FAIL;

	return S_OK;
}

void CShader_3dapi_03_10::Release()
{
	if (m_pShader)
	{
		m_pShader->Release();
		m_pShader = NULL;
	}

	if (m_pMesh1)
	{
		m_pMesh1->Release();
		m_pMesh1 = NULL;
	}

	if (m_pMesh2)
	{
		m_pMesh2->Release();
		m_pMesh2 = NULL;
	}

	if (m_pFVF)
	{
		m_pFVF->Release();
		m_pFVF = NULL;
	}
}

void CShader_3dapi_03_10::Render()
{
	if (m_pdev)
	{
		D3DXMATRIX mtView;
		D3DXMATRIX mtViewInv;
		D3DXMATRIX mtProj;

		D3DXVECTOR4 vLightDir(1.0f, 0.0f, 0.0f, 0.0f);				//광원방향
		D3DXVECTOR3 vCamZ(0, 0, 0);									//Camera Z Axis
		D3DXVECTOR3 vGlowAxis(0, 0, 1);									//Glow Direction

		float fGlowThick = 40.0f;
		float fBright;
		D3DXVECTOR4 vGlowColor(1, 1.0f, 0.2f, 1.0f);
		D3DXVECTOR4 vColor;
		
		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);
		D3DXMatrixInverse(&mtViewInv, NULL, &mtView);
		
		// 시선에 대한 Normal Vector: Nor = dot(AxsZ, CamZ) * AxsZ - CamZ. Nor ⊥ (0,0,1)
		vCamZ = D3DXVECTOR3(mtViewInv._41, mtViewInv._42, mtViewInv._43);		//Camera Z Axis
		float a = D3DXVec3Dot(&vGlowAxis, &vCamZ);			// a = dot(vGlowAxis, vCamZ)
		D3DXVECTOR4 vNormal = a*vGlowAxis - vCamZ;			// vNormal = a * vGlowAxis - vCamZ
		vNormal.w = 0;
		D3DXVec4Normalize(&vNormal, &vNormal);
	
		HRESULT hr = 0;
		hr = m_pShader->SetMatrix("g_mtWorld", &m_mtWorld);
		hr = m_pShader->SetMatrix("g_mtView", &mtView);
		hr = m_pShader->SetMatrix("g_mtProj", &mtProj);
		
		hr = m_pShader->SetVector("g_vNormal", &vNormal);

		m_pdev->SetVertexDeclaration(m_pFVF);

		m_pShader->SetTechnique("Tech0");

		UINT numPasses = 0;
		m_pShader->Begin(&numPasses, NULL);

		{
			m_pShader->BeginPass(0);
			{
				m_pShader->SetVector("g_vColor", &vGlowColor);
				m_pMesh1->DrawSubset(0);
			}
			m_pShader->EndPass();
		}

		/*UINT numPasses = 0;

		m_pShader->Begin(&numPasses, NULL);

		for (UINT i = 0; i < numPasses; ++i)
		{
			m_pShader->BeginPass(i);
			{
				switch (i)
				{
				case 0:
				{
					fBright = (94 + rand() % 7)*0.01;
					vColor = D3DXVECTOR4(0, 0, 1, 1);
					vColor *= fBright;
					m_pShader->SetVector("g_vColor", &vColor);
					m_pMesh2->DrawSubset(0);
				}
				break;
				case 1:
				{
					fBright = (94 + rand() % 7)*0.01;
					vColor = D3DXVECTOR4(0.2f, 1, 0, 0.1f);
					vColor *= fBright;
					m_pShader->SetVector("g_vColor", &vColor);
					m_pMesh1->DrawSubset(0);
				}
				break;
				}
			}
			m_pShader->EndPass();
		}*/

		m_pShader->End();

		m_pdev->SetVertexShader(NULL);
		m_pdev->SetPixelShader(NULL);
		m_pdev->SetVertexDeclaration(NULL);
	}
}

void CShader_3dapi_03_10::Update()
{
	if (m_pdev)
	{
		static float c = 0;

		c += 25.f * 0.05f;
		//	c=90;
		if (c > 360.f)
			c -= 360.f;

		D3DXMATRIX	mtY;
		D3DXMATRIX	mtZ;

		// Update World Matrix
		D3DXMatrixRotationY(&mtY, D3DXToRadian(-c));
		D3DXMatrixRotationZ(&mtZ, D3DXToRadian(-23.5f));
		m_mtWorld = mtY * mtZ;
	}
}
