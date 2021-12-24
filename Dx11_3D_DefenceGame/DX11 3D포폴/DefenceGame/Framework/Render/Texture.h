#pragma once

class Texture
{
private:
	wstring file;

	ID3D11ShaderResourceView* SRV;
	ID3D11SamplerState* sampler;

	ID3D11Texture2D* srcTexture;
	D3D11_TEXTURE2D_DESC srcDesc;

	UINT width, height;

	static map<wstring, ID3D11ShaderResourceView*> totalSRV;
	static vector<Texture*> totalTexture;

	Texture(ID3D11ShaderResourceView* SRV, wstring file);	
	~Texture();

public:
	static Texture* Add(wstring file);
	static ID3D11ShaderResourceView* LoadSRV(wstring file);
	static void Delete();

	vector<Color> ReadPixels();

	void Set(UINT slot);

	ID3D11ShaderResourceView* GetSRV() { return SRV; }
	UINT GetWidth() { return width; }
	UINT GetHeight() { return height; }

	wstring GetFile() { return file; }
};