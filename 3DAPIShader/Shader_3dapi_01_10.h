//---------------------------------------------------------------------------------------------------
//
//Description : Texture 03
//Original File : s0v_06_tex_env.zip
//
//---------------------------------------------------------------------------------------------------

#pragma once
class CShader_3dapi_01_10 : public CBaseClass
{
public:
	CShader_3dapi_01_10();
	~CShader_3dapi_01_10();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

