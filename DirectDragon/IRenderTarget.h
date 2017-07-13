#pragma once
class IRenderTarget
{
public:
	IRenderTarget();
	virtual ~IRenderTarget();

	virtual	INT		Create(void* p1 = NULL, void* p2 = NULL, void* p3 = NULL, void* p4 = NULL) = 0;
	virtual	void	Destroy() = 0;

	virtual	INT		OnResetDevice() = 0;
	virtual	INT		OnLostDevice() = 0;

	//	D3DCLEAR_TARGET	=0x00000001L;
	//	D3DCLEAR_ZBUFFER=0x00000002L;
	//	D3DCLEAR_STENCIL=0x00000004l;
	virtual	INT		BeginScene(DWORD dClearMode = (0x1L | 0x2L | 0x4L), DWORD dClearColor = 0xFF006699) = 0;
	virtual	INT		EndScene() = 0;

	virtual	INT		GetWidth() = 0;				// Get Image Width
	virtual	INT		GetHeight() = 0;				// Get Image Height
	virtual	DWORD	GetDepth() = 0;				// Get Image Depth

	virtual	void*	GetTexture() const = 0;		// Return Texture pointer
	virtual	void*	GetSurface() const = 0;		// Retrurn Surface pointer in Texture.
};

INT LcD3D_CreateRenderTarget(char* sCmd, IRenderTarget** pData, void* pDevice, INT iWidth = -1, INT iHeight = -1);