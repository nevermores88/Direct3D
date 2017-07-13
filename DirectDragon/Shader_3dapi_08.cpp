#include "stdafx.h"
#include "Shader_3dapi_08.h"


CShader_3dapi_08::CShader_3dapi_08()
{
}


CShader_3dapi_08::~CShader_3dapi_08()
{
}

void CShader_3dapi_08::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;

	FLOAT	fSize = 30;

	// front 
	m_Vertex[0] = Vertex(-1.f, -1.f, -1.f, D3DXCOLOR(1.0F, 0.0F, 0.0F, 1.0F));
	m_Vertex[1] = Vertex(-1.f, 1.f, -1.f, D3DXCOLOR(0.0F, 1.0F, 0.0F, 1.0F));
	m_Vertex[2] = Vertex(1.f, 1.f, -1.f, D3DXCOLOR(0.0F, 0.0F, 1.0F, 1.0F));
	m_Vertex[3] = Vertex(1.f, -1.f, -1.f, D3DXCOLOR(1.0F, 0.0F, 1.0F, 1.0F));

	// back
	m_Vertex[4] = Vertex(-1.f, -1.f, 1.f, D3DXCOLOR(1.0F, 1.0F, 0.0F, 1.0F));
	m_Vertex[5] = Vertex(1.f, -1.f, 1.f, D3DXCOLOR(0.0F, 1.0F, 1.0F, 1.0F));
	m_Vertex[6] = Vertex(1.f, 1.f, 1.f, D3DXCOLOR(1.0F, 0.0F, 0.0F, 1.0F));
	m_Vertex[7] = Vertex(-1.f, 1.f, 1.f, D3DXCOLOR(1.0F, 0.0F, 1.0F, 1.0F));

	// top
	m_Vertex[8] = Vertex(-1.f, 1.f, -1.f, D3DXCOLOR(0.0F, 0.0F, 1.0F, 1.0F));
	m_Vertex[9] = Vertex(-1.f, 1.f, 1.f, D3DXCOLOR(0.0F, 1.0F, 0.0F, 1.0F));
	m_Vertex[10] = Vertex(1.f, 1.f, 1.f, D3DXCOLOR(1.0F, 0.0F, 1.0F, 1.0F));
	m_Vertex[11] = Vertex(1.f, 1.f, -1.f, D3DXCOLOR(0.0F, 1.0F, 1.0F, 1.0F));

	// bottom
	m_Vertex[12] = Vertex(-1.f, -1.f, -1.f, D3DXCOLOR(1.0F, 1.0F, 0.0F, 1.0F));
	m_Vertex[13] = Vertex(1.f, -1.f, -1.f, D3DXCOLOR(0.0F, 1.0F, 0.0F, 1.0F));
	m_Vertex[14] = Vertex(1.f, -1.f, 1.f, D3DXCOLOR(0.0F, 0.0F, 1.0F, 1.0F));
	m_Vertex[15] = Vertex(-1.f, -1.f, 1.f, D3DXCOLOR(1.0F, 0.0F, 0.0F, 1.0F));

	// left
	m_Vertex[16] = Vertex(-1.f, -1.f, 1.f, D3DXCOLOR(1.0F, 1.0F, 0.0F, 1.0F));
	m_Vertex[17] = Vertex(-1.f, 1.f, 1.f, D3DXCOLOR(1.0F, 0.0F, 1.0F, 1.0F));
	m_Vertex[18] = Vertex(-1.f, 1.f, -1.f, D3DXCOLOR(0.0F, 1.0F, 1.0F, 1.0F));
	m_Vertex[19] = Vertex(-1.f, -1.f, -1.f, D3DXCOLOR(1.0F, 0.0F, 1.0F, 1.0F));

	// right
	m_Vertex[20] = Vertex(1.f, -1.f, -1.f, D3DXCOLOR(1.0F, 1.0F, 0.0F, 1.0F));
	m_Vertex[21] = Vertex(1.f, 1.f, -1.f, D3DXCOLOR(0.0F, 1.0F, 0.0F, 1.0F));
	m_Vertex[22] = Vertex(1.f, 1.f, 1.f, D3DXCOLOR(1.0F, 0.0F, 1.0F, 1.0F));
	m_Vertex[23] = Vertex(1.f, -1.f, 1.f, D3DXCOLOR(0.0F, 0.0F, 1.0F, 1.0F));

	for (int i = 0; i < 24; ++i)
	{
		m_Vertex[i].x *= fSize;
		m_Vertex[i].y *= fSize;
		m_Vertex[i].z *= fSize;
	}

	// front
	m_Indices[0] = 0, m_Indices[1] = 1, m_Indices[2] = 2;
	m_Indices[3] = 0, m_Indices[4] = 2, m_Indices[5] = 3;

	// back
	m_Indices[6] = 4, m_Indices[7] = 5, m_Indices[8] = 6;
	m_Indices[9] = 4, m_Indices[10] = 6, m_Indices[11] = 7;

	// top
	m_Indices[12] = 8, m_Indices[13] = 9, m_Indices[14] = 10;
	m_Indices[15] = 8, m_Indices[16] = 10, m_Indices[17] = 11;

	// bottom
	m_Indices[18] = 12, m_Indices[19] = 13, m_Indices[20] = 14;
	m_Indices[21] = 12, m_Indices[22] = 14, m_Indices[23] = 15;

	// left
	m_Indices[24] = 16, m_Indices[25] = 17, m_Indices[26] = 18;
	m_Indices[27] = 16, m_Indices[28] = 18, m_Indices[29] = 19;

	// right
	m_Indices[30] = 20, m_Indices[31] = 21, m_Indices[32] = 22;
	m_Indices[33] = 20, m_Indices[34] = 22, m_Indices[35] = 23;

	HRESULT hr = 0;

	LPD3DXBUFFER	pShader = NULL;
	LPD3DXBUFFER	pError = NULL;

	hr = D3DXCompileShaderFromFile(
	L"Shader_3dapi_08/shader.fx",
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

	m_hdWorldMatrix = m_pConstTable->GetConstantByName(0, "WorldMatrix");
	m_hdViewMatrix = m_pConstTable->GetConstantByName(0, "ViewMatrix");
	m_hdProjMatrix = m_pConstTable->GetConstantByName(0, "ProjMatrix");

	m_pConstTable->SetDefaults(m_pdev);
}

void CShader_3dapi_08::Release()
{

}

void CShader_3dapi_08::OnRender()
{
	if (m_pdev)
	{
		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pdev->SetVertexShader(m_pShader);
		m_pdev->SetVertexDeclaration(m_pFVF);

		/*	m_pConstTable->SetMatrix(m_pdev, "m_mtWld", &m_mtWorld);
			m_pConstTable->SetMatrix(m_pdev, "m_mtViw", &m_mtView);
			m_pConstTable->SetMatrix(m_pdev, "m_mtPrj", &m_mtProj);*/

		m_pConstTable->SetMatrix(m_pdev, m_hdWorldMatrix, &m_mtWorld);
		m_pConstTable->SetMatrix(m_pdev, m_hdViewMatrix, &m_mtView);
		m_pConstTable->SetMatrix(m_pdev, m_hdProjMatrix, &m_mtProj);

		m_pdev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 24, 12, m_Indices, D3DFMT_INDEX16, m_Vertex, sizeof(Vertex));

		m_pdev->SetVertexShader(NULL);
	}
}

void CShader_3dapi_08::OnUpdate()
{
	if (m_pdev)
	{
		D3DXMATRIX	mtRotX;			// Rotation Matrix X
		D3DXMATRIX	mtRotY;			// Rotation Matrix Y
		D3DXMATRIX	mtRotZ;			// Rotation Matrix Z

		// 회전하는 월드 행렬 구성
		FLOAT		fAngle = D3DXToRadian(GetTickCount() * 0.1f);
		D3DXMatrixRotationY(&mtRotY, fAngle*3.f);
		D3DXMatrixRotationZ(&mtRotZ, fAngle*2.f);
		D3DXMatrixRotationX(&mtRotX, fAngle*1.f);

		m_mtWorld = mtRotY * mtRotZ * mtRotX;
		m_mtWorld._42 = 40.f;
		m_mtWorld._43 = -30.f;

		 m_pdev->GetTransform(D3DTS_VIEW, &m_mtView);
		 m_pdev->GetTransform(D3DTS_PROJECTION, &m_mtProj);
	}
}
