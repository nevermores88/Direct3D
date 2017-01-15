#pragma once
class IRenderTarget
{
public:
	IRenderTarget();
	~IRenderTarget();

	virtual HRESULT Create(void* p1 = NULL, void* p2 = NULL, void* p3 = NULL, void* p4 = NULL) = 0;
	virtual void Release() = 0;

	virtual int BeginScene(DWORD dwClearMode = (0x1L | 0x2L | 0x4L), DWORD dwClearColor = 0xFF006699) = 0;
	virtual int EndScene() = 0;

	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual DWORD GetDepth() = 0;

	virtual void* GetTexture() const = 0;
	virtual void* GetSurface() const = 0;
};



