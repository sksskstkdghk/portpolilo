#include "Framework.h"

VertexBuffer::VertexBuffer(void* data, UINT stride, UINT count, bool isCpuWrite)
    : stride(stride), offset(0)
{
    D3D11_BUFFER_DESC bd = {};
    if (isCpuWrite)
    {
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    }
    else
    {
        bd.Usage = D3D11_USAGE_DEFAULT;
    }
    bd.ByteWidth = stride * count;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = data;

    HRESULT hr = DEVICE->CreateBuffer(&bd, &initData, &buffer);

    DEBUG(hr);
}

VertexBuffer::~VertexBuffer()
{
    buffer->Release();
}

void VertexBuffer::Set(UINT slot)
{
    DC->IASetVertexBuffers(slot, 1, &buffer, &stride, &offset);
}

void VertexBuffer::UpdateBuffer(void* data, UINT count)
{
    DC->UpdateSubresource(buffer, 0, nullptr, data, stride, count);
}

////////////////////////////////////////////////////////////////////////

IndexBuffer::IndexBuffer(void* data, UINT count)
{
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(UINT) * count;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = data;

    HRESULT hr = DEVICE->CreateBuffer(&bd, &initData, &buffer);
    DEBUG(hr);
}

IndexBuffer::~IndexBuffer()
{
    buffer->Release();
}

void IndexBuffer::Set()
{
    DC->IASetIndexBuffer(buffer, DXGI_FORMAT_R32_UINT, 0);
}
