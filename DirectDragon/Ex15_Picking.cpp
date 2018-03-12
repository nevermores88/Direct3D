#include "stdafx.h"
#include "Ex15_Picking.h"


CEx15_Picking::CEx15_Picking()
{
}


CEx15_Picking::~CEx15_Picking()
{
}

void CEx15_Picking::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_pdev = pdev;

	D3DXCreateTeapot(m_pdev, &m_pTeapot, 0);

	BYTE* v = 0;
	m_pTeapot->LockVertexBuffer(0, (void**)&v);

	D3DXComputeBoundingSphere(
		(D3DXVECTOR3*)v,
		m_pTeapot->GetNumVertices(),
		D3DXGetFVFVertexSize(m_pTeapot->GetFVF()),
		&m_BoundingSphere.m_v3Center,
		&m_BoundingSphere.m_fRadius
		);

	m_pTeapot->UnlockVertexBuffer();

	D3DXCreateSphere(m_pdev, m_BoundingSphere.m_fRadius, 20, 20, &m_pSphere, 0);

	D3DXVECTOR3 dir(0.707f, -0.0f, 0.707f);
	D3DXCOLOR col(1.0f, 1.0f, 1.0f, 1.0f);
	D3DLIGHT9 light = d3d::InitDirectionalLight(&dir, &col);

	m_pdev->SetLight(0, &light);
	m_pdev->LightEnable(0, true);
	m_pdev->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	m_pdev->SetRenderState(D3DRS_SPECULARENABLE, false);
}

void CEx15_Picking::Release()
{
	if (m_pTeapot)
	{
		m_pTeapot->Release();
		m_pTeapot = NULL;
	}

	if (m_pSphere)
	{
		m_pSphere->Release();
		m_pSphere = NULL;
	}
}

void CEx15_Picking::OnRender()
{
	if (m_pdev)
	{
		static float r = 0.0f;
		static float v = 1.0f;
		static float angle = 0.0f;
		static float timeDelta = 0.0f;

		timeDelta = 0.01f;

		D3DXMATRIX mtWorld;

		D3DXMatrixTranslation(&mtWorld, cosf(angle) * r, sinf(angle) * r, 10.0f);

		m_BoundingSphere.m_v3Center = D3DXVECTOR3(cosf(angle) * r, sinf(angle) * r, 10.0f);
		r += v*timeDelta;

		if (r >= 8.0f)
			v = -v;

		if (r <= 0.0f)
			v = -v;

		angle += 1.0f*D3DX_PI * timeDelta;
		if (angle >= D3DX_PI * 2.0f)
			angle = 0.0f;

		m_pdev->SetTransform(D3DTS_WORLD, &mtWorld);
		m_pdev->SetMaterial(&d3d::YELLOW_MTRL);
		m_pTeapot->DrawSubset(0);

		m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		m_pdev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pdev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		D3DMATERIAL9 blue = d3d::BLUE_MTRL;
		blue.Diffuse.a = 0.25f;
		m_pdev->SetMaterial(&blue);
		m_pSphere->DrawSubset(0);

		m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	}
}

void CEx15_Picking::OnUpdate()
{
	if (m_pdev)
	{
		bool bClickedLBtn = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) ? 1 : 0;	// Left Button

		if (bClickedLBtn)
		{
			HWND hwnd;
			D3DDEVICE_CREATION_PARAMETERS ppm;
			m_pdev->GetCreationParameters(&ppm);
			hwnd = ppm.hFocusWindow;

			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(hwnd, &pt);

			d3d::Ray ray = CalculatePickingRay(pt.x, pt.y);

			D3DXMATRIX view;
			m_pdev->GetTransform(D3DTS_VIEW, &view);

			D3DXMATRIX viewInverse;
			D3DXMatrixInverse(&viewInverse, 0, &view);

			TransformRay(&ray, &viewInverse);

			if (RaySphereIntersectionTest(&ray, &m_BoundingSphere))
				::MessageBoxA(0, "Hit!", "Hit", 0);
		}
	}
}

d3d::Ray CEx15_Picking::CalculatePickingRay(int x, int y)
{
	float px = 0.0f;
	float py = 0.0f;

	D3DVIEWPORT9 vp;
	m_pdev->GetViewport(&vp);

	D3DXMATRIX proj;
	m_pdev->GetTransform(D3DTS_PROJECTION, &proj);

	px = (((2.0f*x) / vp.Width) - 1.0f) / proj(0, 0);
	py = (((-2.0f*y) / vp.Height) + 1.0f) / proj(1, 1);

	d3d::Ray ray;
	ray.origin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ray.direction = D3DXVECTOR3(px, py, 1.0f);

	return ray;
}

void CEx15_Picking::TransformRay(d3d::Ray* ray, D3DXMATRIX* mat)
{
	D3DXVec3TransformCoord(&ray->origin, &ray->origin, mat);
	D3DXVec3TransformNormal(&ray->direction, &ray->direction, mat);

	D3DXVec3Normalize(&ray->direction, &ray->direction);
}

bool CEx15_Picking::RaySphereIntersectionTest(d3d::Ray* ray, CBoundingSphere* pSphere)
{
	D3DXVECTOR3 v = ray->origin - pSphere->m_v3Center;

	float b = 2.0f * D3DXVec3Dot(&ray->direction, &v);
	float c = D3DXVec3Dot(&v, &v) - (pSphere->m_fRadius * pSphere->m_fRadius);

	float discriminant = (b * b) - (4.0f * c);

	if (discriminant < 0.0f)
		return false;

	discriminant = sqrtf(discriminant);

	float s0 = (-b + discriminant) / 2.0f;
	float s1 = (-b - discriminant) / 2.0f;

	if (s0 >= 0.0f || s1 >= 0.0f)
		return true;

	return false;
}
