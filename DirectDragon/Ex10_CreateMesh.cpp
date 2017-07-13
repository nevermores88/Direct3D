#include "stdafx.h"
#include "Ex10_CreateMesh.h"


CEx10_CreateMesh::CEx10_CreateMesh()
{
}


CEx10_CreateMesh::~CEx10_CreateMesh()
{
}

void CEx10_CreateMesh::Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType)
{
	m_dwExType = dwExType;
	m_pdev = pdev;
	m_dwNumSubsets = 3;
	memset(m_pTextures, 0, sizeof(m_pTextures));
	m_fYRot = 0.0f;

	HRESULT hr = 0;

	hr = D3DXCreateMeshFVF(12, 24, D3DXMESH_MANAGED, Vertex::FVF, pdev, &m_pMesh);

	Vertex* v = 0;
	m_pMesh->LockVertexBuffer(0, (void**)&v);

	// fill in the front face vertex data
	v[0] = Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[1] = Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[2] = Vertex(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
	v[3] = Vertex(1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	// fill in the back face vertex data
	v[4] = Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[5] = Vertex(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[6] = Vertex(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	v[7] = Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);

	// fill in the top face vertex data
	v[8] = Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[9] = Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[10] = Vertex(1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	v[11] = Vertex(1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

	// fill in the bottom face vertex data
	v[12] = Vertex(-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	v[13] = Vertex(1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
	v[14] = Vertex(1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
	v[15] = Vertex(-1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

	// fill in the left face vertex data
	v[16] = Vertex(-1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[17] = Vertex(-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[18] = Vertex(-1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[19] = Vertex(-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// fill in the right face vertex data
	v[20] = Vertex(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[21] = Vertex(1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[22] = Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[23] = Vertex(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	m_pMesh->UnlockVertexBuffer();

	WORD* i = 0;
	m_pMesh->LockIndexBuffer(0, (void**)&i);

	// fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// fill in the back face index data
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// fill in the top face index data
	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	m_pMesh->UnlockIndexBuffer();

	DWORD* dwAttrBuffer = 0;
	m_pMesh->LockAttributeBuffer(0, &dwAttrBuffer);

	for (int i = 0; i< 4; i++)
		dwAttrBuffer[i] = 0;

	for (int i = 4; i < 8; i++)
		dwAttrBuffer[i] = 1;

	for (int i = 8; i < 12; i++)
		dwAttrBuffer[i] = 2;

	m_pMesh->UnlockAttributeBuffer();

	//메쉬 최적화
	std::vector<DWORD> vAdjBuffer(m_pMesh->GetNumFaces() * 3);
	m_pMesh->GenerateAdjacency(0.0f, &vAdjBuffer[0]);

	hr = m_pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		&vAdjBuffer[0], 0, 0, 0);

	D3DXCreateTextureFromFile(
		m_pdev,
		L"Ex10/brick0.jpg",
		&m_pTextures[0]);

	D3DXCreateTextureFromFile(
		m_pdev,
		L"Ex10/brick1.jpg",
		&m_pTextures[1]);

	D3DXCreateTextureFromFile(
		m_pdev,
		L"Ex10/checker.jpg",
		&m_pTextures[2]);

	m_OutFile.open("Ex10/Mesh Dump.txt");

	d3d::DumpVertices(m_OutFile, m_pMesh);
	d3d::DumpIndices(m_OutFile, m_pMesh);
	d3d::DumpAttributeTable(m_OutFile, m_pMesh);
	d3d::DumpAttributeBuffer(m_OutFile, m_pMesh);
	d3d::dumpAdjacencyBuffer(m_OutFile, m_pMesh);

	m_OutFile.close();
}

void CEx10_CreateMesh::Release()
{

}

void CEx10_CreateMesh::OnRender()
{
	if (m_pdev)
	{
		m_pdev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pdev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

		m_pdev->SetRenderState(D3DRS_LIGHTING, false);

		m_pdev->SetTransform(D3DTS_WORLD, &m_mtWorld);

		for (int i = 0; i < m_dwNumSubsets; i++)
		{
			m_pdev->SetTexture(0, m_pTextures[i]);
			m_pMesh->DrawSubset(i);
		}
	}
}

void CEx10_CreateMesh::OnUpdate()
{
	if (m_pdev)
	{
		D3DXMATRIX xRot;
		D3DXMatrixRotationX(&xRot, D3DX_PI*0.2f);

		D3DXMATRIX yRot;
		D3DXMatrixRotationY(&yRot, m_fYRot);
		m_fYRot += g_pGame->GetManager()->GetTimeManager()->GetTimeDelta();

		if (m_fYRot >= 6.28f)
			m_fYRot = 0.0f;

		m_mtWorld = xRot * yRot;
	}
}
