#pragma once

inline void IASetPT(D3D_PRIMITIVE_TOPOLOGY type = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
	DC->IASetPrimitiveTopology(type);
}

class VertexBuffer
{
private:
	ID3D11Buffer* buffer;

	UINT stride;
	UINT offset;

public:
	VertexBuffer(void* data, UINT stride, UINT count, bool isCpuWrite = false);
	~VertexBuffer();

	void Set(UINT slot = 0);

	void UpdateBuffer(void* data, UINT count);
};

class IndexBuffer
{
private:
	ID3D11Buffer* buffer;

public:
	IndexBuffer(void* data, UINT count);
	~IndexBuffer();

	void Set();
};