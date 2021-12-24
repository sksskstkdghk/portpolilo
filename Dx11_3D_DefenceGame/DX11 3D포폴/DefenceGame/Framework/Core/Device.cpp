#include "Framework.h"

Device* Device::instance = nullptr;

Device::Device()
{
	CreateDeviceAndSwapChain();
	CreateBackBuffer();
}

Device::~Device()
{
    swapChain->Release();
    device->Release();
    context->Release();
    renderTargetView->Release();
    depthStencil->Release();
    depthStencilView->Release();
}

void Device::CreateDeviceAndSwapChain()
{    
    UINT width = WIN_WIDTH;
    UINT height = WIN_HEIGHT;

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };
    UINT featureSize = ARRAYSIZE(featureLevels);

    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;    
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = true;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        0,
        D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT,
        featureLevels,
        featureSize,
        D3D11_SDK_VERSION,
        &sd,
        &swapChain,
        &device,
        nullptr,
        &context
    );

    DEBUG(hr);  
}

void Device::CreateBackBuffer()
{
    ID3D11Texture2D* backBuffer = nullptr;

    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
    backBuffer->Release();

    D3D11_TEXTURE2D_DESC descDepth = {};
    descDepth.Width = WIN_WIDTH;
    descDepth.Height = WIN_HEIGHT;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    HRESULT hr = device->CreateTexture2D(&descDepth, nullptr, &depthStencil);
    DEBUG(hr);

    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    hr = device->CreateDepthStencilView(depthStencil, &descDSV, &depthStencilView);
    DEBUG(hr);

    SetRenderTarget();
}

void Device::SetRenderTarget(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv)
{
    if (rtv == nullptr)
        rtv = renderTargetView;
    if (dsv == nullptr)
        dsv = depthStencilView;

    context->OMSetRenderTargets(1, &rtv, dsv);
}

void Device::SetRenderTargets(UINT count, ID3D11RenderTargetView** rtv, ID3D11DepthStencilView* dsv)
{
    if (dsv == nullptr)
        dsv = depthStencilView;

    context->OMSetRenderTargets(count, rtv, dsv);
}

void Device::Clear(Color color, ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv)
{
    if (rtv == nullptr)
        rtv = renderTargetView;
    if (dsv == nullptr)
        dsv = depthStencilView;

    context->ClearRenderTargetView(rtv, color);
    context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Device::Present()
{
    swapChain->Present(0, 0);
}
