#include "stdafx.h"
#include "Shader_3dapi_02_28.h"


CShader_3dapi_02_28::CShader_3dapi_02_28()
{
}


CShader_3dapi_02_28::~CShader_3dapi_02_28()
{
}

HRESULT CShader_3dapi_02_28::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pVertexShader = LoadVertexShader("Ex02_28/Shader.fx", &m_pVertexConstTbl);
	if (!m_pVertexShader)
		return E_FAIL;

	m_pPixelShader = LoadPixelShader("Ex02_28/Shader.fx", &m_pPixelConstTbl);
	if (!m_pPixelShader)
		return E_FAIL;

	D3DXCreateTextureFromFile(m_pdev, "Ex02_28/earth.bmp", &m_pTex);

	D3DVERTEXELEMENT9	vertex_decl[MAX_FVF_DECL_SIZE] = { 0 };
	D3DXDeclaratorFromFVF(Vertex::FVF, vertex_decl);
	if (FAILED(m_pdev->CreateVertexDeclaration(vertex_decl, &m_pVertexDecl)))
		return E_FAIL;

	// 버텍스 생성
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
			pVertices->x = x0;
			pVertices->y = y0;
			pVertices->z = z0;
			pVertices->u = -((FLOAT)seg) / iNSphereSegments;
			pVertices->v = (ring + 0) / (FLOAT)iNSphereSegments;
			pVertices++;

			pVertices->x = x1;
			pVertices->y = y1;
			pVertices->z = z1;
			pVertices->u = -((FLOAT)seg) / iNSphereSegments;
			pVertices->v = (ring + 1) / (FLOAT)iNSphereSegments;
			pVertices++;
		}
	}

	D3DXMatrixIdentity(&m_mtWorld);
}

void CShader_3dapi_02_28::Release()
{
	if (m_pVertexShader)
	{
		m_pVertexShader->Release();
		m_pVertexShader = NULL;
	}

	if (m_pVertexDecl)
	{
		m_pVertexDecl->Release();
		m_pVertexDecl = NULL;
	}

	if (m_pVertexConstTbl)
	{
		m_pVertexConstTbl->Release();
		m_pVertexConstTbl = NULL;
	}

	if (m_pPixelShader)
	{
		m_pPixelShader->Release();
		m_pPixelShader = NULL;
	}

	if (m_pPixelConstTbl)
	{
		m_pPixelConstTbl->Release();
		m_pPixelConstTbl = NULL;
	}

	if (m_pTex)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}

	if (m_pVertices)
	{
		delete[] m_pVertices;
		m_pVertices = NULL;
	}
}

void CShader_3dapi_02_28::Render()
{
	if (m_pdev)
	{
		D3DXMATRIX mtView;
		D3DXMATRIX mtProj;

		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);

		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		m_pdev->SetVertexShader(m_pVertexShader);
		m_pdev->SetPixelShader(m_pPixelShader);

		m_pdev->SetVertexDeclaration(m_pVertexDecl);

		m_pVertexConstTbl->SetMatrix(m_pdev, "g_WorldViewProj", &(m_mtWorld*mtView*mtProj));

		D3DXCOLOR color(1, 0.3f, 0.7f, 1);
		m_pPixelConstTbl->SetVector(m_pdev, "g_Diffuse", (D3DXVECTOR4*)&(color));

		m_pdev->SetTexture(0, m_pTex);
		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, m_iVertexNum - 2, m_pVertices, sizeof(Vertex));

		m_pdev->SetVertexShader(NULL);
		m_pdev->SetPixelShader(NULL);
		m_pdev->SetTexture(0, NULL);

	}
}

void CShader_3dapi_02_28::Update()
{
	if (m_pdev)
	{
		static float c = 0.0f;

		c += 0.2f;

		D3DXMATRIX mtY;
		D3DXMATRIX mtZ;

		D3DXMatrixIdentity(&m_mtWorld);
		D3DXMatrixRotationY(&mtY, D3DXToRadian(-c));
		D3DXMatrixRotationZ(&mtZ, D3DXToRadian(-23.5f));

		m_mtWorld = mtY * mtZ;
	}
}
