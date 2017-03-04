#pragma once
class CD3DUtility
{
//private:
//	static CD3DUtility* m_pThis;

private:
	CD3DUtility();
	~CD3DUtility();
public:
	//static CD3DUtility* GetThis();

	static LPD3DXEFFECT LoadShader(LPDIRECT3DDEVICE9 pdev, const char* szFileName);
	static LPD3DXMESH LoadModel(LPDIRECT3DDEVICE9 pdev, const char* szFileName);
	static LPDIRECT3DTEXTURE9 LoadTexture(LPDIRECT3DDEVICE9 pdev, const char* szFileName);
};

//inline CD3DUtility* GetD3DUtility() { return CD3DUtility::GetThis(); }

