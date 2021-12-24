#pragma once
class Shader;

class Circle
{
public:
	
	Circle(int num);
	~Circle();
	void Position(float x, float y);
	void Position(D3DXVECTOR2 vec);

	void Scale(float x, float y);
	void Scale(D3DXVECTOR2 vec);

	void Rotate(D3DXVECTOR3 x);
	void Rotate(float x, float y, float z);
	void Rotate2(D3DXVECTOR3 x);
	void Rotate2(float x, float y, float z);

	void Color(float r, float g, float b, float a = 1.0f);
	void Color(D3DXCOLOR vec);


	void Update();
	void Render();
	FCIRCLE GetCircle() { return cc; }
	void SetParent(D3DXMATRIX* p) { parent = p; }
	D3DXMATRIX* GetRT() { return &RT; }
private:
	struct Vertex
	{
		Vertex() {}

		Vertex(D3DXVECTOR3 position) : Position(position)
		{

		}

		D3DXVECTOR3 Position;
	};

	Vertex* vertices;
	ID3D11Buffer* vertexBuffer;
private:
	UINT CircleCount;
	UINT VertexCount;

	D3DXVECTOR2 position;
	D3DXVECTOR2 scale;
	D3DXVECTOR3	angle;
	D3DXVECTOR3	angle2;

	FCIRCLE		cc;
	D3DXMATRIX* parent;
	D3DXMATRIX  RT;
	Shader*		shader;

	//추가 변수
	UINT VertexCount2;
	Vertex* vertices2;
	ID3D11Buffer* vertexBuffer2;
	bool isFill = true;
	//추가
	ID3D11BlendState* blendState[2];
public:
	MAKEGETSET(bool, isFill);




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
