#include "Framework.h"

Render2D::Render2D(wstring shaderFile)
	: position(0, 0), scale(1, 1)
{
	shader = Shader::Add(shaderFile);

	worldBuffer = new WorldBuffer();
	
	D3DXMatrixIdentity(&view);	
	D3DXMatrixOrthoOffCenterLH(&orthographic, 0, WIN_WIDTH, 0, WIN_HEIGHT, -1, 1);

	depthState[0] = new DepthStencilState();
	depthState[1] = new DepthStencilState();
	depthState[1]->DepthEnable(false);

	renderText.text = L"";
	renderText.renderRect.left = 0;
	renderText.renderRect.right = 0;
	renderText.renderRect.top = 0;
	renderText.renderRect.bottom = 0;

	vertices[0].position = { -0.5f, -0.5f, 0.5f };
	vertices[1].position = { -0.5f, 0.5f, 0.5f };
	vertices[2].position = { 0.5f, -0.5f, 0.5f };
	vertices[3].position = { 0.5f, -0.5f, 0.5f };
	vertices[4].position = { -0.5f, 0.5f, 0.5f };
	vertices[5].position = { 0.5f, 0.5f, 0.5f };

	vertices[0].uv = { 0.0f, 1.0f };
	vertices[1].uv = { 0.0f, 0.0f };
	vertices[2].uv = { 1.0f, 1.0f };
	vertices[3].uv = { 1.0f, 1.0f };
	vertices[4].uv = { 0.0f, 0.0f };
	vertices[5].uv = { 1.0f, 0.0f };

	vertices[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
	vertices[1].color = { 0.0f, 0.0f, 0.0f, 1.0f };
	vertices[2].color = { 0.0f, 0.0f, 0.0f, 1.0f };
	vertices[3].color = { 0.0f, 0.0f, 0.0f, 1.0f };
	vertices[4].color = { 0.0f, 0.0f, 0.0f, 1.0f };
	vertices[5].color = { 0.0f, 0.0f, 0.0f, 1.0f };

	vertexBuffer = new VertexBuffer(vertices, sizeof(VertexUVColor), 6);
}

Render2D::~Render2D()
{
	delete worldBuffer;
	delete depthState[0];
	delete depthState[1];
	delete vertexBuffer;
}

void Render2D::Update()
{
	Matrix S, T;
	D3DXMatrixScaling(&S, scale.x, scale.y, 1.0f);
	D3DXMatrixTranslation(&T, position.x, position.y, 0.0f);

	worldBuffer->SetWorld(S * T);
}

void Render2D::Render()
{
	VP->SetView(view);
	VP->SetProjection(orthographic);
	VP->SetVSBuffer(0);

	vertexBuffer->Set();
	IASetPT();

	worldBuffer->SetVSBuffer(1);
	shader->Set();
	
	DC->PSSetShaderResources(10, 1, &srv);

	depthState[1]->SetState();
	DC->Draw(6, 0);

	if (renderText.text != L"")
		WRITE->RenderText(renderText.text, renderText.renderRect);

	depthState[0]->SetState();
}

void Render2D::SetText(Vector2 pos, Vector2 scale, wstring text)
{
	renderText.text = text;

	renderText.renderRect.left = pos.x - (scale.x * 0.5f);
	renderText.renderRect.right = pos.x + (scale.x * 0.5f);
	renderText.renderRect.bottom = WIN_HEIGHT - pos.y - (scale.y * 0.5f);
	renderText.renderRect.top = WIN_HEIGHT - pos.y + (scale.y * 0.5f);
}

void Render2D::SetBackColor(Color color)
{
	for (int i = 0; i < 6; i++)
		vertices[i].color = color;

	vertexBuffer->UpdateBuffer(vertices, 6);
}

bool Render2D::IsMouseCollision()
{
	Vector3 min = Vector3(position.x - scale.x * 0.5f, 0.0f, position.y - scale.y * 0.5f);
	Vector3 max = Vector3(position.x + scale.x * 0.5f, 0.0f, position.y + scale.y * 0.5f);
	Vector3 point = Vector3(mousePos.x, 0.0f, WIN_HEIGHT - mousePos.y);

	return GameMath::IntersectRect(min, max, point);
}
