#include "stdafx.h"
#include "Shader_3dapi_03_11.h"


CShader_3dapi_03_11::CShader_3dapi_03_11()
{
}


CShader_3dapi_03_11::~CShader_3dapi_03_11()
{
}

HRESULT CShader_3dapi_03_11::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	//지형
	m_pField = new CField();
	if (m_pField)
	{
		if (FAILED(m_pField->Create(m_pdev, "Ex03_11/Field_Height10.raw", "Ex03_11/map_diffuse.png", "Ex03_11/map_dtail.png")))
			return E_FAIL;
	}

	//나무
	m_pTreeMesh = new CMesh();
	m_pTreeMesh->Create(m_pdev, "Ex03_11/pine04.x", "Ex03_11");

	m_iNumTree = 80;
	m_pTreeMatrices = new D3DXMATRIX[m_iNumTree];

	float fW= 16.0f;

	D3DXMATRIX mtR, mtY, mtX;

	for (int i = 0; i < m_iNumTree; ++i)
	{
		FLOAT fAngleT = -20.f + rand() % 41;
		FLOAT fAngleP = 0.f + rand() % 360;

		fAngleT *= (D3DX_PI / 180.F);
		fAngleP *= (D3DX_PI / 180.F);

		D3DXMatrixRotationY(&mtY, fAngleP);
		D3DXMatrixRotationX(&mtX, fAngleT);

		mtR = mtY * mtX;

		D3DXMatrixIdentity(&m_pTreeMatrices[i]);
		float x = 0;
		float y = 0;
		float z = 0;
		x = rand() % 65 * fW;
		z = rand() % 65 * fW;

		D3DXVECTOR3 vcIn(x, y, z);
		D3DXVECTOR3 vcOut;
		if (SUCCEEDED(m_pField->GetHeight(&vcOut, &vcIn)))
		{
			y = vcOut.y;
		}

		float fScl = 1;
		fScl = 10.f + rand() % 31;
		fScl *= .35f;

		m_pTreeMatrices[i]._11 = fScl;
		m_pTreeMatrices[i]._22 = fScl;
		m_pTreeMatrices[i]._33 = fScl;

		m_pTreeMatrices[i] *= mtR;

		m_pTreeMatrices[i]._41 = x;
		m_pTreeMatrices[i]._42 = y;
		m_pTreeMatrices[i]._43 = z;
	}

	//렌더 타겟용 텍스쳐 생성
	if (FAILED(LcD3D_CreateRenderTarget(NULL, &m_pRenderTarget, m_pdev)))
		return E_FAIL;

	//셰이더
	HWND hWnd;
	D3DDEVICE_CREATION_PARAMETERS ppm;
	m_pdev->GetCreationParameters(&ppm);
	hWnd = ppm.hFocusWindow;

	m_pVertex[0] = Vertex(-1.05F, -1.02F, 0, 0xffffffff, 0.f, 1.f);
	m_pVertex[1] = Vertex(1.05F, -1.02F, 0, 0xffffffff, 1.f, 1.f);
	m_pVertex[2] = Vertex(1.05F, 1.02F, 0, 0xffffffff, 1.f, 0.f);
	m_pVertex[3] = Vertex(-1.05F, 1.02F, 0, 0xffffffff, 0.f, 0.f);

	DWORD	dFVF = Vertex::FVF;
	D3DVERTEXELEMENT9 vertex_decl[MAX_FVF_DECL_SIZE] = { 0 };
	D3DXDeclaratorFromFVF(dFVF, vertex_decl);
	if (FAILED(m_pdev->CreateVertexDeclaration(vertex_decl, &m_pFVF)))
		return E_FAIL;

	m_pShader = LoadShader("Ex03_11/Shader.fx");
	if (!m_pShader)
		return E_FAIL;

	return S_OK;
}

void CShader_3dapi_03_11::Release()
{
	if (m_pField)
	{
		delete m_pField;
		m_pField = NULL;
	}
}

void CShader_3dapi_03_11::Render()
{
	if (m_pdev)
	{
		m_pTex = (LPDIRECT3DTEXTURE9)m_pRenderTarget->GetTexture();
		
		static float fDeviation = 0.0f;
		static float fDir = 1.0f;

		fDeviation += fDir * 0.2f;

		if (fDeviation > 5.f)
			fDir *= -1.f;

		if (fDeviation < 0.f)
			fDir *= -1.f;

	/*	m_pdev->SetVertexDeclaration(m_pFVF);
		m_pdev->SetTexture(0, m_pTex);
		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_pVertex, sizeof(Vertex));*/

		m_pdev->SetVertexDeclaration(m_pFVF);

		m_pShader->SetTechnique("Tech");
		m_pShader->SetTexture("g_Tex", m_pTex);
		m_pShader->SetValue("g_fDeviation", &fDeviation, 4);

		UINT numPasses = 0;

		m_pShader->Begin(&numPasses, NULL);

		for (UINT i = 0; i < numPasses; ++i)
		{
			m_pShader->BeginPass(i);
			{
				m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_pVertex, sizeof(Vertex));
			}
			m_pShader->EndPass();
		}
		m_pShader->End();

		m_pdev->SetVertexDeclaration(NULL);
		m_pdev->SetVertexShader(NULL);
		m_pdev->SetPixelShader(NULL);
	}
}

void CShader_3dapi_03_11::Update()
{
	if (m_pdev)
	{
		m_pRenderTarget->BeginScene();
			this->RenderScene();
		m_pRenderTarget->EndScene();
	}
}

void CShader_3dapi_03_11::RenderScene()
{
	if (m_pdev)
	{
		D3DLIGHT9 d3Lght;
		D3DXVECTOR3 vcLght(1, 1, 1);

		vcLght = -vcLght;
		D3DXVec3Normalize(&vcLght, &vcLght);

		memset(&d3Lght, 0, sizeof d3Lght);
		d3Lght.Type = D3DLIGHT_DIRECTIONAL;
		d3Lght.Direction = vcLght;
		d3Lght.Range = 15000;
		d3Lght.Position = D3DXVECTOR3(100, 20, 300);
		d3Lght.Diffuse = D3DXCOLOR(1, 1, 1, 1);

		d3Lght.Theta = 0.3f;
		d3Lght.Phi = 1.0f;
		d3Lght.Falloff = 1.0f;
		d3Lght.Attenuation0 = 1.0f;


		D3DMATERIAL9	d3Mtl;
		memset(&d3Mtl, 0, sizeof d3Mtl);

		d3Mtl.Ambient = D3DXCOLOR(1, 1, 1, 1);
		d3Mtl.Diffuse = D3DXCOLOR(1, 1, 1, 1);
		d3Mtl.Specular = D3DXCOLOR(1, 1, 1, 1);
		d3Mtl.Power = 10.f;

		m_pdev->SetMaterial(&d3Mtl);

		m_pdev->SetLight(0, &d3Lght);
		m_pdev->LightEnable(0, TRUE);
		m_pdev->SetRenderState(D3DRS_LIGHTING, TRUE);
		m_pdev->SetRenderState(D3DRS_AMBIENT, 0xFF555555);

		float	fFogBgn = 100.f;
		float	fFogEnd = 1250.f;
		D3DXCOLOR dFogColor(0.f, .3f, .5f, 1.f);

		// Enable fog blending.
		m_pdev->SetRenderState(D3DRS_FOGENABLE, TRUE);

		// Set the fog color.
		m_pdev->SetRenderState(D3DRS_FOGCOLOR, dFogColor);

		m_pdev->SetRenderState(D3DRS_RANGEFOGENABLE, FALSE);

		m_pdev->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
		m_pdev->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&fFogBgn));
		m_pdev->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&fFogEnd));

		m_pdev->SetRenderState(D3DRS_LIGHTING, TRUE);

		D3DXMATRIX mtIdentity;

		m_pField->Render();

		for (int i = 0; i < m_iNumTree; i++)
		{
			m_pdev->SetTransform(D3DTS_WORLD, &m_pTreeMatrices[i]);
			m_pTreeMesh->Render();
		}

		D3DXMatrixIdentity(&mtIdentity);
		m_pdev->SetTransform(D3DTS_WORLD, &mtIdentity);
		m_pdev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	}
}
