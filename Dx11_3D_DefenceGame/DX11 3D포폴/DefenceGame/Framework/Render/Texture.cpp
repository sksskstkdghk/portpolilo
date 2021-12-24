#include "Framework.h"

map<wstring, ID3D11ShaderResourceView*> Texture::totalSRV;
vector<Texture*> Texture::totalTexture;

Texture::Texture(ID3D11ShaderResourceView* SRV, wstring file)
	: SRV(SRV), file(file)
{
	SRV->GetResource((ID3D11Resource**)&srcTexture);
	srcTexture->GetDesc(&srcDesc);

	width = srcDesc.Width;
	height = srcDesc.Height;

	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = DEVICE->CreateSamplerState(&sampDesc, &sampler);
	DEBUG(hr);
}

Texture::~Texture()
{
	sampler->Release();
}

Texture* Texture::Add(wstring file)
{	
	totalTexture.push_back(new Texture(LoadSRV(file), file));

	return totalTexture.back();
}

ID3D11ShaderResourceView* Texture::LoadSRV(wstring file)
{
	ID3D11ShaderResourceView* SRV;

	if (totalSRV.count(file) > 0)
	{
		SRV = totalSRV[file];
	}
	else
	{
		HRESULT hr = D3DX11CreateShaderResourceViewFromFile(DEVICE,
			file.c_str(), nullptr, nullptr, &SRV, nullptr);

		DEBUG(hr);

		//totalSRV[file] = SRV;
		//totalSRV.insert(pair<wstring, ID3D11ShaderResourceView*>(file, SRV));
		//totalSRV.insert(make_pair(file, SRV));
		totalSRV.insert({ file, SRV });
	}
	return SRV;
}

void Texture::Delete()
{
	for (auto SRV : totalSRV)
		SRV.second->Release();

	for (Texture* texture : totalTexture)
		delete texture;
}

vector<Color> Texture::ReadPixels()
{
	vector<Color> pixels;

	D3D11_TEXTURE2D_DESC destDesc = {};
	destDesc.Width = width;
	destDesc.Height = height;
	destDesc.MipLevels = 1;
	destDesc.ArraySize = 1;
	destDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	destDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	destDesc.Usage = D3D11_USAGE_STAGING;
	destDesc.SampleDesc = srcDesc.SampleDesc;
	
	ID3D11Texture2D* destTexture;
	DEVICE->CreateTexture2D(&destDesc, nullptr, &destTexture);

	D3DX11LoadTextureFromTexture(DC, srcTexture, nullptr, destTexture);

	UINT* colors = new UINT[width * height];
	D3D11_MAPPED_SUBRESOURCE map;
	DC->Map(destTexture, 0, D3D11_MAP_READ, 0, &map);
	memcpy(colors, map.pData, sizeof(UINT) * width * height);
	DC->Unmap(destTexture, 0);

	for (UINT i = 0; i < width * height; i++)
	{
		float f = 1.0f / 255.0f;

		float a = f * (float)((0xff000000 & colors[i]) >> 24);
		float b = f * (float)((0x00ff0000 & colors[i]) >> 16);
		float g = f * (float)((0x0000ff00 & colors[i]) >> 8);
		float r = f * (float)((0x000000ff & colors[i]) >> 0);

		pixels.push_back(Color(r, g, b, a));
	}

	destTexture->Release();
	delete[] colors;

	return pixels;
}

void Texture::Set(UINT slot)
{
	DC->PSSetShaderResources(slot, 1, &SRV);
	DC->PSSetSamplers(slot, 1, &sampler);
}
