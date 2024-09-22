#pragma once
class Shader;

enum PlayState
{
	ANI_STOP,
	ANI_LOOP,
	ANI_REVERSE
};


class Image
{
public:
	Image(_tstring file, string ps = "PS", string vs = "VS");
	Image(_tstring file, D3DXVECTOR2 leftDown, D3DXVECTOR2 rightUp, string ps = "PS", string vs = "VS");
	Image(_tstring file, UINT MaxFrameX, UINT MaxFrameY, string ps = "PS");
	~Image();
	void Position(float x, float y);
	void Position(D3DXVECTOR2 vec);

	void Scale(float x, float y);
	void Scale(D3DXVECTOR2 vec);

	void Rotate(D3DXVECTOR3 x);
	void Rotate(float x, float y, float z);
	void Rotate2(D3DXVECTOR3 x);
	void Rotate2(float x, float y, float z);

	void Update();
	void Render();
	void Render(float startx, float starty, float endx, float endy);
	void LoofRender(float x = 0.0f, float y = 0.0f);

	void PlayAnimation(PlayState ps, bool isFrameX, float nexttime);

	void SetParent(D3DXMATRIX* p) { parent = p; }
	void SetCurrentFrameX(UINT frameX) { CurrentFrameX = frameX; }
	void SetCurrentFrameY(UINT frameY) { CurrentFrameY = frameY; }

	D3DXMATRIX* GetRT() { return &RT; }
	D3DXVECTOR2 GetScale() { return scale; }
	D3DXVECTOR2 GetPosition() { return position; }
	FRECT GetRect() { return rc; }

	void Color(float r, float g, float b, float a = 1.0f);
	void Color(D3DXCOLOR vec);

private:
	const UINT VertexCount = 6;

	D3DXVECTOR2 position;
	D3DXVECTOR2 scale;
	D3DXVECTOR3		angle;
	D3DXVECTOR3		angle2;
	Shader*		shader;

	D3DXMATRIX* parent;
	D3DXMATRIX  RT;
	ID3D11ShaderResourceView* srv;

	FRECT		rc;

private:
	struct Vertex
	{
		Vertex() {}

		D3DXVECTOR3 Position;
		D3DXVECTOR2 Uv;
	};

	Vertex* vertices;
	ID3D11Buffer* vertexBuffer;
	//Ãß°¡
	ID3D11BlendState* blendState[2];
private:
	struct FrameUvBufferDesc
	{
		FrameUvBufferDesc() : FrameUv(0, 0, 0, 0)
		{

		}

		FrameUvBufferDesc(D3DXVECTOR4 Uv) : FrameUv(Uv)
		{

		}

		D3DXVECTOR4 FrameUv;
	} FrameUvBufferData;
	ID3D11Buffer* FrameUvBuffer = nullptr;
	UINT maxFrameX = 1;
	UINT maxFrameY = 1;
	UINT CurrentFrameX = 0;
	UINT CurrentFrameY = 0;
	float time = 0.0f;
public:
	MAKEREFGET(UINT, CurrentFrameX);
	MAKEREFGET(UINT, CurrentFrameY);
	MAKEREFGET(UINT, maxFrameX);
	MAKEREFGET(UINT, maxFrameY);


private:
	struct ColorBufferDesc
	{
		ColorBufferDesc() : Color(0, 0, 0, 1)
		{

		}

		ColorBufferDesc(D3DXCOLOR color) : Color(color)
		{

		}

		D3DXCOLOR Color;
	} colorBufferData;
	ID3D11Buffer* colorBuffer;

};

