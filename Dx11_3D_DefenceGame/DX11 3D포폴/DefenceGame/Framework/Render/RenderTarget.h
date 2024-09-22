#pragma once

class Viewport;

class RenderTarget
{
private:
	Viewport* viewport;

	UINT width, height;

	ID3D11ShaderResourceView* srv;
	ID3D11Texture2D* rtvTexture;
	ID3D11RenderTargetView* rtv;

	ID3D11Texture2D* dsvTexture;
	ID3D11DepthStencilView* dsv;

public:
	RenderTarget(UINT width = WIN_WIDTH, UINT height = WIN_HEIGHT);
	~RenderTarget();

	void Set(Color clear = Color(1, 1, 1, 1));

	ID3D11ShaderResourceView* GetSRV() { return srv; }
	ID3D11RenderTargetView* GetRTV() { return rtv; }
	ID3D11DepthStencilView* GetDSV() { return dsv; }
};