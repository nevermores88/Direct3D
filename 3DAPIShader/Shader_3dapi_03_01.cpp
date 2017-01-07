#include "stdafx.h"
#include "Shader_3dapi_03_01.h"


CShader_3dapi_03_01::CShader_3dapi_03_01()
{
}


CShader_3dapi_03_01::~CShader_3dapi_03_01()
{
}

HRESULT CShader_3dapi_03_01::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	m_pShader = NULL;
	m_pTex = NULL;

	DWORD	dFVF = Vertex::FVF;
	D3DVERTEXELEMENT9 vertex_decl[MAX_FVF_DECL_SIZE] = { 0 };
	D3DXDeclaratorFromFVF(dFVF, vertex_decl);
	if (FAILED(m_pdev->CreateVertexDeclaration(vertex_decl, &m_pFVF)))
		return E_FAIL;

	m_pShader = LoadShader("Ex03_10/tmp.fx");
	if (!m_pShader)
		return E_FAIL;

	m_pTex = LoadTexture("Ex03_01/logo.bmp");
	if (!m_pTex)
		return E_FAIL;

	float fSize = 30.0f;

	m_pVertices[0] = Vertex(-1.f, -1.f, -1.f, 0XFFFF0000, 0.f, 1.f);
	m_pVertices[1] = Vertex(-1.f, 1.f, -1.f, 0XFF00FF00, 0.f, 0.f);
	m_pVertices[2] = Vertex(1.f, 1.f, -1.f, 0XFF0000FF, 1.f, 0.f);
	m_pVertices[3] = Vertex(1.f, -1.f, -1.f, 0XFFFF00FF, 1.f, 1.f);

	// the back face vertex data
	m_pVertices[4] = Vertex(-1.f, -1.f, 1.f, 0XFFFFFF00, 0.f, 1.f);
	m_pVertices[5] = Vertex(1.f, -1.f, 1.f, 0XFF00FFFF, 0.f, 0.f);
	m_pVertices[6] = Vertex(1.f, 1.f, 1.f, 0XFFFF00FF, 1.f, 0.f);
	m_pVertices[7] = Vertex(-1.f, 1.f, 1.f, 0XFFFFFFFF, 1.f, 1.f);

	// the top face vertex data
	m_pVertices[8] = Vertex(-1.f, 1.f, -1.f, 0XFFF0FFF0, 0.f, 1.f);
	m_pVertices[9] = Vertex(-1.f, 1.f, 1.f, 0XFF0F0FFF, 0.f, 0.f);
	m_pVertices[10] = Vertex(1.f, 1.f, 1.f, 0XFFFFF00F, 1.f, 0.f);
	m_pVertices[11] = Vertex(1.f, 1.f, -1.f, 0XFFFFFFFF, 1.f, 1.f);

	// the bottom face vertex data
	m_pVertices[12] = Vertex(-1.f, -1.f, -1.f, 0XFFFF0FF0, 0.f, 1.f);
	m_pVertices[13] = Vertex(1.f, -1.f, -1.f, 0XFFF0F0FF, 0.f, 0.f);
	m_pVertices[14] = Vertex(1.f, -1.f, 1.f, 0XFF0FFF0F, 1.f, 0.f);
	m_pVertices[15] = Vertex(-1.f, -1.f, 1.f, 0XFFFFFFFF, 1.f, 1.f);

	// the left face vertex data
	m_pVertices[16] = Vertex(-1.f, -1.f, 1.f, 0XFFF0FFF0, 0.f, 1.f);
	m_pVertices[17] = Vertex(-1.f, 1.f, 1.f, 0XFF0FF0FF, 0.f, 0.f);
	m_pVertices[18] = Vertex(-1.f, 1.f, -1.f, 0XFFFF0F0F, 1.f, 0.f);
	m_pVertices[19] = Vertex(-1.f, -1.f, -1.f, 0XFFFFFFFF, 1.f, 1.f);

	// the right face vertex data
	m_pVertices[20] = Vertex(1.f, -1.f, -1.f, 0XFFFFF00F, 0.f, 1.f);
	m_pVertices[21] = Vertex(1.f, 1.f, -1.f, 0XFF0FFFF0, 0.f, 0.f);
	m_pVertices[22] = Vertex(1.f, 1.f, 1.f, 0XFFF00FFF, 1.f, 0.f);
	m_pVertices[23] = Vertex(1.f, -1.f, 1.f, 0XFFFFFFFF, 1.f, 1.f);

	/*for (int i = 0; i < 24; ++i)
	{
	m_pVertices[i].x *= fSize;
	m_pVertices[i].y *= fSize;
	m_pVertices[i].z *= fSize;
	}
	*/
	// the front face index data
	m_pIndices[0] = Index(0, 1, 2);
	m_pIndices[1] = Index(0, 2, 3);

	// the back face index data
	m_pIndices[2] = Index(4, 5, 6);
	m_pIndices[3] = Index(4, 6, 7);

	// the top face index data
	m_pIndices[4] = Index(8, 9, 10);
	m_pIndices[5] = Index(8, 10, 11);

	// the bottom face index data
	m_pIndices[6] = Index(12, 13, 14);
	m_pIndices[7] = Index(12, 14, 15);

	// the left face index data
	m_pIndices[8] = Index(16, 17, 18);
	m_pIndices[9] = Index(16, 18, 19);

	// the right face index data
	m_pIndices[10] = Index(20, 21, 22);
	m_pIndices[11] = Index(20, 22, 23);

	return S_OK;
}

void CShader_3dapi_03_01::Release()
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
}

void CShader_3dapi_03_01::Render()
{
	if (m_pdev)
	{
		D3DXMATRIX mtWorldViewProj = m_mtWorld * (g_pGame->m_matView)*(g_pGame->m_matProj);
		m_pShader->SetMatrix("gWorldViewProjection", &mtWorldViewProj);
		m_pShader->SetTexture("gTexDiffuse", m_pTex);

		m_pdev->SetVertexDeclaration(m_pFVF);

		m_pShader->SetTechnique("Tech0");

		UINT numPasses = 0;

		m_pShader->Begin(&numPasses, NULL);

		for (UINT i = 0; i < numPasses; ++i)
		{
			m_pShader->BeginPass(i);
			{
				m_pdev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 24, 12, m_pIndices, D3DFMT_INDEX16, m_pVertices, sizeof(Vertex));
			}
			m_pShader->EndPass();
		}
		m_pShader->End();
	}

	m_pdev->SetVertexDeclaration(NULL);
	m_pdev->SetVertexShader(NULL);
	m_pdev->SetPixelShader(NULL);
}

void CShader_3dapi_03_01::Update()
{
	if (m_pdev)
	{
		D3DXMATRIX	mtRotX;			// Rotation Matrix X
		D3DXMATRIX	mtRotY;			// Rotation Matrix Y
		D3DXMATRIX	mtRotZ;			// Rotation Matrix Z


		// Setup Rotating World Matrix
		static float f = 0.0f;
		f += 15.0f;
		FLOAT		fAngle = D3DXToRadian(f/*GetTickCount()*/* 0.03f);
		D3DXMatrixRotationY(&mtRotY, fAngle*3.f);
		D3DXMatrixRotationZ(&mtRotZ, fAngle*2.f);
		D3DXMatrixRotationX(&mtRotX, fAngle*1.f);

		m_mtWorld = mtRotY * mtRotZ * mtRotX;
		//m_mtWorld._42 = 40.f;
		//m_mtWorld._43 = -30.f;
	}
}
