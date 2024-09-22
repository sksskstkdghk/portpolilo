#include "Framework.h"

Viewport::Viewport(float width, float height)
{    
    viewport.Width = width;
    viewport.Height = height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    
    Set();
}

Viewport::~Viewport()
{
}

void Viewport::Set()
{
    DC->RSSetViewports(1, &viewport);
}
