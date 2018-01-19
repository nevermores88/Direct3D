//---------------------------------------------------------------------------------------------------
//
//Description : Texture 04
//Original File : s0v_06_tex_vtx.zip
//
//---------------------------------------------------------------------------------------------------

#pragma once
class CShader_3dapi_01_11 : public CBaseClass
{
public:
	CShader_3dapi_01_11();
	~CShader_3dapi_01_11();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

