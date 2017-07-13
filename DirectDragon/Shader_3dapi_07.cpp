#include "stdafx.h"
#include "Shader_3dapi_07.h"


CShader_3dapi_07::CShader_3dapi_07()
{
}


CShader_3dapi_07::~CShader_3dapi_07()
{
}

void CShader_3dapi_07::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;

	HRESULT hr = 0;

	LPD3DXBUFFER	pShader = NULL;
	LPD3DXBUFFER	pError = NULL;

	hr = D3DXCompileShaderFromFile(
		L"Shader_3dapi_07/shader.fx",
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

	m_Vertex[0] = Vertex(-50, 0, 0, D3DXCOLOR(1, 0, 0, 1));
	m_Vertex[1] = Vertex(50, 0, 0, D3DXCOLOR(0, 1, 0, 1));
	m_Vertex[2] = Vertex(50, 80, 0, D3DXCOLOR(0, 0, 1, 1));
	m_Vertex[3] = Vertex(-50, 80, 0, D3DXCOLOR(1, 0, 1, 1));

	hr = m_pdev->CreateVertexShader(
		(DWORD*)pShader->GetBufferPointer(),
		&m_pShader);

	pShader->Release();

	D3DVERTEXELEMENT9	vertexDecl[MAX_FVF_DECL_SIZE] = { 0 };
	D3DXDeclaratorFromFVF(Vertex::FVF, vertexDecl);

	if (FAILED(m_pdev->CreateVertexDeclaration(vertexDecl, &m_pFVF)))
		return;

	//m_hdWroldMatrix = m_pConstTable->GetConstantByName(NULL, "WorldMatrix");
	m_hdViewProjMatrix = m_pConstTable->GetConstantByName(NULL, "WorldViewProjMatrix");
}

void CShader_3dapi_07::Release()
{

}

void CShader_3dapi_07::OnRender()
{
	if (m_pdev)
	{
		D3DXMATRIX mtWorld;
		D3DXMATRIX mtView;
		D3DXMATRIX mtProj;
		D3DXMATRIX mtWorldViewProj;

		float fAngle = D3DXToRadian(GetTickCount()*0.1f);
		D3DXMatrixRotationY(&mtWorld, fAngle);
		//mtWorld._44 -= 1.f;

		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);

		mtWorldViewProj = mtWorld * mtView * mtProj;

		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		m_pdev->SetVertexShader(m_pShader);
		m_pdev->SetVertexDeclaration(m_pFVF);

		m_pConstTable->SetMatrix(m_pdev, m_hdViewProjMatrix, &mtWorldViewProj);

		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_Vertex, sizeof(Vertex));

		m_pdev->SetVertexShader(NULL);
		m_pdev->SetVertexDeclaration(m_pFVF);
	}
}

void CShader_3dapi_07::OnUpdate()
{
	if (m_pdev)
	{

	}
}
