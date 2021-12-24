#pragma once

class Render2D
{
private:
	struct TextData
	{
		wstring text;
		RECT renderRect;
	}renderText;

	VertexUVColor vertices[6];
public:
	Vector2 position;
	Vector2 scale;

protected:
	Shader* shader;
	ID3D11ShaderResourceView* srv;

	WorldBuffer* worldBuffer;

	VertexBuffer* vertexBuffer;

	Matrix view;
	Matrix orthographic;

	DepthStencilState* depthState[2];

public:
	Render2D(wstring shaderFile = L"Render2D");
	virtual ~Render2D();

	virtual void Update();
	virtual void Render();

	void SetSRV(ID3D11ShaderResourceView* value) { srv = value; }
	void SetText(Vector2 pos, Vector2 scale, wstring text);
	void SetText(wstring text) { renderText.text = text; }
	void SetWorld(Matrix world) { worldBuffer->SetWorld(world); }

	void SetBackColor(Color color);

	bool IsMouseCollision();
};