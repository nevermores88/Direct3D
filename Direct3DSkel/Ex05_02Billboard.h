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
	D3DXVECTOR3		m_vEye;			/// ī�޶��� ���� ��ġ
	D3DXVECTOR3		m_vLookat;		/// ī�޶��� �ü� ��ġ
	D3DXVECTOR3		m_vUp;			/// ī�޶��� ��溤��

	D3DXVECTOR3		m_vView;		/// ī�޶� ���ϴ� �������⺤��
	D3DXVECTOR3		m_vCross;		/// ī������ ���麤�� cross( view, up )

	D3DXMATRIXA16	m_matView;		/// ī�޶� ���
	D3DXMATRIXA16	m_matBill;		/// ������ ���(ī�޶��� �����)
public:

	/// ������
	CEx05_02Camera();

	/// ī�޶� ����� ����.
	D3DXMATRIXA16*	GetViewMatrix() { return &m_matView; }

	/// ������ ����� ����.
	D3DXMATRIXA16*	GetBillMatrix() { return &m_matBill; }

public:

	/// ī�޶� ����� �����ϱ����� �⺻ ���Ͱ����� �����Ѵ�.
	D3DXMATRIXA16*	SetView(D3DXVECTOR3* pvEye, D3DXVECTOR3* pvLookat, D3DXVECTOR3* pvUp);

	/// ī�޶��� ��ġ���� �����Ѵ�.
	void			SetEye(D3DXVECTOR3* pv) { m_vEye = *pv; }

	/// ī�޶��� ��ġ���� ����.
	D3DXVECTOR3*	GetEye() { return &m_vEye; }

	/// ī�޶��� �ü����� �����Ѵ�.
	void			SetLookat(D3DXVECTOR3* pv) { m_vLookat = *pv; }

	/// ī�޶��� �ü����� ����.
	D3DXVECTOR3*	GetLookat() { return &m_vLookat; }

	/// ī�޶��� ��溤�Ͱ��� �����Ѵ�.
	void			SetUp(D3DXVECTOR3* pv) { m_vUp = *pv; }

	/// ī�޶��� ��溤�Ͱ��� ����.
	D3DXVECTOR3*	GetUp() { return &m_vUp; }

	/// ���� �����Ѵ�.
	void			Flush() { SetView(&m_vEye, &m_vLookat, &m_vUp); }

	/// ī�޶� ��ǥ���� X������ angle��ŭ ȸ���Ѵ�.
	D3DXMATRIXA16*	RotateLocalX(float angle);

	/// ī�޶� ��ǥ���� Y������ angle��ŭ ȸ���Ѵ�.
	D3DXMATRIXA16*	RotateLocalY(float angle);

	//	D3DXMATRIXA16*	RotateLocalZ( float angle );

	/// ������ǥ���� *pv���� ��ġ�� �̵��Ѵ�.
	D3DXMATRIXA16*	MoveTo(D3DXVECTOR3* pv);

	/// ī�޶� ��ǥ���� X��������� dist��ŭ �����Ѵ�.(������ -dist�� ������ �ȴ�.)
	D3DXMATRIXA16*	MoveLocalX(float dist);

	/// ī�޶� ��ǥ���� Y��������� dist��ŭ �����Ѵ�.(������ -dist�� ������ �ȴ�.)
	D3DXMATRIXA16*	MoveLocalY(float dist);

	/// ī�޶� ��ǥ���� Z��������� dist��ŭ �����Ѵ�.(������ -dist�� ������ �ȴ�.)
	D3DXMATRIXA16*	MoveLocalZ(float dist);
};