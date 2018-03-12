//---------------------------------------------------------------------------------------------------
//
//Description :  Fog 02
//Original File : s0v_09_fog2_height.zip
//
//---------------------------------------------------------------------------------------------------

class CFiled_01_22;

#pragma once
class CShader_3dapi_01_22 : public CBaseClass
{
	LPDIRECT3DVERTEXSHADER9	m_pVertexShader;
	LPDIRECT3DVERTEXDECLARATION9	m_pFVF;

	CFiled_01_22* m_pField;

public:
	CShader_3dapi_01_22();
	~CShader_3dapi_01_22();

	virtual HRESULT Create(LPDIRECT3DDEVICE9 pdev);
	virtual void Release();

	virtual void Render();
	virtual void Update();
};

