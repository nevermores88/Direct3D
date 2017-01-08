#pragma once

#define g_pCamera CCameraManager::GetInstance()

class CCameraManager
{
private:
	static CCameraManager* m_pCameraManager;

	LPDIRECT3DDEVICE9 m_pdev;
//public:
	D3DXMATRIX m_mtView;
	D3DXMATRIX m_mtProj;

	D3DXVECTOR3 m_vEye;
	D3DXVECTOR3 m_vLook;
	D3DXVECTOR3 m_vUp;

	float m_fScreenWidth;
	float m_fScreenHeight;
	float m_fFov;
	float m_fNear;
	float m_fFar;

	float m_fYaw;
	float m_fPitch;

public:
	CCameraManager();
	~CCameraManager();

	void Create(LPDIRECT3DDEVICE9 pdev);
	void Release();
	void DataClear();

	void Update();

	static CCameraManager* GetInstance();

	void SetEye(D3DXVECTOR3 vEye);
	void SetLook(D3DXVECTOR3 vLook);
	void SetUp(D3DXVECTOR3 vUp);

	void SetProjection(float fScreenWidth, float fScreenHeight, float fFov, float fNear, float fFar);

	D3DXVECTOR3 GetEye();
	D3DXVECTOR3 GetLook();
	D3DXVECTOR3 GetUp();

	void MoveSide(float fSpeed);
	void MoveForward(float fSpeed, float fY = 0);
	void Rotation(D3DXVECTOR3 vDelta);

	void SetTransfrom();
};

