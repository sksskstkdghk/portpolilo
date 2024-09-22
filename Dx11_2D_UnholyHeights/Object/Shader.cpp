#include "stdafx.h"
#include "Shader.h"

Shader::Shader(_tstring file, string vs, string ps)
	: vertexLayout(NULL)
{
	HRESULT hr;

	hr = D3DX11CompileFromFile(file.c_str(), 0, 0, vs.c_str(), "vs_5_0", 0, 0, 0, &vsBlob, 0, 0);
	assert(SUCCEEDED(hr));

	hr = D3DX11CompileFromFile(file.c_str(), 0, 0, ps.c_str(), "ps_5_0", 0, 0, 0, &psBlob, 0, 0);
	assert(SUCCEEDED(hr));


	hr = Device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &vertexShader);
	assert(SUCCEEDED(hr));

	hr = Device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &pixelShader);
	assert(SUCCEEDED(hr));


	//Create WorldBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(WorldBufferDesc);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = Device->CreateBuffer(&desc, NULL, &worldBuffer);
		assert(SUCCEEDED(hr));
	}

	
}

Shader::~Shader()
{
	SAFE_RELEASE(vsBlob);
	SAFE_RELEASE(psBlob);

	SAFE_RELEASE(vertexLayout);
	SAFE_RELEASE(vertexShader);
	SAFE_RELEASE(pixelShader);
	SAFE_RELEASE(worldBuffer);
	
}

void Shader::World(D3DXMATRIX & world)
{
	D3DXMatrixTranspose(&worldBufferData.World, &world);
}

void Shader::CreateInputLayout(D3D11_INPUT_ELEMENT_DESC * desc, UINT count)
{
	HRESULT hr = Device->CreateInputLayout
	(
		desc, count, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &vertexLayout
	);
	assert(SUCCEEDED(hr));
}

void Shader::SetBuffers()
{
	assert(vertexLayout != NULL);

	
	DeviceContext->UpdateSubresource(worldBuffer, 0, NULL, &worldBufferData, 0, 0);
	DeviceContext->VSSetConstantBuffers(1, 1, &worldBuffer);

	DeviceContext->VSSetShader(vertexShader, 0, 0);
	DeviceContext->PSSetShader(pixelShader, 0, 0);

	DeviceContext->IASetInputLayout(vertexLayout);
}
