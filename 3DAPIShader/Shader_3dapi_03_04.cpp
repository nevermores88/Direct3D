#include "stdafx.h"
#include "Shader_3dapi_03_04.h"


CShader_3dapi_03_04::CShader_3dapi_03_04()
{
}


CShader_3dapi_03_04::~CShader_3dapi_03_04()
{
}

HRESULT CShader_3dapi_03_04::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pShader = NULL;
	m_pFVF = NULL;
	m_pTex = NULL;
	m_pVertex = NULL;

	m_pShader = LoadShader("Ex03_04/Shader.fx");
	if (!m_pShader)
		return E_FAIL;

	m_pTex = LoadTexture("Ex03_04/earth.bmp");
	if (!m_pTex)
		return E_FAIL;

	DWORD dwFVF = Vertex::FVF;
	D3DVERTEXELEMENT9 vertex_decl[MAX_FVF_DECL_SIZE];
	D3DXDeclaratorFromFVF(dwFVF, vertex_decl);
	if (FAILED(m_pdev->CreateVertexDeclaration(vertex_decl, &m_pFVF)))
		return E_FAIL;

	// Create Vertex
	INT	iNSphereSegments = 128;
	m_iNumOfVertex = 2 * iNSphereSegments*(iNSphereSegments + 1);

	FLOAT fDeltaRingAngle = (D3DX_PI / iNSphereSegments);
	FLOAT fDeltaSegAngle = (2.0f * D3DX_PI / iNSphereSegments);

	m_pVertex = new Vertex[m_iNumOfVertex];
	Vertex* pVtx = m_pVertex;

	// Generate the group of rings for the sphere
	for (DWORD ring = 0; ring < iNSphereSegments; ring++)
	{
		FLOAT r0 = 50 * sinf((ring + 0) * fDeltaRingAngle);
		FLOAT r1 = 50 * sinf((ring + 1) * fDeltaRingAngle);
		FLOAT y0 = 50 * cosf((ring + 0) * fDeltaRingAngle);
		FLOAT y1 = 50 * cosf((ring + 1) * fDeltaRingAngle);

		// Generate the group of segments for the current ring
		for (DWORD seg = 0; seg < (iNSphereSegments + 1); seg++)
		{
			FLOAT x0 = r0 * sinf(seg * fDeltaSegAngle);
			FLOAT z0 = r0 * cosf(seg * fDeltaSegAngle);
			FLOAT x1 = r1 * sinf(seg * fDeltaSegAngle);
			FLOAT z1 = r1 * cosf(seg * fDeltaSegAngle);

			// Add two vertices to the strip which makes up the sphere
			// (using the transformed normal to generate texture coords)
			pVtx->p.x = x0;
			pVtx->p.y = y0;
			pVtx->p.z = z0;
			pVtx->n.x = x0;
			pVtx->n.y = y0;
			pVtx->n.z = z0;

			D3DXVec3Normalize(&pVtx->n, &pVtx->n);

			pVtx->u = -((FLOAT)seg) / iNSphereSegments;
			pVtx->v = (ring + 0) / (FLOAT)iNSphereSegments;
			pVtx++;

			pVtx->p.x = x1;
			pVtx->p.y = y1;
			pVtx->p.z = z1;
			pVtx->n.x = x1;
			pVtx->n.y = y1;
			pVtx->n.z = z1;

			D3DXVec3Normalize(&pVtx->n, &pVtx->n);
			pVtx->u = -((FLOAT)seg) / iNSphereSegments;
			pVtx->v = (ring + 1) / (FLOAT)iNSphereSegments;
			pVtx++;
		}
	}

	return S_OK;
}

void CShader_3dapi_03_04::Release()
{
	if (m_pShader)
	{
		m_pShader->Release();
		m_pShader = NULL;
	}

	if (m_pTex)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}

	if (m_pVertex)
	{
		delete[] m_pVertex;
		m_pVertex = NULL;
	}
}

void CShader_3dapi_03_04::Render()
{
	if (m_pdev)
	{
		D3DXMATRIX mtView;
		D3DXMATRIX mtProj;
		D3DXMATRIX mtWorldViewProj;
		D3DXVECTOR4 vLightDir(-1, -1, 0.5f, 0);

		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);

		D3DXVec4Normalize(&vLightDir, &vLightDir);
		mtWorldViewProj = m_mtWorld * mtView * mtProj;

		m_pdev->SetVertexDeclaration(m_pFVF);

		m_pShader->SetMatrix("gWorldViewProj", &mtWorldViewProj);
		m_pShader->SetMatrix("gWorld", &m_mtWorld);
		m_pShader->SetVector("gLightDir", &vLightDir);
		m_pShader->SetTexture("gDiffuseTex", m_pTex);

		m_pShader->SetTechnique("Tech0");

		UINT numPasses = 0;

		m_pShader->Begin(&numPasses, NULL);

		for (UINT i = 0; i < numPasses; ++i)
		{
			m_pShader->BeginPass(i);
			{
				m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, m_iNumOfVertex - 2, m_pVertex, sizeof(Vertex));
			}
			m_pShader->EndPass();
		}
		m_pShader->End();
	}

	m_pdev->SetVertexShader(NULL);
	m_pdev->SetPixelShader(NULL);
	m_pdev->SetVertexDeclaration(NULL);
}

void CShader_3dapi_03_04::Update()
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
