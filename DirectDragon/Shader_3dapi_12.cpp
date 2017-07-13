#include "stdafx.h"
#include "Shader_3dapi_12.h"


CShader_3dapi_12::CShader_3dapi_12()
{
}


CShader_3dapi_12::~CShader_3dapi_12()
{
}

void CShader_3dapi_12::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;

	HRESULT hr = 0;

	LPD3DXBUFFER	pShader = NULL;
	LPD3DXBUFFER	pError = NULL;

	hr = D3DXCompileShaderFromFile(
		L"Shader_3dapi_12/shader.fx",
		0, 0,
		"Main",
		"vs_1_1",
		D3DXSHADER_DEBUG,
		&pShader,
		&pError,
		&m_pConstTable);

	if (pError)
	{
		wchar_t errorMsg[1024];
		MultiByteToWideChar(CP_ACP, MB_COMPOSITE, (LPCCH)pError->GetBufferPointer(), -1, errorMsg, 1024);
		::MessageBox(0, errorMsg, 0, 0);
	}

	if (FAILED(hr))
	{
		::MessageBox(0, L"D3DXCompileShaderFromFile() - FAILED", 0, 0);
		return;
	}

	hr = m_pdev->CreateVertexShader(
		(DWORD*)pShader->GetBufferPointer(),
		&m_pShader);

	pShader->Release();

	D3DVERTEXELEMENT9	vertexDecl[MAX_FVF_DECL_SIZE] = { 0 };
	D3DXDeclaratorFromFVF(Vertex::FVF, vertexDecl);

	if (FAILED(m_pdev->CreateVertexDeclaration(vertexDecl, &m_pFVF)))
		return;

	m_hdWorld = m_pConstTable->GetConstantByName(NULL, "WorldMatrix");
	m_hdView = m_pConstTable->GetConstantByName(NULL, "ViewMatrix");
	m_hdProj = m_pConstTable->GetConstantByName(NULL, "ProjMatrix");
	m_hdFogColor = m_pConstTable->GetConstantByName(NULL, "FogColor");
	m_hdFogEnd = m_pConstTable->GetConstantByName(NULL, "FogEnd");
	m_hdFogBegin = m_pConstTable->GetConstantByName(NULL, "FogBegin");

	// 버텍스 생성
	INT	iNSphereSegments = 128;
	m_iNvx = 2 * iNSphereSegments*(iNSphereSegments + 1);

	FLOAT fDeltaRingAngle = (D3DX_PI / iNSphereSegments);
	FLOAT fDeltaSegAngle = (2.0f * D3DX_PI / iNSphereSegments);

	m_Vertex = new Vertex[m_iNvx];
	Vertex* pVtx = m_Vertex;

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

			pVtx->x = x1;
			pVtx->y = y1;
			pVtx->z = z1;
			pVtx++;
		}
	}
}

void CShader_3dapi_12::Release()
{

}

void CShader_3dapi_12::OnRender()
{
	if (m_pdev)
	{
		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);

		D3DXCOLOR	FogColor(1.0F, 1.0F, 0.2F, 1);	// Fog Color
		FLOAT		fFogEnd = 120.f;	// Fog End
		FLOAT		fFogBgn = 0.f;		// Fog Begin

		m_pdev->GetTransform(D3DTS_VIEW, &m_mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &m_mtProj);
		//D3DXMATRIX mtWorldViewProj = m_mtWorld * m_mtView * m_mtProj;

		m_pdev->SetVertexShader(m_pShader);
		m_pdev->SetVertexDeclaration(m_pFVF);

		m_pConstTable->SetMatrix(m_pdev, m_hdWorld, &m_mtWorld);
		m_pConstTable->SetMatrix(m_pdev, m_hdView, &m_mtView);
		m_pConstTable->SetMatrix(m_pdev, m_hdProj, &m_mtProj);
		m_pConstTable->SetVector(m_pdev, m_hdFogColor, (D3DXVECTOR4*)&FogColor);
		m_pConstTable->SetFloat(m_pdev, m_hdFogEnd, fFogEnd);
		m_pConstTable->SetFloat(m_pdev, m_hdFogBegin, fFogBgn);

		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, m_iNvx - 2, m_Vertex, sizeof(Vertex));

		m_pdev->SetVertexShader(NULL);
		m_pdev->SetVertexDeclaration(NULL);
	}
}

void CShader_3dapi_12::OnUpdate()
{
	if (m_pdev)
	{
		D3DXMATRIX	mtY;
		D3DXMATRIX	mtZ;
		FLOAT	c = GetTickCount() * 0.05f;

		// 월드 행렬 갱신
		D3DXMatrixIdentity(&m_mtWorld);
		D3DXMatrixRotationY(&mtY, D3DXToRadian(-c));
		D3DXMatrixRotationZ(&mtZ, D3DXToRadian(-23.5f));

		m_mtWorld = mtY * mtZ;
	}
}
