#pragma once
#include "GameCore.h"
#include "Camera.h"
#include "BaseClass.h"

#include "Ex03_Cube.h"
#include "Ex03_Teapot.h"
#include "Ex03_Create.h"

#include "Ex04_Color.h"

#include "Ex05_Light01.h"
#include "Ex05_Directional.h"
#include "Ex05_Point.h"
#include "Ex05_Spot.h"

#include "Ex06_Texture01.h"
#include "Ex06_TexCube.h"
#include "Ex06_AddressModes.h"

#include "Ex07_Blending01.h"
#include	 "Ex07_Blending02.h"

#include "Ex08_Stencil01.h"

#include "Ex09_Text01.h"
#include "Ex09_Text02.h"
#include "Ex09_Text03.h"

#include "Ex10_CreateMesh.h"

#include "Ex11_XFile.h"
#include "Ex11_PMesh.h"
#include "Ex11_BoundingVolumes.h"

#include "Ex12_Camera.h"

#include "Ex13_Terrain.h"

#include "Ex14_Particle.h"

#include "Ex17_Diffuse.h"
#include "Ex17_Toon.h"
#include "Ex17_ToonOutline.h"

#include "Ex18_MultiTexture.h"

#include "Ex19_Effect01.h"
#include "Ex19_Effect02.h"
#include "Ex19_Effect03.h"

#include "Shader_3dapi_01.h"
#include "Shader_3dapi_02.h"
#include "Shader_3dapi_03.h"
#include "Shader_3dapi_05.h"
#include "Shader_3dapi_06.h"
#include "Shader_3dapi_07.h"
#include "Shader_3dapi_08.h"
#include "Shader_3dapi_09.h"
#include "Shader_3dapi_10.h"
#include "Shader_3dapi_11.h"
#include "Shader_3dapi_12.h"
#include "Shader_3dapi_13.h"
#include "Shader_3dapi_14.h"
#include "Shader_3dapi_15.h"
#include "Shader_3dapi_16.h"
#include "Shader_3dapi_17.h"
#include "Shader_3dapi_18.h"
#include "Shader_3dapi_19.h"
#include "Shader_3dapi_19_1.h"
#include "Shader_3dapi_20_1.h"
#include "Shader_3dapi_21_1.h"
#include "Shader_3dapi_22.h"
#include "Shader_3dapi_23.h"
#include "Shader_3dapi_24.h"
#include "Shader_3dapi_25.h"
#include "Shader_3dapi_26.h"
#include "Shader_3dapi_27.h"
#include "Shader_3dapi_28.h"

#include "Temp.h"
#define g_pGame CGame::GetGame()
#define g_pTimeManager g_pGame->GetManager()->GetTimeManager()
#define g_pCamManager g_pGame->GetManager()->GetCameraManager()
#define g_pInputManager g_pGame->GetManager()->GetInputManager()

class CGame : public CGameCore
{
	static CGame* m_pGame;
	DWORD			m_dwExType;

	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
	D3DXVECTOR3 m_Eye, m_At, m_Up;

	CEx03_Cube		m_Ex03_Cube;
	CEx03_Teapot		m_Ex03_Teapot;
	CEx03_Create		m_Ex03_Create;

	CEx04_Color		m_Ex04_Color;

	CEx05_Light01	m_Ex05_Light01;
	CEx05_Directional	m_Ex05_Directional;
	CEx05_Point		m_Ex05_Point;
	CEx05_Spot		m_Ex05_Spot;

	CEx06_Texture01 m_Ex06_Texture01;
	CEx06_TexCube	m_Ex06_TexCube;
	CEx06_AddressModes	m_Ex06_AddressModes;

	CEx07_Blending01 m_Ex07_Blending01;
	CEx07_Blending02 m_Ex07_Blending02;

	CEx08_Stencil01	m_Ex08_Stencil01;

	CEx09_Text01		m_Ex09_Text01;
	CEx09_Text02		m_Ex09_Text02;
	CEx09_Text03		m_Ex09_Text03;

	CEx10_CreateMesh	m_Ex10_CreateMesh;

	CEx11_XFile		m_Ex11_XFile;
	CEx11_PMesh		m_Ex11_PMesh;
	CEx11_BoundingVolumes	m_Ex11_BoundingVolumes;

	CEx12_Camera	m_Ex12_Camera;

	CEx13_Terrain		m_Ex13_Terrain;

	CEx14_Particle	m_Ex14_Particle;

	CEx17_Diffuse		m_Ex17_Diffuse;
	CEx17_Toon		m_Ex17_Toon;

	CEx18_MultiTexture	m_Ex18_MultiTexture;

	CShader_3dapi_01	m_Shader_3dapi_01;
	CShader_3dapi_02	m_Shader_3dapi_02;
	CShader_3dapi_03	m_Shader_3dapi_03;
	CShader_3dapi_05	m_Shader_3dapi_05;
	CShader_3dapi_06	m_Shader_3dapi_06;
	CShader_3dapi_07	m_Shader_3dapi_07;
	CShader_3dapi_08	m_Shader_3dapi_08;
	CShader_3dapi_09	m_Shader_3dapi_09;
	CShader_3dapi_10	m_Shader_3dapi_10;
	CShader_3dapi_11	m_Shader_3dapi_11;
	CShader_3dapi_12	m_Shader_3dapi_12;
	CShader_3dapi_13	m_Shader_3dapi_13;
	CShader_3dapi_14	m_Shader_3dapi_14;
	CShader_3dapi_15	m_Shader_3dapi_15;
	CShader_3dapi_16	m_Shader_3dapi_16;
	CShader_3dapi_17	m_Shader_3dapi_17;
	CShader_3dapi_18	m_Shader_3dapi_18;

	CTemp	m_Temp;

	CBaseClass* m_pClasses;

protected:
	virtual void OnInit();
	virtual void OnRender();
	virtual void OnUpdate();
	virtual void OnRelease();

public:
	CGame();
	~CGame();

	static CGame* GetGame();
	D3DXMATRIX GetViewMatrix(){ return m_matView; };
	D3DXMATRIX GetProjMatrix(){ return m_matProj; };

};

