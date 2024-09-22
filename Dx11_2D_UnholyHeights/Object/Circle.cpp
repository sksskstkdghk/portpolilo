#include "stdafx.h"
#include "Circle.h"


Circle::Circle(int num)
	:VertexCount(3 * num), VertexCount2(2 * num), parent(nullptr)
{
	ZeroMemory(&angle, sizeof(angle));
	ZeroMemory(&angle2, sizeof(angle));
	CircleCount = num;
	float radius = 360.0f / CircleCount;
	vertices = new Vertex[VertexCount];

	for (UINT i = 0; i < CircleCount; i++)
	{
		vertices[i * 3] = Vertex(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		vertices[i * 3 + 1] = Vertex(D3DXVECTOR3(0 + cosf((i + 1)* radius *TORADIAN)*0.5f,
			0 + sinf((i + 1)*radius*TORADIAN)*0.5f, 0.0f));
		vertices[i * 3 + 2] = Vertex(D3DXVECTOR3(0 + cosf(i*radius*TORADIAN)*0.5f,
			0 + sinf(i*radius*TORADIAN)*0.5f, 0.0f));
	}

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

	for (UINT i = 0; i < CircleCount; i++)
	{
		vertices2[i * 2] = Vertex(D3DXVECTOR3(0 + cosf((i + 1)* radius *TORADIAN)*0.5f,
			0 + sinf((i + 1)*radius*TORADIAN)*0.5f, 0.0f));
		vertices2[i * 2 + 1] = Vertex(D3DXVECTOR3(0 + cosf(i*radius*TORADIAN)*0.5f,
			0 + sinf(i*radius*TORADIAN)*0.5f, 0.0f));
	}

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

	shader = new Shader(_T("./Object/Basic.hlsl"));
	D3D11_INPUT_ELEMENT_DESC LayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	shader->CreateInputLayout(LayoutDesc, 1);

	//Create ColorBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(ColorBufferDesc);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = Device->CreateBuffer(&desc, NULL, &colorBuffer);
		assert(SUCCEEDED(hr));
	}

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


Circle::~Circle()
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

void Circle::Position(float x, float y)
{
	Position(D3DXVECTOR2(x, y));
}

void Circle::Position(D3DXVECTOR2 vec)
{
	position = vec;
}

void Circle::Scale(float x, float y)
{
	Scale(D3DXVECTOR2(x, y));
}

void Circle::Scale(D3DXVECTOR2 vec)
{
	scale = vec;
}

void Circle::Rotate(D3DXVECTOR3 x)
{
	angle = x;
}
void Circle::Rotate(float x, float y, float z)
{
	Rotate(D3DXVECTOR3(x, y, z));
}
void Circle::Rotate2(D3DXVECTOR3 x)
{
	angle2 = x;
}
void Circle::Rotate2(float x, float y, float z)
{
	Rotate2(D3DXVECTOR3(x, y, z));
}

void Circle::Color(float r, float g, float b, float a)
{
	Color(D3DXCOLOR(r, g, b, a));
}

void Circle::Color(D3DXCOLOR vec)
{
	colorBufferData.Color = vec;
}

void Circle::Update()
{
	D3DXMATRIX S, R, T, W , R2;
	D3DXMatrixScaling(&S, scale.x, scale.y, 1);
	D3DXMatrixRotationYawPitchRoll(&R, angle.y, angle.x, angle.z);
	D3DXMatrixTranslation(&T, position.x, position.y, 0);
	D3DXMatrixRotationYawPitchRoll(&R2, angle2.y, angle2.x, angle2.z);

	W = S * R * T * R2;
	RT = R * T * R2;
	cc = CircleMake(position.x, position.y, scale.x *0.5f);
	if (parent)
	{
		W *= *parent;
		RT *= *parent;
		cc = CircleMake(W._41, W._42, scale.x*0.5f);
		
	}
	shader->World(W);
	
}

void Circle::Render()
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
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		DeviceContext->Draw(VertexCount2, 0);
	}
	//ºí·»µå ÄÑ±â
	DeviceContext->OMSetBlendState(blendState[0], NULL, 0xff);
}
