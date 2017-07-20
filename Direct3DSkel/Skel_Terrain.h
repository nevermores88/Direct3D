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

	// x, z��ġ�� �������� ����.
	Vertex*	GetVertex(int x, int z) { return (m_pHeightMap + x + z*m_iXDib); }

	// x, z��ġ�� ����(y)���� ����.
	float		GetHeight(int x, int z) { return (GetVertex(x, z))->p.y; }

	// ���̸� BMP������ �����ȼ���
	int			GetCXDIB() { return m_iXDib; }

	// ���̸� BMP������ �����ȼ���
	int			GetCZDIB() { return m_iZDib; }

	// ���̸��� ���� x�� ��(column)��
	float		GetCXTerrain() { return m_iXDib * m_vScale.x; }

	// ���̸��� ���� y�� ����(height)��
	float		GetCYTerrain() { return m_iYDib * m_vScale.y; }

	// ���̸��� ���� z�� ����(row)��
	float		GetCZTerrain() { return m_iZDib * m_vScale.z; }

	// ȭ�鿡 ������ ����Ѵ�.
	HRESULT		Draw(CSkel_ZFrustum* pFrustum);

private:
	// ������ü�� �޸𸮿��� �Ұ��Ѵ�.
	HRESULT		Destroy();

	// ������ü���� ����� �ؽ�ó�� �о���δ�.
	HRESULT		LoadTextures(LPSTR lpTexFilename[MAX_TERRAIN_TEX]);

	// BMP������ ��� ���̸��� �����Ѵ�.
	HRESULT		BuildHeightMap(LPSTR lpFilename);

	// BMP������ ũ�⿡ ���缭 ����Ʈ���� �����Ѵ�.
	HRESULT		BuildQuadTree();

	// ����, �ε��� ���۸� �����Ѵ�.
	HRESULT		CreateVIB();

	// ȭ�鿡 ������ ����Ѵ�.
	HRESULT		Render();
};

