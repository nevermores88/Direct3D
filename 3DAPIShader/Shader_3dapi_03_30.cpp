#include "stdafx.h"
#include "Shader_3dapi_03_30.h"


CShader_3dapi_03_30::CShader_3dapi_03_30()
{
}


CShader_3dapi_03_30::~CShader_3dapi_03_30()
{
}

HRESULT CShader_3dapi_03_30::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pShader = LoadShader("Ex03_30/Shader.fx");
	if (!m_pShader)
		return E_FAIL;

	D3DXCreateTextureFromFile(m_pdev, "Ex03_30/rock_n.tga", &m_pNormalTex);

	DWORD dwFVF = Vertex::FVF;
	D3DVERTEXELEMENT9	vertex_decl[MAX_FVF_DECL_SIZE];
	D3DXDeclaratorFromFVF(dwFVF, vertex_decl);
	if (FAILED(m_pdev->CreateVertexDeclaration(vertex_decl, &m_pFVF)))
		return E_FAIL;

	// Create Vertex
	INT	iNSphereSegments = 128;
	m_iVertexNum = 2 * iNSphereSegments*(iNSphereSegments + 1);

	FLOAT fDeltaRingAngle = (D3DX_PI / iNSphereSegments);
	FLOAT fDeltaSegAngle = (2.0f * D3DX_PI / iNSphereSegments);

	m_pVertices = new Vertex[m_iVertexNum];
	Vertex* pVertices = m_pVertices;

	// Generate the group of rings for the sphere
	for (DWORD ring = 0; ring < iNSphereSegments; ring++)
	{
		FLOAT r0 = 50 * sinf((ring + 0) * fDeltaRingAngle);
		FLOAT r1 = 50 * sinf((ring + 1) * fDeltaRingAngle);
		FLOAT r2 = 50 * sinf((ring + 2) * fDeltaRingAngle);
		FLOAT y0 = 50 * cosf((ring + 0) * fDeltaRingAngle);
		FLOAT y1 = 50 * cosf((ring + 1) * fDeltaRingAngle);
		FLOAT y2 = 50 * cosf((ring + 2) * fDeltaRingAngle);

		// Generate the group of segments for the current ring
		for (DWORD seg = 0; seg < (iNSphereSegments + 1); seg++)
		{
			FLOAT x0 = r0 * sinf(seg * fDeltaSegAngle);
			FLOAT z0 = r0 * cosf(seg * fDeltaSegAngle);
			FLOAT x1 = r1 * sinf(seg * fDeltaSegAngle);
			FLOAT z1 = r1 * cosf(seg * fDeltaSegAngle);
			FLOAT x2 = r2 * sinf(seg * fDeltaSegAngle);
			FLOAT z2 = r2 * cosf(seg * fDeltaSegAngle);

			// Add two vertices to the strip which makes up the sphere
			// (using the transformed normal to generate texture coords)
			pVertices->p.x = x0;
			pVertices->p.y = y0;
			pVertices->p.z = z0;
			pVertices->n.x = x0;
			pVertices->n.y = y0;
			pVertices->n.z = z0;

			pVertices->t = (D3DXVECTOR3(x1, y1, z1) - D3DXVECTOR3(x0, y0, z0));

			D3DXVec3Normalize(&pVertices->n, &pVertices->n);
			D3DXVec3Normalize(&pVertices->t, &pVertices->t);

			pVertices->u = -((FLOAT)seg) / iNSphereSegments;
			pVertices->v = (ring + 0) / (FLOAT)iNSphereSegments;
			pVertices++;

			pVertices->p.x = x1;
			pVertices->p.y = y1;
			pVertices->p.z = z1;
			pVertices->n.x = x1;
			pVertices->n.y = y1;
			pVertices->n.z = z1;

			pVertices->t = (D3DXVECTOR3(x2, y2, z2) - D3DXVECTOR3(x1, y1, z1));

			D3DXVec3Normalize(&pVertices->n, &pVertices->n);
			D3DXVec3Normalize(&pVertices->t, &pVertices->t);

			pVertices->u = -((FLOAT)seg) / iNSphereSegments;
			pVertices->v = (ring + 1) / (FLOAT)iNSphereSegments;
			pVertices++;
		}
	}

	return S_OK;
}

void CShader_3dapi_03_30::Release()
{
	if (m_pShader)
	{
		m_pShader->Release();
		m_pShader = NULL;
	}

	if (m_pFVF)
	{
		m_pFVF->Release();
		m_pFVF = NULL;
	}

	if (m_pNormalTex)
	{
		m_pNormalTex->Release();
		m_pNormalTex = NULL;
	}

	if (m_pVertices)
	{
		delete[] m_pVertices;
		m_pVertices = NULL;
	}
}

void CShader_3dapi_03_30::Render()
{
	if (m_pdev)
	{
		D3DXMATRIX mtWorld;
		D3DXMATRIX mtView;
		D3DXMATRIX mtProj;
		D3DXMATRIX mtRot;
		D3DXVECTOR4 vLightDir;

		static float fRot = 0.0f;
		fRot += 0.2f;
		float fTime = -D3DXToRadian(fRot);

		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);

		vLightDir = D3DXVECTOR4(m_vLightDir.x, m_vLightDir.y, m_vLightDir.z, 0);

		D3DXVec4Normalize(&vLightDir, &vLightDir);
		D3DXMatrixRotationY(&mtRot, fTime);
		mtWorld = mtRot;

		m_pdev->SetVertexDeclaration(m_pFVF);

		m_pShader->SetMatrix("g_mtWorld", &mtWorld);
		m_pShader->SetMatrix("g_mtView", &mtView);
		m_pShader->SetMatrix("g_mtProj", &mtProj);
		m_pShader->SetMatrix("g_mtRot", &mtRot);

		m_pShader->SetVector("g_vLightDir", &vLightDir);

		m_pShader->SetTexture("g_NormalTex", m_pNormalTex);

		UINT numPasses = 0;
		m_pShader->Begin(&numPasses, NULL);
		{
			for (UINT i = 0; i < numPasses; ++i)
			{
				m_pShader->BeginPass(i);
				{
					m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, m_iVertexNum - 2, m_pVertices, sizeof(Vertex));
				}
				m_pShader->EndPass();
			}
		}
		m_pShader->End();

		m_pdev->SetVertexShader(NULL);
		m_pdev->SetPixelShader(NULL);
		m_pdev->SetVertexDeclaration(NULL);
	}
}

void CShader_3dapi_03_30::Update()
{
	if (m_pdev)
	{
		D3DXMATRIX	mtTM;
		D3DXMATRIX	mtRotX;
		D3DXMATRIX	mtRotZ;

		static float fRot = 0.0f;
		fRot += 0.3f;
		FLOAT	fAngle = D3DXToRadian(fRot);
		D3DXMatrixRotationY(&mtTM, fAngle*3.f);
		D3DXMatrixRotationZ(&mtRotZ, fAngle*2.f);
		D3DXMatrixRotationX(&mtRotX, fAngle*1.f);

		mtTM *= mtRotZ;
		mtTM *= mtRotX;

		D3DXVec3TransformCoord(&m_vLightDir, &D3DXVECTOR3(0, -1, 0), &mtTM);
	}
}
