#pragma once
class CEx05_06Animation : public CBaseClass
{
	struct Vertex
	{
		float x, y, z;
		D3DCOLOR color;

		Vertex(){}
		Vertex(float _x, float _y, float _z, D3DCOLOR _color)
		{
			x = _x; y = _y; z = _z;
			color = _color;
		}

		enum { FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE) };
	};

	struct Index
	{
		WORD _0, _1, _2;
	};

	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;

	D3DXMATRIX m_mtTMParent;	//�θ��� TM
	D3DXMATRIX m_mtRParent;		//�θ��� ȸ�����

	D3DXMATRIX m_mtTMChild;		//�ڽ��� TM
	D3DXMATRIX m_mtRChild;		//�ڽ��� ȸ�����

	float m_fRot;

	/// �ִϸ��̼� Ű���� �����ϱ����� �迭
	D3DXVECTOR3		m_vAniPos[2];		/// ��ġ(position)Ű ��
	D3DXQUATERNION	m_qAniRot[2];		/// ȸ��(quaternion)Ű ��

public:
	CEx05_06Animation();
	~CEx05_06Animation();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();

	void InitAnimation();
	void DrawMesh(D3DXMATRIX* pMat);
};

