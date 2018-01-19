//---------------------------------------------------------------------------------------------------
//
//Description : Texture 02
//Original File : s0v_06_tex_earth.zip
//
//---------------------------------------------------------------------------------------------------

#pragma once
class CShader_3dapi_01_09 : public CBaseClass
{
public:
	CShader_3dapi_01_09();
	~CShader_3dapi_01_09();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

