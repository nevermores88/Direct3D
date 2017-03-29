#include "stdafx.h"
#include "Shader_3dapi_02_12.h"


CShader_3dapi_02_12::CShader_3dapi_02_12()
{
}


CShader_3dapi_02_12::~CShader_3dapi_02_12()
{
}

HRESULT CShader_3dapi_02_12::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pShader = LoadVertexShader("Ex02_12/Shader.fx", &m_pConstTbl);
	if (!m_pShader)
		return E_FAIL;

	/*DWORD dwFlags = 0;
#if defined( _DEBUG ) || defined( DEBUG )
	dwFlags |= D3DXSHADER_DEBUG;
	#endif

	HRESULT	hr = 0;

	LPD3DXBUFFER	pShader = NULL;
	LPD3DXBUFFER	pErr = NULL;

	hr = D3DXCompileShaderFromFile("Ex02_12/Shader.fx"
	, NULL
	, NULL
	, "VtxPrc"
	, "vs_1_1"
	, dwFlags
	, &pShader
	, &pErr
	, &m_pConstTbl
	);

	if (FAILED(hr))
	{
	char sErr[2048] = { 0 };
	if (pErr)
	{
	char* s = (char*)pErr->GetBufferPointer();
	sprintf(sErr, s);
	}
	else
	{
	sprintf(sErr, "Cannot Compile Shader.");
	}

	MessageBox(g_pGame->GetHWND(), sErr, "Err", MB_ICONEXCLAMATION);
	return -1;
	}

	hr = m_pdev->CreateVertexShader((DWORD*)pShader->GetBufferPointer(), &m_pShader);
	pShader->Release();
	if (FAILED(hr))
	return -1;*/

	D3DVERTEXELEMENT9 vertex_decl[MAX_FVF_DECL_SIZE] = { 0 };
	D3DXDeclaratorFromFVF(Vertex::FVF, vertex_decl);
	if (FAILED(m_pdev->CreateVertexDeclaration(vertex_decl, &m_pFVF)))
		return -1;

	// 버텍스 생성
	INT	iNSphereSegments = 128;
	m_iVerticesNum = 2 * iNSphereSegments*(iNSphereSegments + 1);

	FLOAT fDeltaRingAngle = (D3DX_PI / iNSphereSegments);
	FLOAT fDeltaSegAngle = (2.0f * D3DX_PI / iNSphereSegments);

	m_pVertices = new Vertex[m_iVerticesNum];
	Vertex* pVtx = m_pVertices;

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
			pVtx->x = x0;
			pVtx->y = y0;
			pVtx->z = z0;
			pVtx++;
			pVtx->color = 0xff0000ff;

			pVtx->x = x1;
			pVtx->y = y1;
			pVtx->z = z1;
			pVtx++;

			pVtx->color = 0xff0000ff;
		}
	}


	return S_OK;
}

void CShader_3dapi_02_12::Release()
{

}

void CShader_3dapi_02_12::Render()
{
	if (m_pdev)
	{
		HRESULT	hr = 0;

		//m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);

		D3DXMatrixIdentity(&m_mtWorld);

		D3DXMATRIX		mtView;			// View Matrix
		D3DXMATRIX		mtProj;			// Projection Matrix

		D3DXCOLOR	FogColor(1.0F, 1.0F, 0.2F, 1);	// Fog Color
		FLOAT		FogEnd = 1200.f;	// Fog End
		FLOAT		FogBgn = 0.f;		// Fog Begin

		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);

		// Render
		m_pdev->SetVertexShader(m_pShader);
		m_pdev->SetVertexDeclaration(m_pFVF);

		hr = m_pConstTbl->SetMatrix(m_pdev, "m_mtWorld", &m_mtWorld);
		hr = m_pConstTbl->SetMatrix(m_pdev, "m_mtView", &mtView);
		hr = m_pConstTbl->SetMatrix(m_pdev, "m_mtProj", &mtProj);
		hr = m_pConstTbl->SetVector(m_pdev, "m_FogColor", (D3DXVECTOR4*)&FogColor);
		hr = m_pConstTbl->SetFloat(m_pdev, "m_FogEnd", FogEnd);
		hr = m_pConstTbl->SetFloat(m_pdev, "m_FogBgn", FogBgn);

		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, m_iVerticesNum - 2, m_pVertices, sizeof(Vertex));

		m_pdev->SetVertexShader(NULL);
		m_pdev->SetVertexDeclaration(NULL);
	}
}

void CShader_3dapi_02_12::Update()
{

}
