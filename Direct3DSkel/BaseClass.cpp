#include "stdafx.h"
#include "BaseClass.h"


CBaseClass::CBaseClass()
{
}


CBaseClass::~CBaseClass()
{
}

HRESULT CBaseClass::Create(LPDIRECT3DDEVICE9 pdev)
{
	m_pdev = pdev;

	return S_OK;
}
