#include "stdafx.h"
#include "Basic_3dapi_08_03.h"


CBasic_3dapi_08_03::CBasic_3dapi_08_03()
{
}


CBasic_3dapi_08_03::~CBasic_3dapi_08_03()
{
}

HRESULT CBasic_3dapi_08_03::Create(LPDIRECT3DDEVICE9 pdev)
{
	CBaseClass::Create(pdev);

	// Setup Tiger
	{
		m_pMeshS = NULL;
		if (FAILED(D3DXLoadMeshFromX("Ex08_01/Tiger.x", D3DXMESH_SYSTEMMEM, m_pdev, NULL, NULL, NULL, NULL, &m_pMeshS)))
			return E_FAIL;

		D3DXCreateTextureFromFile(m_pdev, "Ex08_01/tiger.bmp", &m_pTigerTex);


		LPD3DXMESH pMeshD = NULL;
		m_pMeshS->CloneMeshFVF(D3DXMESH_SYSTEMMEM, Vertex::FVF, m_pdev, &pMeshD);
		//pMeshS->Release();

		m_iNumVertex = pMeshD->GetNumVertices();
		m_iNumIndex = pMeshD->GetNumFaces();

		//	DWORD dFVF = pMdlD->GetFVF();

		m_pVertices = new Vertex[m_iNumVertex];
		m_pIndices = new Index[m_iNumIndex];

		void* pVtx = NULL;
		pMeshD->LockVertexBuffer(0, &pVtx);

		memcpy(m_pVertices, pVtx, m_iNumVertex * sizeof(Vertex));

		pMeshD->UnlockVertexBuffer();

		void* pIdx = NULL;
		pMeshD->LockIndexBuffer(0, &pIdx);

		memcpy(m_pIndices, pIdx, m_iNumIndex * sizeof(Index));

		pMeshD->UnlockIndexBuffer();

		pMeshD->Release();


		float fSCale = 10;
		float	fMin = 100000;
		for (int i = 0; i < m_iNumVertex; ++i)
		{
			m_pVertices[i].x *= fSCale;
			m_pVertices[i].y *= fSCale;
			m_pVertices[i].z *= fSCale;
			
			if (m_pVertices[i].y < fMin)
				fMin = m_pVertices[i].y;
		}

		fMin = -fMin;

		for (int i = 0; i < m_iNumVertex; ++i)
		{
			m_pVertices[i].y += fMin;
		}

		m_vTiger = D3DXVECTOR3(8.0f, .0f, 0.f);
	}

	// Setup Wall
	D3DXCreateTextureFromFile(m_pdev, "Ex08_01/seafloor.bmp", &m_pFloorTex);

	m_pFloorVertices[0] = Vertex(-25.f, -1.f, -25.f, 0xFFFFFFFF, 0.f, 4.f);
	m_pFloorVertices[1] = Vertex(-25.f, -1.f, 25.f, 0xFFFFFFFF, 0.f, 0.f);
	m_pFloorVertices[2] = Vertex(25.f, -1.f, 25.f, 0xFFFFFFFF, 0.f, 0.f);
	m_pFloorVertices[3] = Vertex(25.f, -1.f, -25.f, 0xFFFFFFFF, 0.f, 4.f);

	return S_OK;
}

void CBasic_3dapi_08_03::Release()
{

}

void CBasic_3dapi_08_03::Render()
{
	if (m_pdev)
	{
		D3DXMATRIX mtWorld;	D3DXMatrixIdentity(&mtWorld);
		D3DXMATRIX mtRotY;	D3DXMatrixRotationY(&mtRotY, D3DXToRadian(GetTickCount() *0.05F));
		D3DXMATRIX mtIdentity;		D3DXMatrixIdentity(&mtIdentity);

		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_pdev->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		m_pdev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

		// 1. 스텐실이 Default로 설정되어 있는 오브젝트들을 먼저 그린다.
		// 호랑이를 그린다.
		mtWorld = mtRotY;
		mtWorld._41 = m_vTiger.x; mtWorld._42 = m_vTiger.y; mtWorld._43 = m_vTiger.z;

		m_pdev->SetTransform(D3DTS_WORLD, &mtWorld);
		m_pdev->SetFVF(Vertex::FVF);
		m_pdev->SetTexture(0, m_pTigerTex);
		m_pdev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, m_iNumVertex, m_iNumIndex, m_pIndices, D3DFMT_INDEX16, m_pVertices, sizeof(Vertex));

		m_pdev->SetTexture(0, NULL);
		m_pdev->SetTransform(D3DTS_WORLD, &mtIdentity);

		// 2. 스텐실을 설정하고 바닥을 그린다.
		m_pdev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		m_pdev->SetRenderState(D3DRS_STENCILENABLE, TRUE);
		m_pdev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
		m_pdev->SetRenderState(D3DRS_STENCILREF, 0x1);
		m_pdev->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
		m_pdev->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
		m_pdev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
		m_pdev->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
		m_pdev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);

		m_pdev->SetTexture(0, m_pFloorTex);
		m_pdev->SetFVF(Vertex::FVF);
		m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, m_pFloorVertices, sizeof(Vertex));
		m_pdev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		//3. 그림자를 그린다.
		// 호랑이를 위한 월드 행렬을 설정한다.
		// 호랑이를 평면에 투영 되도록 행렬을 만든다.
		D3DXVECTOR4 vcLight(-2.8f, -1.0f, 0.0f, 0.0f);
		D3DXPLANE	dxPlane(0.f, -1.0f, 0.0f, 0.0f);

		D3DXMATRIX mtShd;
		D3DXMATRIX mtTrn;

		D3DXVec4Normalize(&vcLight, &vcLight);
		D3DXMatrixShadow(&mtShd, &vcLight, &dxPlane);
		D3DXMatrixTranslation(&mtTrn, m_vTiger.x, m_vTiger.y, m_vTiger.z);

		mtWorld = mtShd* mtRotY *mtTrn;

		// 호랑이를 평면에 그리되 스텐실 값을 1로 설정한다.
		m_pdev->SetRenderState(D3DRS_STENCILENABLE, TRUE);
		m_pdev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
		m_pdev->SetRenderState(D3DRS_STENCILREF, 0x1);
		m_pdev->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
		m_pdev->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);

		// 호랑이에 대한 모든 스텐실 테스트에 대해서 값을 0X1로 설정한다.
		m_pdev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
		m_pdev->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
		m_pdev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

		// 알파 블렌딩 활성화
		m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pdev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pdev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// 디바이스에 TFACTOR 색상을 정하고 이것을 호랑이 색상으로 설정한다.
		m_pdev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DXCOLOR(0.3F, 0.3F, 0.3F, 0.3F));
		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
		m_pdev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);

		// 호랑이를 그린다.
		m_pdev->SetTransform(D3DTS_WORLD, &mtWorld);
		m_pdev->SetFVF(Vertex::FVF);
		m_pdev->SetTexture(0, NULL);
		m_pdev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, m_iNumVertex, m_iNumIndex, m_pIndices, D3DFMT_INDEX16, m_pVertices, sizeof(Vertex));

		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		m_pdev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

		m_pdev->SetTexture(0, NULL);
		m_pdev->SetTransform(D3DTS_WORLD, &mtIdentity);

		m_pdev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pdev->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	}
}

void CBasic_3dapi_08_03::Update()
{

}
