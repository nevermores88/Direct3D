#include "stdafx.h"
#include "d3dUtility.h"


D3DMATERIAL9 d3d::InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient = a;
	mtrl.Diffuse = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power = p;

	return mtrl;
}

D3DLIGHT9 d3d::InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = *color * 0.6f;
	light.Diffuse = *color;
	light.Specular = *color * 0.6f;
	light.Direction = *direction;

	return light;
}

D3DLIGHT9 d3d::InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_POINT;
	light.Ambient = *color * 0.6f;
	light.Diffuse = *color;
	light.Specular = *color * 0.6f;
	light.Position = *position;
	light.Range = 1000.0f;
	light.Falloff = 1.0f;
	light.Attenuation0 = 1.0f;
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 0.0f;

	return light;
}

D3DLIGHT9 d3d::InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type = D3DLIGHT_SPOT;
	light.Ambient = *color * 0.0f;
	light.Diffuse = *color;
	light.Specular = *color * 0.6f;
	light.Position = *position;
	light.Direction = *direction;
	light.Range = 1000.0f;
	light.Falloff = 1.0f;
	light.Attenuation0 = 1.0f;
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 0.0f;
	light.Theta = 0.4f;
	light.Phi = 0.9f;

	return light;
}

void d3d::DumpVertices(std::ofstream& outFile, LPD3DXMESH pMesh)
{
	outFile << "Vertices:" << endl;
	outFile << "----------" << endl << endl;

	Vertex* v = 0;
	pMesh->LockVertexBuffer(0, (void**)&v);

	for (int i = 0; i < pMesh->GetNumVertices(); i++)
	{
		outFile << "Vertex " << i << ": (";
		outFile << v[i].x << ", " << v[i].y << ", " << v[i].z << ", ";
		outFile << v[i].nx << ", " << v[i].ny << ", " << v[i].nz << ", ";
		outFile << v[i].u << ", " << v[i].v << ")" << endl;
	}

	pMesh->UnlockVertexBuffer();

	outFile << std::endl << std::endl;
}

void d3d::DumpIndices(std::ofstream& outFile, LPD3DXMESH pMesh)
{
	outFile << "Indicies:" << endl;
	outFile << "----------" << endl << endl;

	WORD* indicies = 0;
	pMesh->LockIndexBuffer(0, (void**)&indicies);

	for (int i = 0; i < pMesh->GetNumFaces(); i++)
	{
		outFile << "Triangle " << i << ": ";
		outFile << indicies[i * 3] << " ";
		outFile << indicies[i * 3 + 1] << " ";
		outFile << indicies[i * 3 + 2] << endl;
	}

	pMesh->UnlockIndexBuffer();

	outFile << endl << endl;
}

void d3d::DumpAttributeBuffer(std::ofstream& outFile, LPD3DXMESH pMesh)
{
	outFile << "Attribute Buffer:" << endl;
	outFile << "-------------------" << endl << endl;

	DWORD* attributeBuffer = 0;
	pMesh->LockAttributeBuffer(0, &attributeBuffer);

	for (int i = 0; i < pMesh->GetNumFaces(); i++)
	{
		outFile << "Triangle lives in subset " << i << ": ";
		outFile << attributeBuffer[i] << endl;
	}

	pMesh->UnlockAttributeBuffer();

	outFile << endl << endl;
}

void d3d::dumpAdjacencyBuffer(std::ofstream& outFile, LPD3DXMESH pMesh)
{
	outFile << "Adjacency Buffer:" << endl;
	outFile << "-----------------" << endl << endl;

	std::vector<DWORD> adjacencyBuffer(pMesh->GetNumFaces() * 3);

	pMesh->GenerateAdjacency(0.0f, &adjacencyBuffer[0]);

	for (int i = 0; i < pMesh->GetNumFaces(); i++)
	{
		outFile << "Triangle's adjacent to triangle " << i << ": ";
		outFile << adjacencyBuffer[i * 3] << " ";
		outFile << adjacencyBuffer[i * 3 + 1] << " ";
		outFile << adjacencyBuffer[i * 3 + 2] <<endl;
	}

	outFile << endl << endl;
}

void d3d::DumpAttributeTable(std::ofstream& outFile, LPD3DXMESH pMesh)
{
	outFile << "Attribute Table:" << endl;
	outFile << "----------------" << endl << endl;

	// number of entries in the attribute table
	DWORD numEntries = 0;

	pMesh->GetAttributeTable(0, &numEntries);

	std::vector<D3DXATTRIBUTERANGE> table(numEntries);

	pMesh->GetAttributeTable(&table[0], &numEntries);

	for (int i = 0; i < numEntries; i++)
	{
		outFile << "Entry " << i << endl;
		outFile << "-----------" << endl;

		outFile << "Subset ID:    " << table[i].AttribId << endl;
		outFile << "Face Start:   " << table[i].FaceStart << endl;
		outFile << "Face Count:   " << table[i].FaceCount << endl;
		outFile << "Vertex Start: " << table[i].VertexStart << endl;
		outFile << "Vertex Count: " << table[i].VertexCount << endl;
		outFile << endl;
	}

	outFile << endl << endl;
}

float d3d::Lerp(float a, float b, float t)
{
	return a - (a*t) + (b*t);
}

DWORD d3d::FtoDw(float f)
{
	return *((DWORD*)&f);
}

float	d3d::GetRandomFloat(float fLowBound, float fHighBound)
{
	if (fLowBound >= fHighBound)
		return fLowBound;

	float f = (rand() % 10000) * 0.0001f;

	return (f*(fHighBound - fLowBound)) + fLowBound;
}

void	d3d::GetRandomVector(D3DXVECTOR3* pOut, D3DXVECTOR3* pMin, D3DXVECTOR3* pMax)
{
	pOut->x = GetRandomFloat(pMin->x, pMax->x);
	pOut->y = GetRandomFloat(pMin->y, pMax->y);
	pOut->z = GetRandomFloat(pMin->z, pMax->z);
}