#include "stdafx.h"
#include "Rect.h"

Rect::Rect():parent(nullptr)
{
	ZeroMemory(&angle, sizeof(angle));
	ZeroMemory(&angle2, sizeof(angle));
	vertices = new Vertex[VertexCount];
	vertices[0].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
	vertices[1].Position = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);
	vertices[2].Position = D3DXVECTOR3(0.5f, -0.5f, 0.0f);
	vertices[3].Position = D3DXVECTOR3(0.5f, -0.5f, 0.0f);
	vertices[4].Position = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);
	vertices[5].Position = D3DXVECTOR3(0.5f, 0.5f, 0.0f);


	//CreateVertexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.ByteWidth = sizeof(Vertex) * VertexCount;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices;

		HRESULT hr = Device->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}

	vertices2 = new Vertex[VertexCount2];
	vertices2[0].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
	vertices2[1].Position = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);
	vertices2[2].Position = D3DXVECTOR3(0.5f, 0.5f, 0.0f);
	vertices2[3].Position = D3DXVECTOR3(0.5f, -0.5f, 0.0f);
	vertices2[4].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);


	//CreateVertexBuffer2
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.ByteWidth = sizeof(Vertex) * VertexCount2;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices2;

		HRESULT hr = Device->CreateBuffer(&desc, &data, &vertexBuffer2);
		assert(SUCCEEDED(hr));
	}




	//Create ColorBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(ColorBufferDesc);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = Device->CreateBuffer(&desc, NULL, &colorBuffer);
		assert(SUCCEEDED(hr));
	}
	shader = new Shader(_T("./Object/Basic.hlsl"));
	D3D11_INPUT_ELEMENT_DESC LayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	shader->CreateInputLayout(LayoutDesc, 1);

	//CreateBlendState
	{
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));

		desc.RenderTarget[0].BlendEnable = false;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		Device->CreateBlendState(&desc, &blendState[0]);

		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		Device->CreateBlendState(&desc, &blendState[1]);
	}


}

Rect::Rect(D3DXVECTOR2 leftdown, D3DXVECTOR2 rightTop) :parent(nullptr)
{
	ZeroMemory(&angle, sizeof(angle));
	ZeroMemory(&angle2, sizeof(angle));
	vertices = new Vertex[VertexCount];
	vertices[0].Position = D3DXVECTOR3(leftdown.x, leftdown.y, 0.0f);
	vertices[1].Position = D3DXVECTOR3(leftdown.x, rightTop.y, 0.0f);
	vertices[2].Position = D3DXVECTOR3(rightTop.x, leftdown.y, 0.0f);
	vertices[3].Position = D3DXVECTOR3(rightTop.x, leftdown.y, 0.0f);
	vertices[4].Position = D3DXVECTOR3(leftdown.x, rightTop.y, 0.0f);
	vertices[5].Position = D3DXVECTOR3(rightTop.x, rightTop.y, 0.0f);


	//CreateVertexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.ByteWidth = sizeof(Vertex) * VertexCount;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices;

		HRESULT hr = Device->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}

	vertices2 = new Vertex[VertexCount2];
	vertices2[0].Position = D3DXVECTOR3(leftdown.x, leftdown.y, 0.0f);
	vertices2[1].Position = D3DXVECTOR3(leftdown.x, rightTop.y, 0.0f);
	vertices2[2].Position = D3DXVECTOR3(rightTop.x, rightTop.y, 0.0f);
	vertices2[3].Position = D3DXVECTOR3(rightTop.x, leftdown.y, 0.0f);
	vertices2[4].Position = D3DXVECTOR3(leftdown.x, leftdown.y, 0.0f);


	//CreateVertexBuffer2
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.ByteWidth = sizeof(Vertex) * VertexCount2;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices2;

		HRESULT hr = Device->CreateBuffer(&desc, &data, &vertexBuffer2);
		assert(SUCCEEDED(hr));
	}




	//Create ColorBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(ColorBufferDesc);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = Device->CreateBuffer(&desc, NULL, &colorBuffer);
		assert(SUCCEEDED(hr));
	}
	shader = new Shader(_T("./Object/Basic.hlsl"));
	D3D11_INPUT_ELEMENT_DESC LayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	shader->CreateInputLayout(LayoutDesc, 1);

	//CreateBlendState
	{
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));

		desc.RenderTarget[0].BlendEnable = false;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		Device->CreateBlendState(&desc, &blendState[0]);

		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		Device->CreateBlendState(&desc, &blendState[1]);
	}


}

Rect::~Rect()
{
	SAFE_DELETE(shader);
	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(vertices2);
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(vertexBuffer2);
	SAFE_RELEASE(colorBuffer);
	SAFE_RELEASE(blendState[1]);
	SAFE_RELEASE(blendState[0]);
}

void Rect::Position(float x, float y)
{
	Position(D3DXVECTOR2(x, y));
}

void Rect::Position(D3DXVECTOR2 vec)
{
	position = vec;
}

void Rect::Scale(float x, float y)
{
	Scale(D3DXVECTOR2(x, y));
}

void Rect::Scale(D3DXVECTOR2 vec)
{
	scale = vec;
}

void Rect::Rotate(D3DXVECTOR3 x)
{
	angle = x;
}
void Rect::Rotate(float x, float y, float z)
{
	Rotate(D3DXVECTOR3(x, y, z));
}
void Rect::Rotate2(D3DXVECTOR3 x)
{
	angle2 = x;
}
void Rect::Rotate2(float x, float y, float z)
{
	Rotate2(D3DXVECTOR3(x, y, z));
}

void Rect::Color(float r, float g, float b, float a)
{
	Color(D3DXCOLOR(r, g, b, a));
}

void Rect::Color(D3DXCOLOR vec)
{
	colorBufferData.Color = vec;
}

void Rect::Update()
{
	D3DXMATRIX S,R,T,W,R2;
	D3DXMatrixScaling(&S, scale.x, scale.y, 1);
	D3DXMatrixRotationYawPitchRoll(&R, angle.y, angle.x, angle.z);
	D3DXMatrixTranslation(&T, position.x, position.y, 0);
	D3DXMatrixRotationYawPitchRoll(&R2, angle2.y, angle2.x, angle2.z);
	W = S * R * T * R2;
	RT = R * T * R2;
	rc = RectMakeCenter(position.x, position.y, scale.x, scale.y);
	if (parent)
	{
		W *= *parent;
		RT *= *parent;
		rc = RectMakeCenter(W._41, W._42, scale.x, scale.y);
		
	}
	shader->World(W);
	
}

void Rect::Render()
{
	Update();
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	shader->SetBuffers();

	DeviceContext->UpdateSubresource(colorBuffer, 0, NULL, &colorBufferData, 0, 0);
	DeviceContext->PSSetConstantBuffers(0, 1, &colorBuffer);
	//ºí·»µå ÄÑ±â
	DeviceContext->OMSetBlendState(blendState[1], NULL, 0xff);
	if (isFill)
	{
		DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		DeviceContext->Draw(VertexCount, 0);
	}
	else
	{
		DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer2, &stride, &offset);
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		DeviceContext->Draw(VertexCount2, 0);
	}
	//ºí·»µå ÄÑ±â
	DeviceContext->OMSetBlendState(blendState[0], NULL, 0xff);
}
