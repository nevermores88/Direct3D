#include "stdafx.h"
#include "Shader_3dapi_19.h"


CShader_3dapi_19::CShader_3dapi_19()
{
}


CShader_3dapi_19::~CShader_3dapi_19()
{
}

void CShader_3dapi_19::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_pdev = pdev;
	m_pField = new CField;

	m_pField->Create(m_pdev, "Shader_3dapi_19/data/Field_Height10.raw", "Shader_3dapi_19/data/map_diffuse.png", "Shader_3dapi_19/data/map_dtail.png");

	m_pTreeMesh = new CMesh;
	m_pTreeMesh->Create(m_pdev, L"Shader_3dapi_19/xFile/pine04.x", "Shader_3dapi_19/xFile");

	m_nTreeNum = 80;
	m_matTrees = new D3DXMATRIX[m_nTreeNum];

	FLOAT fW = 16.f;

	D3DXMATRIX mtR;
	D3DXMATRIX mtY;
	D3DXMATRIX mtX;

	for (int i = 0; i < m_nTreeNum; ++i)
	{
		float fAngleT = -20.0f + rand() % 41;
		float fAngleP = 0.f + rand() % 360;

		fAngleT *= (D3DX_PI / 180.0f);
		fAngleP *= (D3DX_PI / 180.0f);

		D3DXMatrixRotationX(&mtX, fAngleT);
		D3DXMatrixRotationY(&mtY, fAngleP);

		mtR = mtY*mtX;

		D3DXMatrixIdentity(&m_matTrees[i]);

		float x = 0, y = 0, z = 0;
		x = rand() % 65 * fW;
		z = rand() % 65 * fW;

		D3DXVECTOR3 vcIn(x, y, z);
		D3DXVECTOR3 vcOut;

		if (SUCCEEDED(m_pField->GetHeight(&vcOut, &vcIn)))
		{
			y = vcOut.y;
		}

		float fScale = 1.0f;
		fScale = 10.0f + rand() % 31;
		fScale *= 0.35f;

		m_matTrees[i]._11 = fScale; m_matTrees[i]._22 = fScale; m_matTrees[i]._33 = fScale;
		m_matTrees[i] *= mtR;
		m_matTrees[i]._41 = x; m_matTrees[i]._42 = y; m_matTrees[i]._43 = z;
	}

	m_pSkyBox = new CMesh;
	m_pSkyBox->Create(m_pdev, L"Shader_3dapi_19/xFile/skybox2.x", "Shader_3dapi_19/xFile");

	// 렌더 타켓용 텍스처 생성
	LcD3D_CreateRenderTarget(NULL, &m_pTrnd, m_pdev);
	InitShader();
}

void CShader_3dapi_19::Release()
{

}

void CShader_3dapi_19::OnRender()
{
	m_pTrnd->BeginScene();

	// 지형을 그린다.
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

	//// Enable fog blending.
	//m_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);

	//// Set the fog color.
	//m_pd3dDevice->SetRenderState(D3DRS_FOGCOLOR, dFogColor);

	//m_pd3dDevice->SetRenderState(D3DRS_RANGEFOGENABLE, FALSE);

	//m_pd3dDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
	//m_pd3dDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&fFogBgn));
	//m_pd3dDevice->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&fFogEnd));

	// 스카이박스를 그린다.
	D3DXMATRIX	mtSky;
	D3DXMatrixScaling(&mtSky, 400, 400, 400);
	D3DXVECTOR3 vcEye;
	g_pCamManager->GetCamera()->GetPosition(&vcEye);

	mtSky._41 = vcEye.x;
	mtSky._42 = vcEye.y - 800;
	mtSky._43 = vcEye.z;

	m_pdev->SetTransform(D3DTS_WORLD, &mtSky);
	m_pSkyBox->OnRender();

	D3DXMATRIX mtI;
	D3DXMatrixIdentity(&mtI);
	m_pdev->SetTransform(D3DTS_WORLD, &mtI);

	// 지형을 그린다.
	m_pField->Render();

	// 나무를 그린다.
	for (int i = 0; i < m_nTreeNum; ++i)
	{
	m_pdev->SetTransform(D3DTS_WORLD, &m_matTrees[i]);
	m_pTreeMesh->OnRender();
	}

	D3DXMatrixIdentity(&mtI);
	m_pdev->SetTransform(D3DTS_WORLD, &mtI);
	m_pdev->SetRenderState(D3DRS_FOGENABLE, FALSE);

	m_pTrnd->EndScene();

	RenderShader();
}

void CShader_3dapi_19::OnUpdate()
{
	/*D3DXVECTOR3 vecIn;
	D3DXVECTOR3 vecOut;
	g_pCamManager->GetCamera()->GetPosition(&vecIn);
	m_pField->GetHeight(&vecOut, &vecIn);
	vecIn.y = vecOut.y + 40.0f;
	g_pCamManager->GetCamera()->SetPosition(&vecIn);*/
}

void CShader_3dapi_19::InitShader()
{
	HRESULT hr = 0;

	LPD3DXBUFFER	pShader = NULL;
	LPD3DXBUFFER	pError = NULL;
	hr = D3DXCompileShaderFromFile(L"Shader_3dapi_19/data/shader.fx"
		, NULL
		, NULL
		, "Main"
		, "ps_2_0"
		, D3DXSHADER_DEBUG
		, &pShader
		, &pError
		, &m_pConstTable
		);

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
		&m_pShader);

	D3DVERTEXELEMENT9 vertex_decl[MAX_FVF_DECL_SIZE] = { 0 };
	D3DXDeclaratorFromFVF(VtxRHWDUV1::FVF, vertex_decl);
	m_pdev->CreateVertexDeclaration(vertex_decl, &m_pFVF);

	FLOAT	fScnW = 800;
	FLOAT	fScnH = 600;

	/*m_pVtx[0] = VtxRHWDUV1(-.5f, -.5f, 0.f, 0.f);
	m_pVtx[1] = VtxRHWDUV1(fScnW, -.5f, 1.f, 0.f);
	m_pVtx[2] = VtxRHWDUV1(fScnW, fScnH, 1.f, 1.f);
	m_pVtx[3] = VtxRHWDUV1(-.5f, fScnH, 0.f, 1.f);*/

	m_pVtx[0] = VtxRHWDUV1(0.0f, 0.0f, 0.f, 0.f);
	m_pVtx[1] = VtxRHWDUV1(1424.0f, 0.0f, 1.f, 0.f);
	m_pVtx[2] = VtxRHWDUV1(1424.0f, 700.0f, 1.f, 1.f);
	m_pVtx[3] = VtxRHWDUV1(0.0f, 700.0f, 0.f, 1.f);

}

void CShader_3dapi_19::RenderShader()
{
	m_pTex = (LPDIRECT3DTEXTURE9)m_pTrnd->GetTexture();

	m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);


	m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	m_pdev->SetSamplerState(0, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);

	m_pdev->SetVertexDeclaration(m_pFVF);
	m_pdev->SetPixelShader(m_pShader);

	m_pdev->SetFVF(VtxRHWDUV1::FVF);
	m_pdev->SetTexture(0, m_pTex);

	m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_pVtx, sizeof(VtxRHWDUV1));
	m_pdev->SetVertexDeclaration(NULL);
	m_pdev->SetPixelShader(NULL);

	m_pdev->SetTexture(0, NULL);

	m_pdev->SetPixelShader(NULL);
}
