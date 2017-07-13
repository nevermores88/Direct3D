#pragma once
class CEx08_Stencil01 : public CBaseClass
{
	struct Vertex
	{
		float x, y, z;
		float nx, ny, nz;
		float u, v;

		Vertex(){}
		Vertex(float _x, float _y, float _z, float _nx, float _ny, float _nz, float _u, float _v)
		{
			x = _x; y = _y; z = _z;
			nx = _nx; ny = _ny; nz = _nz;
			u = _u; v = _v;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1) };
	};

	LPDIRECT3DVERTEXBUFFER9	m_pVB;

	LPDIRECT3DTEXTURE9				m_pFloorTex;
	LPDIRECT3DTEXTURE9				m_pWallTex;
	LPDIRECT3DTEXTURE9				m_pMirrorTex;

	D3DMATERIAL9						m_FloorMtrl;
	D3DMATERIAL9						m_WallMtrl;
	D3DMATERIAL9						m_MirrorMtrl;

	LPD3DXMESH							m_pTeapot;
	D3DXVECTOR3						m_vTeapotPos;
	D3DMATERIAL9						m_TeapotMtrl;

	D3DLIGHT9							m_Light;
public:
	CEx08_Stencil01();
	~CEx08_Stencil01();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();

	void DataClear();

	void RenderMirror();
	void RenderShadow();
};

