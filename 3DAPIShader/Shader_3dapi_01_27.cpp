#include "stdafx.h"
#include "Shader_3dapi_01_27.h"


CShader_3dapi_01_27::CShader_3dapi_01_27()
{
}


CShader_3dapi_01_27::~CShader_3dapi_01_27()
{
}

HRESULT CShader_3dapi_01_27::Create(LPDIRECT3DDEVICE9 pdev)
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

	hr = D3DXAssembleShaderFromFile("Ex01_27/Shader.vsh", NULL, NULL, dwFlags, &pShader, &pError);

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
		hr = m_pdev->CreateVertexShader((DWORD*)pShader->GetBufferPointer(), &m_pVertexShader);
		pShader->Release();
	}

	if (FAILED(hr))
		return E_FAIL;

	D3DVERTEXELEMENT9 vertex_decl[MAX_FVF_DECL_SIZE] = { 0 };
	D3DXDeclaratorFromFVF(Vertex::FVF, vertex_decl);
	if (FAILED(m_pdev->CreateVertexDeclaration(vertex_decl, &m_pFVF)))
		return E_FAIL;

	INT	iSphereSegmentsNum = 128;
	m_iVertexNum = 2 * iSphereSegmentsNum*(iSphereSegmentsNum + 1);

	FLOAT fDeltaRingAngle = (D3DX_PI / iSphereSegmentsNum);
	FLOAT fDeltaSegAngle = (2.0f * D3DX_PI / iSphereSegmentsNum);

	m_pVertices = new Vertex[m_iVertexNum];
	Vertex* pVertices = m_pVertices;

	// Generate the group of rings for the sphere
	for (DWORD ring = 0; ring < iSphereSegmentsNum; ring++)
	{
		FLOAT r0 = 50 * sinf((ring + 0) * fDeltaRingAngle);
		FLOAT r1 = 50 * sinf((ring + 1) * fDeltaRingAngle);
		FLOAT y0 = 50 * cosf((ring + 0) * fDeltaRingAngle);
		FLOAT y1 = 50 * cosf((ring + 1) * fDeltaRingAngle);

		// Generate the group of segments for the current ring
		for (DWORD seg = 0; seg < (iSphereSegmentsNum + 1); seg++)
		{
			FLOAT x0 = r0 * sinf(seg * fDeltaSegAngle);
			FLOAT z0 = r0 * cosf(seg * fDeltaSegAngle);
			FLOAT x1 = r1 * sinf(seg * fDeltaSegAngle);
			FLOAT z1 = r1 * cosf(seg * fDeltaSegAngle);

			// Add two vertices to the strip which makes up the sphere
			// (using the transformed normal to generate texture coords)
			pVertices->p.x = x0;
			pVertices->p.y = y0;
			pVertices->p.z = z0;
			pVertices->n = pVertices->p;
			D3DXVec3Normalize(&pVertices->n, &pVertices->n);
			pVertices->u = -((float)seg) / iSphereSegmentsNum;			//-부호라서 U 어드레스모드를 clamp로 하면 엣지부분 텍스쳐로만 변경됨. 그래서 이상하게 보임
			pVertices->v = (ring + 0) / (float)iSphereSegmentsNum;

			pVertices++;

			pVertices->p.x = x1;
			pVertices->p.y = y1;
			pVertices->p.z = z1;
			pVertices->n = pVertices->p;
			D3DXVec3Normalize(&pVertices->n, &pVertices->n);
			pVertices->u = -((float)seg) / iSphereSegmentsNum;
			pVertices->v = (ring + 1) / (float)iSphereSegmentsNum;

			pVertices++;
		}
	}


	D3DXCreateTextureFromFile(m_pdev, "Ex01_27/earth.bmp", &m_pDiffuseTex);
	D3DXCreateTexture(m_pdev, 512, 1, 0, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &m_pToonTex);

	D3DLOCKED_RECT pRect;
	m_pToonTex->LockRect(0, &pRect, NULL, 0);

	DWORD* pColor = (DWORD*)pRect.pBits;

	for (int i = 0; i < 512; i++)
	{
		float c = 0.0f;

		if (i < 10) c = 0;
		else if (i < 100) c = 0.2f;
		else if (i < 200) c = 0.4f;
		else if (i < 300) c = 0.6f;
		else if (i < 400) c = 0.8f;
		else if (i < 500) c = 0.9f;
		else c = 1.0f;

		pColor[i] = D3DXCOLOR(c, c, c, 1);
	}

	m_pToonTex->UnlockRect(0);

	return S_OK;
}

void CShader_3dapi_01_27::Release()
{
	if (m_pVertexShader)
	{
		m_pVertexShader->Release();
		m_pVertexShader = NULL;
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

	if (m_pToonTex)
	{
		m_pToonTex->Release();
		m_pToonTex = NULL;
	}

	if (m_pVertices)
	{
		delete[] m_pVertices;
		m_pVertices = NULL;
	}
}

void CShader_3dapi_01_27::Render()
{
	if (m_pdev)
	{
		D3DXMATRIX mtWorld;
		D3DXMATRIX mtView;
		D3DXMATRIX mtProj;

		m_pdev->GetTransform(D3DTS_VIEW, &mtView);
		m_pdev->GetTransform(D3DTS_PROJECTION, &mtProj);

		D3DXCOLOR color(1.0f, 0.3f, 1.0f, 0.0f);
		D3DXVECTOR4 vLight(-1.0f, -1.0f, 1.0f, 0.0f);
		D3DXVec4Normalize(&vLight, &vLight);
		//D3DXVec3TransformNormal((D3DXVECTOR3*)&vLight, (D3DXVECTOR3*)&vLight, &mtView);

		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pdev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

		m_pdev->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

		m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);

		m_pdev->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		m_pdev->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		m_pdev->SetSamplerState(1, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);

		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		m_pdev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

		m_pdev->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
		m_pdev->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
		m_pdev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADDSIGNED);

		D3DXMATRIX mtWVP = m_mtWorld*mtView*mtProj;
		D3DXMatrixTranspose(&mtWVP, &mtWVP);

		D3DXMatrixTranspose(&mtWorld, &m_mtWorld);

		m_pdev->SetVertexShader(m_pVertexShader);
		m_pdev->SetVertexDeclaration(m_pFVF);

		m_pdev->SetVertexShaderConstantF(0, (FLOAT*)&mtWVP, 4);
		m_pdev->SetVertexShaderConstantF(4, (FLOAT*)&mtWorld, 4);
		m_pdev->SetVertexShaderConstantF(8, (FLOAT*)&vLight, 1);
		m_pdev->SetVertexShaderConstantF(9, (FLOAT*)&color, 1);

		m_pdev->SetTexture(0, m_pDiffuseTex);
		m_pdev->SetTexture(1, m_pToonTex);
		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, m_iVertexNum - 2, m_pVertices, sizeof(Vertex));

		m_pdev->SetVertexShader(NULL);
		m_pdev->SetVertexDeclaration(NULL);
		m_pdev->SetTexture(0, NULL);
		m_pdev->SetTexture(1, NULL);
	}
}

void CShader_3dapi_01_27::Update()
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
