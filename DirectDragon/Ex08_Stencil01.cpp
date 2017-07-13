#include "stdafx.h"
#include "Ex08_Stencil01.h"


CEx08_Stencil01::CEx08_Stencil01()
{
}


CEx08_Stencil01::~CEx08_Stencil01()
{
}

void CEx08_Stencil01::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	DataClear();
	m_dwExType = dwExType;
	m_pdev = pdev;

	m_FloorMtrl = d3d::WHITE_MTRL;
	m_WallMtrl = d3d::WHITE_MTRL;
	m_MirrorMtrl = d3d::WHITE_MTRL;

	m_vTeapotPos = D3DXVECTOR3(0.0f, 3.0f, -7.5f);
	m_TeapotMtrl = d3d::YELLOW_MTRL;

	m_WallMtrl.Specular = d3d::WHITE * 0.2f;

	D3DXCreateTeapot(m_pdev, &m_pTeapot, 0);

	m_pdev->CreateVertexBuffer(
		24 * sizeof(Vertex),
		0,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&m_pVB,
		0);

	Vertex* v = 0;
	m_pVB->Lock(0, 0, (void**)&v, 0);

	// floor
	v[0] = Vertex(-7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[1] = Vertex(-7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[2] = Vertex(7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

	v[3] = Vertex(-7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[4] = Vertex(7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[5] = Vertex(7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	// wall
	v[6] = Vertex(-7.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[7] = Vertex(-7.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[8] = Vertex(-2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[9] = Vertex(-7.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[10] = Vertex(-2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[11] = Vertex(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Note: We leave gap in middle of walls for mirror

	v[12] = Vertex(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[13] = Vertex(2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[14] = Vertex(7.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[15] = Vertex(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[16] = Vertex(7.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[17] = Vertex(7.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// mirror
	v[18] = Vertex(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[19] = Vertex(-2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[20] = Vertex(2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[21] = Vertex(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[22] = Vertex(2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[23] = Vertex(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	m_pVB->Unlock();

	D3DXCreateTextureFromFile(m_pdev, L"Ex08/checker.jpg", &m_pFloorTex);
	D3DXCreateTextureFromFile(m_pdev, L"Ex08/brick0.jpg", &m_pWallTex);
	D3DXCreateTextureFromFile(m_pdev, L"Ex08/ice.bmp", &m_pMirrorTex);

	m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);


}

void CEx08_Stencil01::Release()
{

}

void CEx08_Stencil01::OnRender()
{
	if (m_pdev)
	{
		m_pdev->SetLight(0, &m_Light);
		m_pdev->LightEnable(0, TRUE);

		m_pdev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		m_pdev->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
		
		////draw teapot
		//m_pdev->SetMaterial(&m_TeapotMtrl);
		//m_pdev->SetTexture(0, 0);

		D3DXMATRIX mat;
		D3DXMatrixTranslation(&mat, m_vTeapotPos.x, m_vTeapotPos.y, m_vTeapotPos.z);

		/*m_pdev->SetTransform(D3DTS_WORLD, &mat);
		m_pTeapot->DrawSubset(0);*/

		D3DXMATRIX identity;
		D3DXMatrixIdentity(&identity);
		m_pdev->SetTransform(D3DTS_WORLD, &identity);

		m_pdev->SetStreamSource(0, m_pVB, 0, sizeof(Vertex));
		m_pdev->SetFVF(Vertex::FVF);

		//draw the floor
		m_pdev->SetMaterial(&m_FloorMtrl);
		m_pdev->SetTexture(0, m_pFloorTex);
		m_pdev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

		//draw the walls
		m_pdev->SetMaterial(&m_WallMtrl);
		m_pdev->SetTexture(0, m_pWallTex);
		m_pdev->DrawPrimitive(D3DPT_TRIANGLELIST, 6, 4);

		//draw the mirror
		m_pdev->SetMaterial(&m_MirrorMtrl);
		m_pdev->SetTexture(0, m_pMirrorTex);
		m_pdev->DrawPrimitive(D3DPT_TRIANGLELIST, 18, 2);

		RenderShadow();
		m_pdev->SetMaterial(&m_TeapotMtrl);
		m_pdev->SetTexture(0, 0);

		m_pdev->SetTransform(D3DTS_WORLD, &mat);
		m_pTeapot->DrawSubset(0);
		RenderMirror();
	}
}

void CEx08_Stencil01::OnUpdate()
{
	if (m_pdev)
	{
		D3DXVECTOR3 lightDir(0.707f, -0.707f, 0.707f);
		D3DXCOLOR color(1.0f, 1.0f, 1.0f, 1.0f);
		::ZeroMemory(&m_Light, sizeof(m_Light));

		m_Light.Type = D3DLIGHT_DIRECTIONAL;
		m_Light.Ambient = color * 0.4f;
		m_Light.Diffuse = color;
		m_Light.Specular = color * 0.6f;
		m_Light.Direction = lightDir;

		static float radius = 20.0f;

		if (::GetAsyncKeyState('Y') & 0x8000f)
			m_vTeapotPos.x -= 3.0f * 0.01f;

		if (::GetAsyncKeyState('U') & 0x8000f)
			m_vTeapotPos.x += 3.0f * 0.01f;

		/*if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
			m_vTeapotPos.x -= 3.0f * 0.01f;

			if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
			m_vTeapotPos.x += 3.0f * 0.01f;

			if (::GetAsyncKeyState(VK_UP) & 0x8000f)
			radius -= 2.0f * 0.01f;

			if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
			radius += 2.0f * 0.01f;

			static float angle = (3.0f * D3DX_PI) / 2.0f;

			if (::GetAsyncKeyState('A') & 0x8000f)
			angle -= 0.5f * 0.01f;

			if (::GetAsyncKeyState('S') & 0x8000f)
			angle += 0.5f * 0.01f;*/
	}
}

void CEx08_Stencil01::DataClear()
{
	m_pdev = NULL;

	m_pFloorTex = NULL;
	m_pWallTex = NULL;
	m_pMirrorTex = NULL;

	ZeroMemory(&m_FloorMtrl, sizeof(m_FloorMtrl));
	ZeroMemory(&m_WallMtrl, sizeof(m_WallMtrl));
	ZeroMemory(&m_MirrorMtrl, sizeof(m_MirrorMtrl));

	m_pTeapot = NULL;
	ZeroMemory(&m_TeapotMtrl, sizeof(m_TeapotMtrl));
}

void CEx08_Stencil01::RenderMirror()
{
	D3DXMATRIX identity;

	//Mirror
	//(참조 & 매스크) 비교 연산자 (값 & 매스크)
	m_pdev->SetRenderState(D3DRS_STENCILENABLE, true);
	m_pdev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	m_pdev->SetRenderState(D3DRS_STENCILREF, 0x1);				//스텐실 참조 값 설정
	m_pdev->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);		//스텐실 매스크 값 설정 (0xffffffff 이면 어떤 비트도 매스크하지 않음)
	m_pdev->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
	m_pdev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	m_pdev->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	m_pdev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);

	//깊이 버퍼 갱신 막기
	m_pdev->SetRenderState(D3DRS_ZWRITEENABLE, false);

	//후면 버퍼 갱신 막기
	m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pdev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
	m_pdev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	m_pdev->SetStreamSource(0, m_pVB, 0, sizeof(Vertex));
	m_pdev->SetFVF(Vertex::FVF);
	m_pdev->SetMaterial(&m_MirrorMtrl);
	m_pdev->SetTexture(0, m_pMirrorTex);

	D3DXMatrixIdentity(&identity);
	m_pdev->SetTransform(D3DTS_WORLD, &identity);
	m_pdev->DrawPrimitive(D3DPT_TRIANGLELIST, 18, 2);

	m_pdev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pdev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	m_pdev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

	D3DXMATRIX W, T, R;
	D3DXPLANE plane(0.0f, 0.0f, 1.0f, 0.0f);
	D3DXMatrixReflect(&R, &plane);

	D3DXMatrixTranslation(&T, m_vTeapotPos.x, m_vTeapotPos.y, m_vTeapotPos.z);

	W = T * R;

	m_pdev->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
	m_pdev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
	m_pdev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	m_pdev->SetTransform(D3DTS_WORLD, &W);
	m_pdev->SetMaterial(&m_TeapotMtrl);
	m_pdev->SetTexture(0, 0);

	m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	m_pTeapot->DrawSubset(0);

	m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pdev->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CEx08_Stencil01::RenderShadow()
{
	m_pdev->SetRenderState(D3DRS_STENCILENABLE, true);
	m_pdev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	m_pdev->SetRenderState(D3DRS_STENCILREF, 0x0);
	m_pdev->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
	m_pdev->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
	m_pdev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	m_pdev->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	m_pdev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);
	
	D3DXVECTOR4 vLightDir(0.707f, -0.707f, 0.707f, 0.0f);
	D3DXPLANE groupPlane(0.0f, -1.0f, 0.0f, 0.0f);

	D3DXMATRIX mtShadow;
	D3DXMatrixShadow(
		&mtShadow,
		&vLightDir,
		&groupPlane);

	D3DXMATRIX mtPos;
	D3DXMatrixTranslation(
		&mtPos,
		m_vTeapotPos.x,
		m_vTeapotPos.y,
		m_vTeapotPos.z);

	D3DXMATRIX mtWorld = mtPos*mtShadow;
	m_pdev->SetTransform(D3DTS_WORLD, &mtWorld);

	m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pdev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pdev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	D3DMATERIAL9 shadowMtrl = d3d::InitMtrl(d3d::BLACK, d3d::BLACK, d3d::BLACK, d3d::BLACK, 0.0f);
	shadowMtrl.Diffuse.a = 0.5f;

	m_pdev->SetRenderState(D3DRS_ZENABLE, false);

	m_pdev->SetMaterial(&shadowMtrl);
	m_pdev->SetTexture(0, 0);
	m_pTeapot->DrawSubset(0);

	m_pdev->SetRenderState(D3DRS_ZENABLE, true);
	m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_pdev->SetRenderState(D3DRS_STENCILENABLE, false);
}
