#pragma once

class CEx03_Cube : public CBaseClass
{
private:
	struct Vertex
	{
		float x, y, z;
		//static DWORD FVF;

		Vertex(){}
		Vertex(float _x, float _y, float _z)
		{
			x = _x; y = _y; z = _z;
			//FVF = D3DFVF_XYZ;
		}

		enum { FVF = (D3DFVF_XYZ) };
	};

	//const DWORD Vertex::FVF = D3DFVF_XYZ;

	LPDIRECT3DVERTEXBUFFER9	VB;
	LPDIRECT3DINDEXBUFFER9		IB;
public:
	CEx03_Cube();
	~CEx03_Cube();

	void Create(LPDIRECT3DDEVICE9 pdev, DWORD dwExType);
	void Release();

	void OnRender();
	void OnUpdate();

};

