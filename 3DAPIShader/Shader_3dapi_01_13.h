//---------------------------------------------------------------------------------------------------
//
//Description :  Lighting 02
//Original File : s0v_07_2phong.zip
//
//---------------------------------------------------------------------------------------------------

#pragma once
class CShader_3dapi_01_13 : public CBaseClass
{
public:
	CShader_3dapi_01_13();
	~CShader_3dapi_01_13();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

