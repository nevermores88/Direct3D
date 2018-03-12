#include "stdafx.h"
#include "Shader_3dapi_01_28.h"


CShader_3dapi_01_28::CShader_3dapi_01_28()
{
}


CShader_3dapi_01_28::~CShader_3dapi_01_28()
{
}

HRESULT CShader_3dapi_01_28::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	HRESULT hr = 0;
	HWND hwnd;
	D3DDEVICE_CREATION_PARAMETERS ppm;

	m_pdev->GetCreationParameters(&ppm);
	hwnd = ppm.hFocusWindow;

	DWORD dwFlags = 0;

#if defined(_DEBUG) || defined(DEBUG)
	dwFlags |= D3DXSHADER_DEBUG;
#endif

	LPD3DXBUFFER pShader = NULL;
	LPD3DXBUFFER pError = NULL;

	hr = D3DXAssembleShaderFromFile("Ex01_28/Shader1.vsh", NULL, NULL, dwFlags, &pShader, &pError);

	if (FAILED(hr))
	{
		int iSize = pError->GetBufferSize();
		void* ack = pError->GetBufferPointer();

		if (ack)
		{
			char* str = new char[iSize];
			sprintf(str, (const char*)ack, iSize);
			OutputDebugString(str);
			delete[] str;
		}
	}

	if (pShader)
	{
		hr = m_pdev->CreateVertexShader((DWORD*)pShader->GetBufferPointer(), &m_pVertexShaderForModel);
		pShader->Release();
	}

	if (FAILED(hr))
		return E_FAIL;

	pShader = NULL;
	pError = NULL;

	hr = D3DXAssembleShaderFromFile("Ex01_28/Shader2.vsh", NULL, NULL, dwFlags, &pShader, &pError);

	if (FAILED(hr))
	{
		int iSize = pError->GetBufferSize();
		void* ack = pError->GetBufferPointer();

		if (ack)
		{
			char* str = new char[iSize];
			sprintf(str, (const char*)ack, iSize);
			OutputDebugString(str);
			delete[] str;
		}
	}

	if (pShader)
	{
		hr = m_pdev->CreateVertexShader((DWORD*)pShader->GetBufferPointer(), &m_pVertexShaderForEdge);
		pShader->Release();
	}

	if (FAILED(hr))
		return E_FAIL;

	D3DVERTEXELEMENT9 vertex_decl[MAX_FVF_DECL_SIZE] = { 0 };
	D3DXDeclaratorFromFVF(Vertex::FVF, vertex_decl);
	if (FAILED(m_pdev->CreateVertexDeclaration(vertex_decl, &m_pFVF)))
		return E_FAIL;

	LPD3DXMESH pMeshO = NULL;
	LPD3DXMESH pMeshT = NULL;

	D3DXCreateTorus(m_pdev, 20, 40, 60, 80, &pMeshO, NULL);
	pMeshO->CloneMeshFVF(D3DXMESH_MANAGED, Vertex::FVF, m_pdev, &pMeshT);
	pMeshO->Release();

	pMeshT->GetVertexBuffer(&m_pVB);
	pMeshT->GetIndexBuffer(&m_pIB);

	m_iFaceNum = pMeshT->GetNumFaces();
	m_iVertexNum = pMeshT->GetNumVertices();
	pMeshT->Release();

	D3DXCreateTexture(m_pdev, 512, 1, 0, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &m_pTex);

	D3DLOCKED_RECT pRect;
	m_pTex->LockRect(0, &pRect, NULL, 0);

	DWORD* pColor = (DWORD*)pRect.pBits;

	for (int i = 0; i < 512; i++)
	{
		float c = 0.0f;

		if (i < 10) c = 0;
		else if (i < 100) c = 0.2f;
		else if (i < 200) c = 0.4f;
		else if (i < 300) c = 0.6f;
		else if (i < 400) c = 0.8f;
		else c = 1.0f;

		pColor[i] = D3DXCOLOR(c, c, c, 1);
	}

	m_pTex->UnlockRect(0);

	return S_OK;
}

void CShader_3dapi_01_28::Release()
{
	if (m_pVertexShaderForModel)
	{
		m_pVertexShaderForModel->Release();
		m_pVertexShaderForModel = NULL;
	}

	if (m_pVertexShaderForEdge)
	{
		m_pVertexShaderForEdge->Release();
		m_pVertexShaderForEdge = NULL;
	}

	if (m_pFVF)
	{
		m_pFVF->Release();
		m_pFVF = NULL;
	}

	if (m_pTex)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}

	if (m_pVB)
	{
		m_pVB->Release();
		m_pVB = NULL;
	}

	if (m_pIB)
	{
		m_pIB->Release();
		m_pIB = NULL;
	}
}

void CShader_3dapi_01_28::Render()
{
	if (m_pdev)
	{
		D3DXMATRIX mtWorld;
		D3DXMATRIX mtView;
		D3DXMATRIX mtProj;

		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);

		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pdev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

		m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);

		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		m_pdev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

		D3DXMATRIX mtWVP = m_mtWorld*mtView*mtProj;
		D3DXMatrixTranspose(&mtWVP, &mtWVP);

		D3DXMatrixTranspose(&mtWorld, &m_mtWorld);

		///////////////////////////////////////////////////////////////////////////////////////////
		//Toon Shading Process

		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		D3DXCOLOR color(1.0f, 0.3f, 1.0f, 0.0f);
		D3DXVECTOR4 vLight(-1.0f, -1.0f, 1.0f, 0.0f);
		D3DXVec4Normalize(&vLight, &vLight);

		m_pdev->SetVertexShader(m_pVertexShaderForModel);
		m_pdev->SetVertexDeclaration(m_pFVF);

		m_pdev->SetVertexShaderConstantF(0, (FLOAT*)&mtWVP, 4);
		m_pdev->SetVertexShaderConstantF(4, (FLOAT*)&mtWorld, 4);
		m_pdev->SetVertexShaderConstantF(8, (FLOAT*)&vLight, 1);
		m_pdev->SetVertexShaderConstantF(9, (FLOAT*)&color, 1);

		m_pdev->SetTexture(0, m_pTex);
		m_pdev->SetStreamSource(0, m_pVB, 0, sizeof(Vertex));
		m_pdev->SetIndices(m_pIB);
		m_pdev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexNum, 0, m_iFaceNum);


		//Edge¶û Toon Shading ¼ø¼­¸¦ ¹Ù²Ù¸é Á¦´ë·Î ·»´õ¸µµÊ
		///////////////////////////////////////////////////////////////////////////////////////////
		//Edge Shading Process

		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		m_pdev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		D3DXCOLOR color1 = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		D3DXVECTOR4 vScale(2.0f, 2.0f, 2.0f, 0.0f);

		m_pdev->SetVertexShader(m_pVertexShaderForEdge);
		m_pdev->SetVertexDeclaration(m_pFVF);

		m_pdev->SetVertexShaderConstantF(0, (FLOAT*)&mtWVP, 4);
		m_pdev->SetVertexShaderConstantF(9, (FLOAT*)&color1, 1);
		m_pdev->SetVertexShaderConstantF(27, (FLOAT*)&vScale, 1);

		m_pdev->SetTexture(0, NULL);
		m_pdev->SetStreamSource(0, m_pVB, 0, sizeof(Vertex));
		m_pdev->SetIndices(m_pIB);
		m_pdev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexNum, 0, m_iFaceNum);

		m_pdev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_pdev->SetVertexDeclaration(NULL);
		m_pdev->SetVertexShader(NULL);
	}
}

void CShader_3dapi_01_28::Update()
{
	if (m_pdev)
	{
		static float c = 0.0f;
		c += 0.1f;

		if (c > 360.0f)
			c -= 360.0f;

		D3DXMATRIX	mtY;
		D3DXMATRIX mtZ;

		D3DXMatrixIdentity(&m_mtWorld);
		D3DXMatrixRotationY(&mtY, D3DXToRadian(-c));
		D3DXMatrixRotationZ(&mtZ, D3DXToRadian(-23.5f));

		m_mtWorld = mtY * mtZ;
	}
}
