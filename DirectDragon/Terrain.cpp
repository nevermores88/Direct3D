#include "stdafx.h"
#include "Terrain.h"


CTerrain::CTerrain()
{
}


CTerrain::~CTerrain()
{
}

void CTerrain::Create(LPDIRECT3DDEVICE9 pDev, std::string szHeightMapFileName, int nNumVertsPerRow, int nNumVertsPerCol, int nCellSpacing, float fHeightScale)
{
	m_pdev = pDev;

	m_nNumVertsPerRow = nNumVertsPerRow;
	m_nNumVertsPerCol = nNumVertsPerCol;
	m_nCellSpacing = nCellSpacing;

	m_nNumCellsPerRow = m_nNumVertsPerRow - 1;
	m_nNumCellsPerCol = m_nNumVertsPerCol - 1;
	m_nWidth = m_nNumCellsPerRow * m_nCellSpacing;
	m_nDepth = m_nNumCellsPerCol * m_nCellSpacing;
	m_nNumVertices = m_nNumVertsPerRow * m_nNumVertsPerCol;
	m_nNumTriangles = m_nNumCellsPerRow * m_nNumCellsPerCol * 2;

	m_fHeightScale = fHeightScale;

	if (!ReadRawFile(szHeightMapFileName))
	{
		::MessageBox(0, L"readRawFile - FAILED", 0, 0);
		::PostQuitMessage(0);
	}

	for (int i = 0; i < m_HeightMap.size(); i++)
		m_HeightMap[i] *= m_fHeightScale;

	// compute the vertices
	if (!ComputeVertices())
	{
		::MessageBox(0, L"computeVertices - FAILED", 0, 0);
		::PostQuitMessage(0);
	}

	// compute the indices
	if (!ComputeIndices())
	{
		::MessageBox(0, L"computeIndices - FAILED", 0, 0);
		::PostQuitMessage(0);
	}
}

bool CTerrain::ReadRawFile(std::string szFileName)
{
	std::vector<BYTE> in(m_nNumVertices);

	std::ifstream inFile(szFileName.c_str(), std::ios_base::binary);

	if (!inFile)
		return false;

	inFile.read(
		(char*)&in[0],
		in.size());

	inFile.close();

	m_HeightMap.resize(m_nNumVertices);

	for (int i = 0; i < in.size(); i++)
	{
		m_HeightMap[i] = in[i];
	}

	return true;
}

int CTerrain::GetHeightMapEntry(int nRow, int nCol)
{
	return m_HeightMap[nRow*m_nNumVertsPerRow + nCol];
}

void CTerrain::SetHeightMapEntry(int nRow, int nCol, int nVal)
{
	m_HeightMap[nRow*m_nNumVertsPerRow + nCol] = nVal;
}

bool CTerrain::ComputeVertices()
{
	HRESULT hr = 0;

	hr = m_pdev->CreateVertexBuffer(
		m_nNumVertices*sizeof(TerrainVertex),
		D3DUSAGE_WRITEONLY,
		TerrainVertex::FVF,
		D3DPOOL_MANAGED,
		&m_VB,
		0);

	if (FAILED(hr))
		return false;

	int startX = -m_nWidth / 2;
	int startZ = m_nDepth / 2;

	int endX = m_nWidth / 2;
	int endZ = -m_nDepth / 2;

	float uCoordIncrementSize = 1.0f / (float)m_nNumCellsPerRow;
	float vCoordIncrementSize = 1.0f / (float)m_nNumCellsPerCol;

	TerrainVertex* v = NULL;
	m_VB->Lock(0, 0, (void**)&v, 0);

	int i = 0;

	for (int z = startZ ; z >= endZ; z -= m_nCellSpacing, i++)
	{
		int j = 0;

		for (int x = startX; x <= endX; x += m_nCellSpacing, j++)
		{
			int index = i*m_nNumVertsPerRow + j;

			v[index] = TerrainVertex((float)x, (float)m_HeightMap[index], (float)z,
				(float)j*uCoordIncrementSize, (float)i*vCoordIncrementSize);
		}
	}

	m_VB->Unlock();

	return true;
}

bool CTerrain::ComputeIndices()
{
	HRESULT hr = 0;

	hr = m_pdev->CreateIndexBuffer(
		m_nNumTriangles * 3 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_IB,
		0);

	if (FAILED(hr))
		return false;

	WORD* indicies = 0;

	m_IB->Lock(0, 0, (void**)&indicies, 0);

	int baseIndex = 0;

	for (int i = 0; i < m_nNumCellsPerCol; i++)
	{
		for (int j = 0; j < m_nNumCellsPerRow; j++)
		{
			indicies[baseIndex] = (i*m_nNumVertsPerRow) + j;
			indicies[baseIndex + 1] = (i*m_nNumVertsPerRow) + j + 1;
			indicies[baseIndex + 2] = (i + 1)*m_nNumVertsPerRow + j;

			indicies[baseIndex + 3] = (i + 1)*m_nNumVertsPerRow + j;
			indicies[baseIndex + 4] = (i * m_nNumVertsPerRow) + j + 1;
			indicies[baseIndex + 5] = (i + 1)*m_nNumVertsPerRow + j + 1;

			baseIndex += 6;
		}
	}

	m_IB->Unlock();

	return true;
}

bool CTerrain::LoadTexture(std::string fileName)
{
	HRESULT hr = 0;
	
	hr = D3DXCreateTextureFromFile(
		m_pdev,
		(LPCWSTR)fileName.c_str(),
		&m_pTex);

	if (FAILED(hr))
		return false;

	return true;
}

bool CTerrain::GenTexture(D3DXVECTOR3* dirToLight)
{
	HRESULT hr = 0;

	int nTexWidth = m_nNumCellsPerRow;
	int nTexHeight = m_nNumCellsPerCol;

	hr = D3DXCreateTexture(
		m_pdev,
		nTexWidth, nTexHeight,
		0, 0,
		D3DFMT_X8R8G8B8,
		D3DPOOL_MANAGED,
		&m_pTex);

	if (FAILED(hr))
		return false;

	D3DSURFACE_DESC texDesc;
	m_pTex->GetLevelDesc(0, &texDesc);

	if (texDesc.Format != D3DFMT_X8R8G8B8)
		return false;

	D3DLOCKED_RECT lockedRect;
	m_pTex->LockRect(0, &lockedRect, 0, 0);

	DWORD* imageData = (DWORD*)lockedRect.pBits;

	for (int i = 0; i < nTexHeight; i++)
	{
		for (int j = 0; j < nTexWidth; j++)
		{
			D3DXCOLOR c;

			float height = (float)GetHeightMapEntry(i, j) / m_fHeightScale;

			if ((height) < 42.5f) 		 c = d3d::BEACH_SAND;
			else if ((height) < 85.0f)	 c = d3d::LIGHT_YELLOW_GREEN;
			else if ((height) < 127.5f) c = d3d::PUREGREEN;
			else if ((height) < 170.0f) c = d3d::DARK_YELLOW_GREEN;
			else if ((height) < 212.5f) c = d3d::DARKBROWN;
			else	                     c = d3d::WHITE;

			imageData[i*lockedRect.Pitch / 4 + j] = (D3DCOLOR)c;
		}
	}

	m_pTex->UnlockRect(0);

	if (!LightTerrain(dirToLight))
	{
		::MessageBox(0,L"lightTerrain() - FAILED", 0, 0);
		return false;
	}

	//밉맵 채우기
	hr = D3DXFilterTexture(
		m_pTex,
		0, // default palette
		0, // use top level as source level
		D3DX_DEFAULT); // default filter

	if (FAILED(hr))
	{
		::MessageBox(0, L"D3DXFilterTexture() - FAILED", 0, 0);
		return false;
	}

	return true;
}

float CTerrain::ComputeShade(int nCellRow, int nCellCol, D3DXVECTOR3* dirToLight)
{
	float heightA = GetHeightMapEntry(nCellRow, nCellCol);
	float heightB = GetHeightMapEntry(nCellRow, nCellCol + 1);
	float heightC = GetHeightMapEntry(nCellRow + 1, nCellCol);

	D3DXVECTOR3 u(m_nCellSpacing, heightB - heightA, 0.0f);
	D3DXVECTOR3 v(0.0f, heightC - heightA, -m_nCellSpacing);

	D3DXVECTOR3 n;
	D3DXVec3Cross(&n, &u, &v);
	D3DXVec3Normalize(&n, &n);

	float cosine = D3DXVec3Dot(&n, dirToLight);

	if (cosine < 0.0f)
		cosine = 0.0f;

	return cosine;
}

bool CTerrain::LightTerrain(D3DXVECTOR3* dirToLight)
{
	HRESULT hr = 0;

	D3DSURFACE_DESC textureDesc;

	m_pTex->GetLevelDesc(0, &textureDesc);

	if (textureDesc.Format != D3DFMT_X8R8G8B8)
		return false;

	D3DLOCKED_RECT lockedRect;
	m_pTex->LockRect(
		0, &lockedRect, 0, 0);

	DWORD* pImageData = (DWORD*)lockedRect.pBits;

	for (int i = 0; i < textureDesc.Height; i++)
	{
		for (int j = 0; j < textureDesc.Width; j++)
		{
			int index = i*lockedRect.Pitch / 4 + j;

			D3DXCOLOR c(pImageData[index]);
			c *= ComputeShade(i, j, dirToLight);

			pImageData[index] = (D3DCOLOR)c;
		}
	}

	m_pTex->UnlockRect(0);

	return true;
} 

float CTerrain::GetHeight(float x, float z)
{
	x = ((float)m_nWidth / 2.0f) + x;
	z = ((float)m_nDepth / 2.0f) - z;

	x /= (float)m_nCellSpacing;
	z /= (float)m_nCellSpacing;

	float col = ::floorf(x);
	float row = ::floorf(z);

	float A = GetHeightMapEntry(row, col);
	float B = GetHeightMapEntry(row, col + 1);
	float C = GetHeightMapEntry(row + 1, col);
	float D = GetHeightMapEntry(row + 1, col + 1);

	//셀을 원점으로 이동 시키기
	float dx = x - col;
	float dz = z - row;

	float height = 0.0f;

	if (dz < 1.0f - dx)				//위쪽 삼각형
	{
		float uy = B - A;
		float vy = C - A;
		height = A + d3d::Lerp(0.0f, uy, dx) + d3d::Lerp(0.0f, vy, dz);
	}
	else									//아래쪽 삼각형
	{
		float uy = C - D;
		float vy = B - D;
		height = D + d3d::Lerp(0.0f, uy, 1.0f - dx) + d3d::Lerp(0.0f, vy, 1.0f - dz);
	}

	return height;
}

bool CTerrain::Draw(D3DXMATRIX* world, bool drawTris)
{
	HRESULT hr = 0;

	if (m_pdev)
	{
		m_pdev->SetTransform(D3DTS_WORLD, world);

		m_pdev->SetStreamSource(0, m_VB, 0, sizeof(TerrainVertex));
		m_pdev->SetFVF(TerrainVertex::FVF);
		m_pdev->SetIndices(m_IB);

		m_pdev->SetTexture(0, m_pTex);

		m_pdev->SetRenderState(D3DRS_LIGHTING, false);

		hr = m_pdev->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,
			0, 0, m_nNumVertices, 0, m_nNumTriangles);

		m_pdev->SetRenderState(D3DRS_LIGHTING, true);

		if (drawTris)
		{
			m_pdev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			hr = m_pdev->DrawIndexedPrimitive(
				D3DPT_TRIANGLELIST,
				0, 0, m_nNumVertices, 0, m_nNumTriangles);
			m_pdev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}

		if (FAILED(hr))
			return false;
	}

	return true;
}
