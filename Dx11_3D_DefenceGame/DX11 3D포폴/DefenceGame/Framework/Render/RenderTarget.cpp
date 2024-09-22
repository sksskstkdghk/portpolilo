#include "Framework.h"

RenderTarget::RenderTarget(UINT width, UINT height)
	: width(width), height(height)
{
	viewport = new Viewport(width, height);

	HRESULT hr;
	{//Create RTV Texture
        D3D11_TEXTURE2D_DESC desc = {};
        desc.Width = width;
        desc.Height = height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

        hr = DEVICE->CreateTexture2D(&desc, nullptr, &rtvTexture);
        DEBUG(hr);
	}

    {//Create RTV
        D3D11_RENDER_TARGET_VIEW_DESC desc = {};
        desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

        hr = DEVICE->CreateRenderTargetView(rtvTexture, &desc, &rtv);
        DEBUG(hr);
    }

    {//Create SRV
        D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
        desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        desc.Texture2D.MipLevels = 1;

        hr = DEVICE->CreateShaderResourceView(rtvTexture, &desc, &srv);
        DEBUG(hr);
    }

	{//Create DSV Texture
        D3D11_TEXTURE2D_DESC desc = {};
        desc.Width = width;
        desc.Height = height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        hr = DEVICE->CreateTexture2D(&desc, nullptr, &dsvTexture);
        DEBUG(hr);
	}

    {//Create DSV
        D3D11_DEPTH_STENCIL_VIEW_DESC desc = {};
        desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;        

        hr = DEVICE->CreateDepthStencilView(dsvTexture, &desc, &dsv);
        DEBUG(hr);
    }
}

RenderTarget::~RenderTarget()
{
    rtvTexture->Release();
    dsvTexture->Release();

    srv->Release();
    rtv->Release();
    dsv->Release();

    delete viewport;
}

void RenderTarget::Set(Color clear)
{
    Device::Get()->SetRenderTarget(rtv, dsv);
    Device::Get()->Clear(clear, rtv, dsv);

    viewport->Set();
}
