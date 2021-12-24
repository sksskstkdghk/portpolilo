#pragma once

class Viewport
{
private:
	D3D11_VIEWPORT viewport;

public:
	Viewport(float width = WIN_WIDTH, float height = WIN_HEIGHT);
	~Viewport();

	void Set();
};