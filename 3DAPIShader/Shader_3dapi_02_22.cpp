#include "stdafx.h"
#include "Shader_3dapi_02_22.h"


CShader_3dapi_02_22::CShader_3dapi_02_22()
{
}


CShader_3dapi_02_22::~CShader_3dapi_02_22()
{
}

HRESULT CShader_3dapi_02_22::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pShader = LoadPixelShader("Ex02_22/Shader.fx", &m_pConstTbl);
	if (!m_pShader)
		return E_FAIL;

	D3DXCreateTextureFromFile(m_pdev, "Ex02_22/earth.bmp", &m_pTex);

	// 버텍스 생성
	INT	iNSphereSegments = 128;
	m_iVertexNum = 2 * iNSphereSegments*(iNSphereSegments + 1);

	FLOAT fDeltaRingAngle = (D3DX_PI / iNSphereSegments);
	FLOAT fDeltaSegAngle = (2.0f * D3DX_PI / iNSphereSegments);

	m_pVerticies = new Vertex[m_iVertexNum];
	Vertex* pVerticies = m_pVerticies;

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
			pVerticies->x = x0;
			pVerticies->y = y0;
			pVerticies->z = z0;
			pVerticies->u = -((FLOAT)seg) / iNSphereSegments;
			pVerticies->v = (ring + 0) / (FLOAT)iNSphereSegments;
			pVerticies++;

			pVerticies->x = x1;
			pVerticies->y = y1;
			pVerticies->z = z1;
			pVerticies->u = -((FLOAT)seg) / iNSphereSegments;
			pVerticies->v = (ring + 1) / (FLOAT)iNSphereSegments;
			pVerticies++;
		}
	}

	return S_OK;
}

void CShader_3dapi_02_22::Release()
{
	if (m_pShader)
	{
		m_pShader->Release();
		m_pShader = NULL;
	}

	if (m_pConstTbl)
	{
		m_pConstTbl->Release();
		m_pConstTbl = NULL;
	}

	if (m_pTex)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}

	if (m_pVerticies)
	{
		delete[] m_pVerticies;
		m_pVerticies = NULL;
	}
}

void CShader_3dapi_02_22::Render()
{
	if (m_pdev)
	{
		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		m_pdev->SetPixelShader(m_pShader);

		m_pdev->SetFVF(Vertex::FVF);
		m_pdev->SetTexture(0, m_pTex);
		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, m_iVertexNum - 2, m_pVerticies, sizeof(Vertex));
	}
}

void CShader_3dapi_02_22::Update()
{
	if (m_pdev)
	{

	}
}
