#pragma once

//#include "Skel_QuadTree.h"

class CSkel_QuadTree;
class CSkel_ZFrustum;

#define MAX_TERRAIN_TEX 4
#define _USE_INDEX16

class CSkel_Terrain
{
public:
	struct Vertex
	{
		D3DXVECTOR3 p;
		D3DXVECTOR3 n;
		D3DXVECTOR2 t;

		Vertex(){}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1) };
	};

private:

	struct Index
	{
#ifdef _USE_INDEX16
		WORD _0, _1, _2;
#else
		DWORD _0, _1, _2;
#endif
	};

	LPDIRECT3DDEVICE9	m_pdev;

	int m_iXDib;
	int m_iZDib;
	int m_iYDib;

	D3DXVECTOR3 m_vScale;
	Vertex*	m_pHeightMap;
	
	LPDIRECT3DTEXTURE9		m_pTex[MAX_TERRAIN_TEX];
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;

	int m_iTriangles;
	CSkel_QuadTree*		m_pQuadTree;

public:
	CSkel_Terrain();
	~CSkel_Terrain();

	HRESULT Create(LPDIRECT3DDEVICE9 pdev, D3DXVECTOR3* pScaleVector, LPSTR lpBMPFilename, LPSTR lpTexFilename[MAX_TERRAIN_TEX]);

	// x, z위치의 정점값을 얻어낸다.
	Vertex*	GetVertex(int x, int z) { return (m_pHeightMap + x + z*m_iXDib); }

	// x, z위치의 높이(y)값만 얻어내다.
	float		GetHeight(int x, int z) { return (GetVertex(x, z))->p.y; }

	// 높이맵 BMP파일의 가로픽셀수
	int			GetCXDIB() { return m_iXDib; }

	// 높이맵 BMP파일의 가로픽셀수
	int			GetCZDIB() { return m_iZDib; }

	// 높이맵의 실제 x축 폭(column)값
	float		GetCXTerrain() { return m_iXDib * m_vScale.x; }

	// 높이맵의 실제 y축 높이(height)값
	float		GetCYTerrain() { return m_iYDib * m_vScale.y; }

	// 높이맵의 실제 z축 길이(row)값
	float		GetCZTerrain() { return m_iZDib * m_vScale.z; }

	// 화면에 지형을 출력한다.
	HRESULT		Draw(CSkel_ZFrustum* pFrustum);

private:
	// 지형객체를 메모리에서 소거한다.
	HRESULT		Destroy();

	// 지형객체에서 사용할 텍스처를 읽어들인다.
	HRESULT		LoadTextures(LPSTR lpTexFilename[MAX_TERRAIN_TEX]);

	// BMP파일을 열어서 높이맵을 생성한다.
	HRESULT		BuildHeightMap(LPSTR lpFilename);

	// BMP파일의 크기에 맞춰서 쿼드트리를 생성한다.
	HRESULT		BuildQuadTree();

	// 정점, 인덱스 버퍼를 생성한다.
	HRESULT		CreateVIB();

	// 화면에 지형을 출력한다.
	HRESULT		Render();
};

