#pragma once
class CEx02_RedShader : public CBaseClass
{
	LPD3DXMESH	m_pSphere;
	LPD3DXEFFECT m_pShader;   //�����Ͼ��ص� �Ǵ� ���̴� �ڵ��ε�

public:
	CEx02_RedShader();
	~CEx02_RedShader();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

