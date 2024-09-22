#pragma once

class Device
{
private:
	ID3D11Device* device;
	ID3D11DeviceContext* context;

	IDXGISwapChain* swapChain;

	ID3D11RenderTargetView* renderTargetView;
	ID3D11Texture2D* depthStencil;
	ID3D11DepthStencilView* depthStencilView;

	static Device* instance;

	Device();
	~Device();

public:
	static Device* Get() { return instance; }
	static void Create() { instance = new Device(); }
	static void Delete() { delete instance; }

	void CreateDeviceAndSwapChain();
	void CreateBackBuffer();

	void SetRenderTarget(ID3D11RenderTargetView* rtv = nullptr,
		ID3D11DepthStencilView* dsv = nullptr);
	void SetRenderTargets(UINT count, ID3D11RenderTargetView** rtv = nullptr,
		ID3D11DepthStencilView* dsv = nullptr);

	void Clear(Color color = Color(0.0f, 0.125f, 0.3f, 1.0f),
		ID3D11RenderTargetView* rtv = nullptr, ID3D11DepthStencilView* dsv = nullptr);

	void Present();

	ID3D11Device* GetDevice() { return device; }
	ID3D11DeviceContext* GetContext() { return context; }
	IDXGISwapChain* GetSwapChain() { return swapChain; }
};