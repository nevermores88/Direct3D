#include "stdafx.h"
#include "Field.h"


CField::CField()
{
}


CField::~CField()
{
}

HRESULT CField::Create(LPDIRECT3DDEVICE9 pDev, char* sRaw, char* sTexDif, char* sTexDet)
{
	m_pdev = pDev;

	int x;
	int z;
	int n;
	INT	nVtxT;		// 축에 대한 버텍스 수

	// 1. 정점과 인덱스 구성
	m_TileN = 128;
	m_TileW = 6.f;
	m_fHscl = .8f;
	m_fUV = 16.f;

	nVtxT = m_TileN + 1;
	m_nVtx = nVtxT * nVtxT;
	m_nFce = 2 * m_TileN * m_TileN;


	m_pVertex = new Vertex[m_nVtx];
	m_pIndex = new Index[m_nFce];


	//	1-----3
	//	.\    |
	//	.  \  |
	//	.    \|
	//	0-----2
	n = 0;

	for (z = 0; z < m_TileN; ++z)
	{
		for (x = 0; x < m_TileN; ++x)
		{
			int _0 = nVtxT*(z + 0) + x;
			int _1 = nVtxT*(z + 1) + x;
			int _2 = nVtxT*(z + 0) + x + 1;
			int _3 = nVtxT*(z + 1) + x + 1;


			m_pIndex[n] = Index(_0, _1, _2);
			++n;
			m_pIndex[n] = Index(_3, _2, _1);
			++n;
		}
	}


	for (z = 0; z <= m_TileN; ++z)
	{
		for (x = 0; x <= m_TileN; ++x)
		{
			n = z * nVtxT + x;

			m_pVertex[n].p = D3DXVECTOR3(FLOAT(x), 0.F, FLOAT(z));
			m_pVertex[n].p *= m_TileW;

			m_pVertex[n].u0 = FLOAT(x) / m_TileN;
			m_pVertex[n].v0 = 1.f - FLOAT(z) / m_TileN;

			m_pVertex[n].u1 = m_fUV * m_pVertex[n].u0;
			m_pVertex[n].v1 = m_fUV * m_pVertex[n].v0;
		}
	}



	//2. 높이 결정(Test)
	FILE* fp;

	fp = fopen(sRaw, "rb");
	if (!fp)
		return -1;

	fseek(fp, 0, SEEK_END);
	long lSize = ftell(fp);

	fseek(fp, 0, SEEK_SET);
	BYTE* pH = new BYTE[lSize];

	fread(pH, lSize, 1, fp);
	fclose(fp);


	for (z = 0; z <= m_TileN; ++z)
	{
		for (x = 0; x <= m_TileN; ++x)
		{
			FLOAT h;
			n = z * nVtxT + x;
			h = pH[n] * m_fHscl;
			m_pVertex[n].p.y = h;

			DWORD d;

			if (h < 1.f)
				d = D3DCOLOR_XRGB(255, 249, 157);
			else if (h < 45.0f)
				d = D3DCOLOR_XRGB(124, 197, 118);
			else if (h < 85.5f)
				d = D3DCOLOR_XRGB(0, 166, 81);
			else if (h < 120.0f)
				d = D3DCOLOR_XRGB(25, 123, 48);
			else if (h < 170.f)
				d = D3DCOLOR_XRGB(115, 100, 87);
			else
				d = D3DCOLOR_XRGB(255, 255, 255);

			m_pVertex[n].color = d;
		}
	}

	delete[] pH;



	//3. 법선 벡터 설정
	SetupNormal();

	if (sTexDif)
		D3DXCreateTextureFromFile(m_pdev, sTexDif, &m_pTxDif);

	if (sTexDet)
		D3DXCreateTextureFromFile(m_pdev, sTexDet, &m_pTxDet);
}

void CField::Release()
{

}

void CField::Update()
{

}

void CField::Render()
{
	if (m_pdev)
	{
		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pdev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		m_pdev->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pdev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		m_pdev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

		m_pdev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pdev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		m_pdev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

		m_pdev->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
		m_pdev->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
		m_pdev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE2X);

		m_pdev->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
		m_pdev->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
		m_pdev->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

		if (m_pTxDif)
			m_pdev->SetTexture(0, m_pTxDif);

		if (m_pTxDet)
			m_pdev->SetTexture(1, m_pTxDet);

		m_pdev->SetFVF(Vertex::FVF);

		m_pdev->DrawIndexedPrimitiveUP(
			D3DPT_TRIANGLELIST
			, 0
			, m_nVtx
			, m_nFce
			, m_pIndex
			, D3DFMT_INDEX16
			, m_pVertex, sizeof(Vertex));

		m_pdev->SetTexture(0, NULL);
		m_pdev->SetTexture(1, NULL);

		m_pdev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_pdev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pdev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
	}
}

int CField::GetHeight(D3DXVECTOR3* pOut, const D3DXVECTOR3* pIn)
{
	D3DXVECTOR3 vIn = *pIn;
	int nX = int(pIn->x / m_TileW);
	int nZ = int(pIn->z / m_TileW);

	// 실패
	if (nX < 0 || nX >= m_TileN ||
		nZ < 0 || nZ >= m_TileN)
		return -1;

	//	1------3
	//	.＼    |
	//	.  ＼  |
	//	.    ＼|
	//	0------2

	int _0 = nX + 0 + (m_TileN + 1)*(nZ + 0);
	int _1 = nX + 0 + (m_TileN + 1)*(nZ + 1);
	int _2 = nX + 1 + (m_TileN + 1)*(nZ + 0);
	int _3 = nX + 1 + (m_TileN + 1)*(nZ + 1);


	FLOAT	dX = vIn.x - nX * m_TileW;
	FLOAT	dZ = vIn.z - nZ * m_TileW;

	D3DXVECTOR3 vcX;
	D3DXVECTOR3 vcZ;
	D3DXVECTOR3 vcOut;

	if ((dX + dZ) <= m_TileW)	// 아래 쪽 삼각형
	{
		vcX = m_pVertex[_2].p - m_pVertex[_0].p;
		vcZ = m_pVertex[_1].p - m_pVertex[_0].p;

		vcOut = vcX * dX / m_TileW + vcZ * dZ / m_TileW;
		vcOut += m_pVertex[_0].p;
	}
	else					// 위 쪽 삼각형
	{
		dX = m_pVertex[_3].p.x - vIn.x;
		dZ = m_pVertex[_3].p.z - vIn.z;

		vcX = m_pVertex[_1].p - m_pVertex[_3].p;
		vcZ = m_pVertex[_2].p - m_pVertex[_3].p;

		vcOut = vcX * dX / m_TileW + vcZ * dZ / m_TileW;
		vcOut += m_pVertex[_3].p;
	}

	*pOut = vcOut;

	return 0;
}

void CField::SetupNormal()
{
	int x;
	int z;
	int	k;
	int n;
	INT	nVtxT = m_TileN + 1;

	D3DXVECTOR3	Normal(0, 0, 0);
	D3DXVECTOR3	v0, v1, v2;
	D3DXVECTOR3	Nor;


	for (z = 1; z < m_TileN; ++z)
	{
		for (x = 1; x < m_TileN; ++x)
		{
			n = z * nVtxT + x;

			INT nIdx[6][3] =
			{
				{ n, n - 1, n - nVtxT },
				{ n, n - nVtxT, n - nVtxT + 1 },
				{ n, n - nVtxT + 1, n + 1 },
				{ n, n + 1, n + nVtxT },
				{ n, n + nVtxT, n + nVtxT - 1 },
				{ n, n + nVtxT - 1, n - 1 },
			};

			Normal = D3DXVECTOR3(0, 0, 0);

			for (k = 0; k < 6; ++k)
			{
				v0 = m_pVertex[nIdx[k][0]].p;
				v1 = m_pVertex[nIdx[k][1]].p;
				v2 = m_pVertex[nIdx[k][2]].p;

				CalculateNormal(&Nor, &v0, &v1, &v2);
				Normal += Nor;
			}

			Normal /= 6.f;
			D3DXVec3Normalize(&Normal, &Normal);

			m_pVertex[n].n = Normal;
		}
	}



	//왼쪽
	for (z = 1; z < m_TileN; ++z)
	{
		x = 0;
		n = z * nVtxT + x;

		INT nIdx[3][3] =
		{
			{ n, n - nVtxT, n - nVtxT + 1 },
			{ n, n - nVtxT + 1, n + 1 },
			{ n, n + 1, n + nVtxT },
		};

		Normal = D3DXVECTOR3(0, 0, 0);

		for (k = 0; k < 3; ++k)
		{
			v0 = m_pVertex[nIdx[k][0]].p;
			v1 = m_pVertex[nIdx[k][1]].p;
			v2 = m_pVertex[nIdx[k][2]].p;

			CalculateNormal(&Nor, &v0, &v1, &v2);
			Normal += Nor;
		}

		Normal /= 3.f;
		D3DXVec3Normalize(&Normal, &Normal);

		m_pVertex[n].n = Normal;
	}


	//오른쪽
	for (z = 1; z < m_TileN; ++z)
	{
		x = m_TileN;
		n = z * nVtxT + x;

		INT nIdx[3][3] =
		{
			{ n, n - 1, n - nVtxT },
			{ n, n + nVtxT, n + nVtxT - 1 },
			{ n, n + nVtxT - 1, n - 1 },
		};

		Normal = D3DXVECTOR3(0, 0, 0);

		for (k = 0; k < 3; ++k)
		{
			v0 = m_pVertex[nIdx[k][0]].p;
			v1 = m_pVertex[nIdx[k][1]].p;
			v2 = m_pVertex[nIdx[k][2]].p;

			CalculateNormal(&Nor, &v0, &v1, &v2);
			Normal += Nor;
		}

		Normal /= 3.f;
		D3DXVec3Normalize(&Normal, &Normal);

		m_pVertex[n].n = Normal;
	}



	//아래
	for (x = 1; x < m_TileN; ++x)
	{
		z = 0;
		n = z * nVtxT + x;

		INT nIdx[3][3] =
		{
			{ n, n + 1, n + nVtxT },
			{ n, n + nVtxT, n + nVtxT - 1 },
			{ n, n + nVtxT - 1, n - 1 },
		};

		Normal = D3DXVECTOR3(0, 0, 0);

		for (k = 0; k < 3; ++k)
		{
			v0 = m_pVertex[nIdx[k][0]].p;
			v1 = m_pVertex[nIdx[k][1]].p;
			v2 = m_pVertex[nIdx[k][2]].p;

			CalculateNormal(&Nor, &v0, &v1, &v2);
			Normal += Nor;
		}

		Normal /= 3.f;
		D3DXVec3Normalize(&Normal, &Normal);

		m_pVertex[n].n = Normal;
	}


	//위
	for (x = 1; x < m_TileN; ++x)
	{
		z = m_TileN;
		n = z * nVtxT + x;

		INT nIdx[6][3] =
		{
			{ n, n - 1, n - nVtxT },
			{ n, n - nVtxT, n - nVtxT + 1 },
			{ n, n - nVtxT + 1, n + 1 },
		};

		Normal = D3DXVECTOR3(0, 0, 0);

		for (k = 0; k < 3; ++k)
		{
			v0 = m_pVertex[nIdx[k][0]].p;
			v1 = m_pVertex[nIdx[k][1]].p;
			v2 = m_pVertex[nIdx[k][2]].p;

			CalculateNormal(&Nor, &v0, &v1, &v2);
			Normal += Nor;
		}

		Normal /= 3.f;
		D3DXVec3Normalize(&Normal, &Normal);

		m_pVertex[n].n = Normal;
	}


	//(0,0)
	{
		x = 0;
		z = 0;
		n = z * nVtxT + x;

		v0 = m_pVertex[n].p;
		v1 = m_pVertex[n + 1].p;
		v2 = m_pVertex[n + nVtxT].p;

		CalculateNormal(&Nor, &v0, &v1, &v2);
		m_pVertex[n].n = Nor;
	}


	//(m_TileN,0)
	{
		x = m_TileN;
		z = 0;
		n = z * nVtxT + x;

		INT nIdx[2][3] =
		{
			{ n, n + nVtxT, n + nVtxT - 1 },
			{ n, n + nVtxT - 1, n - 1 },
		};

		Normal = D3DXVECTOR3(0, 0, 0);

		for (k = 0; k < 2; ++k)
		{
			v0 = m_pVertex[nIdx[k][0]].p;
			v1 = m_pVertex[nIdx[k][1]].p;
			v2 = m_pVertex[nIdx[k][2]].p;

			CalculateNormal(&Nor, &v0, &v1, &v2);
			Normal += Nor;
		}

		Normal /= 2.f;
		D3DXVec3Normalize(&Normal, &Normal);

		m_pVertex[n].n = Normal;
	}


	//(0, m_TileN)
	{
		x = 0;
		z = m_TileN;
		n = z * nVtxT + x;


		INT nIdx[2][3] =
		{
			{ n, n - nVtxT, n - nVtxT + 1 },
			{ n, n - nVtxT + 1, n + 1 },
		};

		Normal = D3DXVECTOR3(0, 0, 0);

		for (k = 0; k < 2; ++k)
		{
			v0 = m_pVertex[nIdx[k][0]].p;
			v1 = m_pVertex[nIdx[k][1]].p;
			v2 = m_pVertex[nIdx[k][2]].p;

			CalculateNormal(&Nor, &v0, &v1, &v2);
			Normal += Nor;
		}

		Normal /= 2.f;
		D3DXVec3Normalize(&Normal, &Normal);

		m_pVertex[n].n = Normal;
	}


	//(m_TileN, m_TileN)
	{
		x = m_TileN;
		z = m_TileN;
		n = z * nVtxT + x;

		v0 = m_pVertex[n].p;
		v1 = m_pVertex[n - 1].p;
		v2 = m_pVertex[n - nVtxT].p;

		CalculateNormal(&Nor, &v0, &v1, &v2);
		m_pVertex[n].n = Nor;
	}

}

void CField::CalculateNormal(D3DXVECTOR3* pOut, D3DXVECTOR3* v0, D3DXVECTOR3* v1, D3DXVECTOR3* v2)
{
	D3DXVECTOR3 n;
	D3DXVECTOR3 A = *v2 - *v0;
	D3DXVECTOR3 B = *v1 - *v0;
	D3DXVec3Cross(&n, &A, &B);
	D3DXVec3Normalize(&n, &n);

	*pOut = n;
}
