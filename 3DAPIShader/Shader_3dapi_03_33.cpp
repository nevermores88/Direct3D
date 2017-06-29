#include "stdafx.h"
#include "Shader_3dapi_03_33.h"


CShader_3dapi_03_33::CShader_3dapi_03_33()
{
}


CShader_3dapi_03_33::~CShader_3dapi_03_33()
{
}

HRESULT CShader_3dapi_03_33::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pShader = LoadShader("Ex03_33/Shader.fx");
	if (!m_pShader)
		return E_FAIL;

	D3DXCreateTextureFromFile(m_pdev, "Ex03_33/earth_d.bmp", &m_pDiffuseTex);
	D3DXCreateTextureFromFile(m_pdev, "Ex03_33/earth_l.bmp", &m_pLTex);
	D3DXCreateTextureFromFile(m_pdev, "Ex03_33/earth_s.bmp", &m_pSpecularTex);

	//이 예제에서는 사용안해도 되는듯
	/*DWORD dwFVF = Vertex::FVF;
	D3DVERTEXELEMENT9	vertex_decl[MAX_FVF_DECL_SIZE];
	D3DXDeclaratorFromFVF(dwFVF, vertex_decl);
	if (FAILED(m_pdev->CreateVertexDeclaration(vertex_decl, &m_pFVF)))
	return E_FAIL;*/

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

			D3DXVec3Normalize(&pVertices->n, &pVertices->n);

			pVertices->u = -((FLOAT)seg) / iNSphereSegments;
			pVertices->v = (ring + 0) / (FLOAT)iNSphereSegments;
			pVertices++;

			pVertices->p.x = x1;
			pVertices->p.y = y1;
			pVertices->p.z = z1;
			pVertices->n.x = x1;
			pVertices->n.y = y1;
			pVertices->n.z = z1;

			D3DXVec3Normalize(&pVertices->n, &pVertices->n);

			pVertices->u = -((FLOAT)seg) / iNSphereSegments;
			pVertices->v = (ring + 1) / (FLOAT)iNSphereSegments;
			pVertices++;
		}
	}

	return S_OK;
}

void CShader_3dapi_03_33::Release()
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

	if (m_pDiffuseTex)
	{
		m_pDiffuseTex->Release();
		m_pDiffuseTex = NULL;
	}

	if (m_pLTex)
	{
		m_pLTex->Release();
		m_pLTex = NULL;
	}

	if (m_pSpecularTex)
	{
		m_pSpecularTex->Release();
		m_pSpecularTex = NULL;
	}

	if (m_pVertices)
	{
		delete[] m_pVertices;
		m_pVertices = NULL;
	}
}

void CShader_3dapi_03_33::Render()
{
	if (m_pdev)
	{
		D3DXMATRIX mtTexScale;
		D3DXMatrixIdentity(&mtTexScale);

		mtTexScale._11 = 0.5f;
		mtTexScale._22 = -0.5f;
		mtTexScale._33 = 0.0f;
		mtTexScale._41 = 0.5f;
		mtTexScale._42 = 0.5f;
		mtTexScale._43 = 1.0f;
		mtTexScale._44 = 1.0f;

		m_pdev->SetFVF(Vertex::FVF);

		m_pShader->SetTexture("g_DiffuseTex", m_pDiffuseTex);
		m_pShader->SetTexture("g_LTex", m_pLTex);
		m_pShader->SetTexture("g_SpecularTex", m_pSpecularTex);

		UINT numPasses = 0;
		m_pShader->Begin(&numPasses, NULL);
		{
			for (UINT i = 0; i < numPasses; ++i)
			{
				m_pShader->BeginPass(i);
				{
					m_pdev->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL);
					m_pdev->SetTextureStageState(2, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR);

					m_pdev->SetTransform(D3DTS_TEXTURE0, &mtTexScale);
					m_pdev->SetTransform(D3DTS_TEXTURE1, &mtTexScale);
					m_pdev->SetTransform(D3DTS_TEXTURE2, &mtTexScale);

					m_pdev->SetTransform(D3DTS_WORLD, &m_mtWorld);

					m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, m_iVertexNum - 2, m_pVertices, sizeof(Vertex));
				}
				m_pShader->EndPass();
			}
		}
		m_pShader->End();
	}
}

void CShader_3dapi_03_33::Update()
{
	if (m_pdev)
	{
		D3DXMATRIX	mtRotY;
		D3DXMATRIX	mtRotZ;

		static float fRot = 0.0f;
		fRot += 0.3f;

		D3DXMatrixIdentity(&m_mtWorld);
		D3DXMatrixRotationY(&mtRotY, D3DXToRadian(fRot));
		D3DXMatrixRotationZ(&mtRotZ, D3DXToRadian(-23.5f));

		m_mtWorld = mtRotY * mtRotZ;
	}
}
