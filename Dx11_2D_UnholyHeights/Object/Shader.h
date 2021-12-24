#pragma once
//객체마다 다른 월드,다른 쉐이더,다른 인풋레이아웃을 설정하여 사용할수 있게끔 만든 클래스
class Shader
{
private:
	D3DXMATRIX world;

	ID3D11InputLayout* vertexLayout;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	
	ID3D10Blob* vsBlob;
	ID3D10Blob* psBlob;
public:
	Shader(_tstring file, string vs = "VS", string ps = "PS");
	~Shader(); 
	void World(D3DXMATRIX& world);
	void CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* desc, UINT count);

	void SetBuffers();
	struct WorldBufferDesc
	{
	public:
		WorldBufferDesc()
		{
			D3DXMatrixIdentity(&World);
		}

		D3DXMATRIX World;
	} worldBufferData;

	ID3D11Buffer* worldBuffer;
};

