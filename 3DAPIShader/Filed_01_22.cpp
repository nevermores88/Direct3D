#include "stdafx.h"
#include "Filed_01_22.h"


CFiled_01_22::CFiled_01_22()
{
}


CFiled_01_22::~CFiled_01_22()
{
}

INT CFiled_01_22::Create(LPDIRECT3DDEVICE9 pDev)
{
	m_pDev = pDev;

	m_iW = 32;


	FILE* fp;

	INT x, z;
	TCHAR sFile[] = "Ex01_22/Height.raw";


	fp = fopen(sFile, "rb");

	if (!fp)
		return -1;


	long end;

	fseek(fp, 0L, SEEK_SET);	fseek(fp, 0L, SEEK_END);	end = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	BYTE*	pHi = new BYTE[end];
	fread(pHi, sizeof(BYTE), end, fp);
	fclose(fp);


	m_dFVF = VtxDUV1::FVF;
	m_iVxS = sizeof(VtxDUV1);
	m_iNvx = end;
	m_iN = int(sqrtf(FLOAT(m_iNvx)));
	m_pVtx = new VtxDUV1[m_iNvx];

	for (z = 0; z < m_iN; ++z)
	{
		for (x = 0; x < m_iN; ++x)
		{
			FLOAT fH = pHi[(m_iN - 1 - z) * m_iN + x] - 10;
			fH *= 10.f;
			m_pVtx[z * m_iN + x].p = D3DXVECTOR3(FLOAT(x * m_iW), fH, FLOAT(z * m_iW));
		}
	}


	INT iN = m_iN - 1;

	m_iNix = 8 * (m_iN - 1) / 2 * (m_iN - 1) / 2;

	m_pIdx = new VtxIdx[m_iNix];

	INT i = 0;

	WORD index;
	WORD f[9];

	for (z = 0; z < iN / 2; ++z)														// Index를 채운다.
	{
		for (x = 0; x < iN / 2; ++x)
		{
			index = 2 * m_iN*z + m_iN + 1 + 2 * x;

			f[6] = index + m_iN - 1;	f[5] = index + m_iN;	f[4] = index + m_iN + 1;
			f[7] = index - 1;	f[8] = index;	f[3] = index + 1;
			f[0] = index - m_iN - 1;	f[1] = index - m_iN;	f[2] = index - m_iN + 1;


			i = z * iN / 2 + x;
			i *= 8;

			for (int m = 0; m < 8; ++m)
				m_pIdx[i + m] = VtxIdx(f[8], f[(m + 1) % 8], f[(m + 0) % 8]);
		}
	}



	D3DXCOLOR	g_TableColor[] =
	{
		D3DXCOLOR(255 / 255.f, 249 / 255.f, 157 / 255.f, 1.f),
		D3DXCOLOR(124 / 255.f, 197 / 255.f, 118 / 255.f, 1.f),
		D3DXCOLOR(0 / 255.f, 166 / 255.f, 81 / 255.f, 1.f),
		D3DXCOLOR(25 / 255.f, 123 / 255.f, 48 / 255.f, 1.f),
		D3DXCOLOR(115 / 255.f, 100 / 255.f, 87 / 255.f, 1.f),
		D3DXCOLOR(255 / 255.f, 255 / 255.f, 255 / 255.f, 1.f),
	};


	for (z = 0; z < m_iN; ++z)
	{
		for (int x = 0; x < m_iN; ++x)
		{
			D3DXCOLOR c;
			INT		nIdx = z*m_iN + x;

			float	height = m_pVtx[nIdx].p.y / 4.5f;
			float	fWeight;

			if ((height) < 1.f)
			{
				fWeight = 1.f;
				c = fWeight * g_TableColor[0];
			}
			else if ((height) < 35.0f)
			{
				fWeight = (height - 1.f) / (35.f - 1.f) * 1.2f;

				if (fWeight > 1)
					fWeight = 1.f;

				c = fWeight * g_TableColor[1] + (1 - fWeight) * g_TableColor[0];
			}
			else if ((height) < 75.5f)
			{
				fWeight = (height - 35.0f) / (75.5f - 35.0f) * 1.2f;

				if (fWeight > 1)
					fWeight = 1.f;

				c = fWeight * g_TableColor[2] + (1 - fWeight) * g_TableColor[1];
			}
			else if ((height) < 100.0f)
			{
				fWeight = (height - 75.5f) / (100.f - 75.5f) * 1.2f;

				if (fWeight > 1)
					fWeight = 1.f;

				c = fWeight * g_TableColor[3] + (1 - fWeight) * g_TableColor[2];
			}
			else if ((height) < 140.5f)
			{
				fWeight = (height - 100.0f) / (140.5f - 100.f) * 1.2f;

				if (fWeight > 1)
					fWeight = 1.f;

				c = fWeight * g_TableColor[4] + (1 - fWeight) * g_TableColor[3];
			}
			else
			{
				fWeight = (height - 140.0f) / 60.F * 1.2f;

				if (fWeight > 1)
					fWeight = 1.f;

				c = fWeight * g_TableColor[5] + (1 - fWeight) * g_TableColor[4];
			}

			m_pVtx[nIdx].d = c;
		}
	}

	return 0;
}

void CFiled_01_22::Destroy()
{

}

INT CFiled_01_22::FrameMove()
{
	return 0;
}

void CFiled_01_22::Render()
{
	m_pDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pDev->SetTexture(0, NULL);
	m_pDev->SetFVF(m_dFVF);
	m_pDev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, m_iNvx, m_iNix, m_pIdx, D3DFMT_INDEX16, m_pVtx, m_iVxS);
}
