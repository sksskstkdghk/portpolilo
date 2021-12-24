#pragma once

class Perspective
{
private:
	Matrix projection;

public:
	Perspective(float width = WIN_WIDTH, float height = WIN_HEIGHT,
		float fov = D3DX_PI * 0.25f, float zn = 0.1f, float zf = 1000.0f);
	~Perspective();

	Matrix GetMatrix() { return projection; }
};