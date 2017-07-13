#include "stdafx.h"
#include "Shader_3dapi_28.h"


CShader_3dapi_28::CShader_3dapi_28()
{
}


CShader_3dapi_28::~CShader_3dapi_28()
{
}

void CShader_3dapi_28::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;

	HRESULT hr = 0;

	LPD3DXBUFFER	pShader = NULL;
	LPD3DXBUFFER	pError = NULL;

	//���ؽ� Shader
	hr = D3DXCompileShaderFromFile(
		L"Shader_3dapi_28/shader.fx",
		0, 0,
		"VertexMain",
		"vs_1_1",
		D3DXSHADER_DEBUG,
		&pShader,
		&pError,
		&m_pVertexConstTable);

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
		&m_pVertexShader);

	pShader->Release();

	D3DVERTEXELEMENT9 vertex_decl[MAX_FVF_DECL_SIZE] = { 0 };
	D3DXDeclaratorFromFVF(Vertex::FVF, vertex_decl);
	m_pdev->CreateVertexDeclaration(vertex_decl, &m_pFVF);

	//�ȼ� Shader
	hr = D3DXCompileShaderFromFile(
		L"Shader_3dapi_28/shader.fx",
		0, 0,
		"PixelMain",
		"ps_2_0",
		D3DXSHADER_DEBUG,
		&pShader,
		&pError,
		&m_pPixelConstTable);

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

	hr = m_pdev->CreatePixelShader(
		(DWORD*)pShader->GetBufferPointer(),
		&m_pPixelShader);

	pShader->Release();

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
			pVtx->u = -((FLOAT)seg) / iNSphereSegments;
			pVtx->v = (ring + 0) / (FLOAT)iNSphereSegments;
			pVtx++;

			pVtx->x = x1;
			pVtx->y = y1;
			pVtx->z = z1;
			pVtx->u = -((FLOAT)seg) / iNSphereSegments;
			pVtx->v = (ring + 1) / (FLOAT)iNSphereSegments;
			pVtx++;
		}
	}

	D3DXCreateTextureFromFile(m_pdev, L"Shader_3dapi_28/earth.bmp", &m_pTex);
}

void CShader_3dapi_28::Release()
{

}

void CShader_3dapi_28::OnRender()
{
	D3DXMATRIX		mtViw;			// View Matrix
	D3DXMATRIX		mtPrj;			// Projection Matrix

	m_pdev->GetTransform(D3DTS_VIEW, &mtViw);
	m_pdev->GetTransform(D3DTS_PROJECTION, &mtPrj);

	m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);


	// Render
	m_pdev->SetVertexShader(m_pVertexShader);
	m_pdev->SetPixelShader(m_pPixelShader);

	m_pdev->SetVertexDeclaration(m_pFVF);


	// ���ؽ� ���꿡 ���� ��� ����
	m_pVertexConstTable->SetMatrix(m_pdev, "WorldViewProj", &(m_mtWorld * mtViw * mtPrj));


	D3DXCOLOR	color(1, 0.3F, 0.7F, 1);
	// �ȼ� ���꿡 ���� ��� ����
	m_pPixelConstTable->SetVector(m_pdev, "Diffuse", (D3DXVECTOR4*)&(color));


	m_pdev->SetTexture(0, m_pTex);
	m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, m_iNvx - 2, m_Vertex, sizeof(Vertex));

	m_pdev->SetVertexShader(NULL);
	m_pdev->SetPixelShader(NULL);
}

void CShader_3dapi_28::OnUpdate()
{
	if (m_pdev)
	{
		static float c = 0;

		c += 1.0f;

		if (c > 360.0f)
			c -= 360.f;

		D3DXMATRIX mtY;
		D3DXMATRIX mtZ;

		D3DXMatrixIdentity(&m_mtWorld);
		D3DXMatrixRotationY(&mtY, D3DXToRadian(-c));
		D3DXMatrixRotationZ(&mtZ, D3DXToRadian(-23.5f));

		m_mtWorld = mtY * mtZ;
	}
}
