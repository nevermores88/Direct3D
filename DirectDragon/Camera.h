#pragma once
class CCamera
{
public:
	enum CameraType { LANDOBJECT, AIRCRAFT };

private:
	CameraType m_CamType;
	D3DXVECTOR3 m_vRight;
	D3DXVECTOR3 m_vUp;
	D3DXVECTOR3 m_vLook;
	D3DXVECTOR3 m_vPos;

	LPDIRECT3DDEVICE9 m_pdev;

public:
	CCamera();
	~CCamera();

	void Create(CameraType camType = AIRCRAFT, LPDIRECT3DDEVICE9 pdev = NULL);
	void Release();

	void Pitch(float angle);
	void Yaw(float angle);
	void Roll(float angle);

	void Walk(float units);
	void Strafe(float units);
	void Fly(float units);

	void GetViewMatrix(D3DXMATRIX* v);
	void SetCameraType(CameraType camType);
	void GetPosition(D3DXVECTOR3* pos);
	void SetPosition(D3DXVECTOR3* pos);

	void GetRight(D3DXVECTOR3* right);
	void GetUp(D3DXVECTOR3* up);
	void GetLook(D3DXVECTOR3* look);

	void OnUpdate();
};

