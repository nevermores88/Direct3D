#pragma once
class CTerrain
{
private:

	struct TerrainVertex
	{
		float x, y, z;
		float u, v;

		TerrainVertex(){}
		TerrainVertex(float _x, float _y, float _z, float _u, float _v)
		{
			x = _x, y = _y, z = _z;
			u = _u, v = _v;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_TEX1) };
	};


	LPDIRECT3DDEVICE9	m_pdev;
	LPDIRECT3DTEXTURE9		m_pTex;
	LPDIRECT3DVERTEXBUFFER9	m_VB;
	LPDIRECT3DINDEXBUFFER9		m_IB;

	int m_nNumVertsPerRow;
	int m_nNumVertsPerCol;
	int m_nCellSpacing;

	int m_nNumCellsPerRow;
	int m_nNumCellsPerCol;
	int m_nWidth;
	int m_nDepth;
	int m_nNumVertices;
	int m_nNumTriangles;

	float m_fHeightScale;

	std::vector<int> m_HeightMap;

private:
	bool ReadRawFile(std::string szFileName);
	bool ComputeVertices();
	bool ComputeIndices();


public:
	CTerrain();
	~CTerrain();

	void Create(LPDIRECT3DDEVICE9 pDev, std::string szHeightMapFileName, int nNumVertsPerRow,
		int nNumVertsPerCol, int nCellSpacing, float fHeightScale);

	int GetHeightMapEntry(int nRow, int nCol);
	void SetHeightMapEntry(int nRow, int nCol, int nVal);
	
	bool LoadTexture(std::string fileName);

	bool GenTexture(D3DXVECTOR3* dirToLight);
	float ComputeShade(int nCellRow, int nCellCol, D3DXVECTOR3* dirToLight);
	bool	LightTerrain(D3DXVECTOR3* dirToLight);

	float GetHeight(float x, float z);

	bool Draw(D3DXMATRIX* world, bool drawTris);
};

