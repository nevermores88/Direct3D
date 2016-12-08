#pragma once

extern class CEx05_02Water;
extern class CEx05_02Camera;

class CEx05_02Billboard : public CBaseClass
{
	struct Vertex
	{
		float x, y, z;
		D3DCOLOR color;
		float u, v;

		Vertex(){}
		Vertex(float _x, float _y, float _z, D3DCOLOR _color, float _u, float _v)
		{
			x = _x; y = _y; z = _z;
			color = _color;
			u = _u; v = _v;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1) };
	};

	CEx05_02Water* m_pWater;
	CEx05_02Camera* m_pCam;

	LPDIRECT3DTEXTURE9		m_pTexBillboard[4];

	D3DXMATRIX m_mtAni;
	D3DXMATRIX m_mtWorld;
	D3DXMATRIX m_mtView;
	D3DXMATRIX m_mtProj;

	DWORD m_dwMouseX;
	DWORD m_dwMouseY;
	BOOL m_bBillboard;
	BOOL	 m_bWireframe;

	DWORD m_dwMousePos[2];

public:
	CEx05_02Billboard();
	~CEx05_02Billboard();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

class CEx05_02Water : public CBaseClass
{
	struct Vertex
	{
		float x, y, z;
		D3DCOLOR color;
		float u, v;

		Vertex(){}
		Vertex(float _x, float _y, float _z, D3DCOLOR _color, float _u, float _v)
		{
			x = _x; y = _y; z = _z;
			color = _color;
			u = _u; v = _v;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1) };
	};

	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;
	LPDIRECT3DTEXTURE9				m_pTex[2];

	int m_nX;
	int m_nZ;
	int m_nWaveTick;
	int m_nPrevTick;
	float m_fWave;
	float m_fDir;

	HRESULT LoadTexture();
	HRESULT CreateVIB();
	HRESULT WaveTexture();

public:
	CEx05_02Water();
	~CEx05_02Water();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev, int nX, int nZ, int nSpeed);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

class CEx05_02Camera
{
	D3DXVECTOR3		m_vEye;			/// 카메라의 현재 위치
	D3DXVECTOR3		m_vLookat;		/// 카메라의 시선 위치
	D3DXVECTOR3		m_vUp;			/// 카메라의 상방벡터

	D3DXVECTOR3		m_vView;		/// 카메라가 향하는 단위방향벡터
	D3DXVECTOR3		m_vCross;		/// 카마레의 측면벡터 cross( view, up )

	D3DXMATRIXA16	m_matView;		/// 카메라 행렬
	D3DXMATRIXA16	m_matBill;		/// 빌보드 행렬(카메라의 역행렬)
public:

	/// 생성자
	CEx05_02Camera();

	/// 카메라 행렬을 얻어낸다.
	D3DXMATRIXA16*	GetViewMatrix() { return &m_matView; }

	/// 빌보드 행렬을 얻어낸다.
	D3DXMATRIXA16*	GetBillMatrix() { return &m_matBill; }

public:

	/// 카메라 행렬을 생성하기위한 기본 벡터값들을 설정한다.
	D3DXMATRIXA16*	SetView(D3DXVECTOR3* pvEye, D3DXVECTOR3* pvLookat, D3DXVECTOR3* pvUp);

	/// 카메라의 위치값을 설정한다.
	void			SetEye(D3DXVECTOR3* pv) { m_vEye = *pv; }

	/// 카메라의 위치값을 얻어낸다.
	D3DXVECTOR3*	GetEye() { return &m_vEye; }

	/// 카메라의 시선값을 설정한다.
	void			SetLookat(D3DXVECTOR3* pv) { m_vLookat = *pv; }

	/// 카메라의 시선값을 얻어낸다.
	D3DXVECTOR3*	GetLookat() { return &m_vLookat; }

	/// 카메라의 상방벡터값을 설정한다.
	void			SetUp(D3DXVECTOR3* pv) { m_vUp = *pv; }

	/// 카메라의 상방벡터값을 얻어낸다.
	D3DXVECTOR3*	GetUp() { return &m_vUp; }

	/// 값을 갱신한다.
	void			Flush() { SetView(&m_vEye, &m_vLookat, &m_vUp); }

	/// 카메라 좌표계의 X축으로 angle만큼 회전한다.
	D3DXMATRIXA16*	RotateLocalX(float angle);

	/// 카메라 좌표계의 Y축으로 angle만큼 회전한다.
	D3DXMATRIXA16*	RotateLocalY(float angle);

	//	D3DXMATRIXA16*	RotateLocalZ( float angle );

	/// 월드좌표계의 *pv값의 위치로 이동한다.
	D3DXMATRIXA16*	MoveTo(D3DXVECTOR3* pv);

	/// 카메라 좌표계의 X축방향으로 dist만큼 전진한다.(후진은 -dist를 넣으면 된다.)
	D3DXMATRIXA16*	MoveLocalX(float dist);

	/// 카메라 좌표계의 Y축방향으로 dist만큼 전진한다.(후진은 -dist를 넣으면 된다.)
	D3DXMATRIXA16*	MoveLocalY(float dist);

	/// 카메라 좌표계의 Z축방향으로 dist만큼 전진한다.(후진은 -dist를 넣으면 된다.)
	D3DXMATRIXA16*	MoveLocalZ(float dist);
};