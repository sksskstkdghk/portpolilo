#include "Framework.h"

map<wstring, Shader*> Shader::totalShader;

Shader::Shader(wstring shaderFile, string vsName, string psName)
	: vsName(vsName), psName(psName), gsName(""),
	geometryShader(nullptr), geometryBlob(nullptr)
{
	this->shaderFile = L"Shaders/" + shaderFile + L".hlsl";
	CreateVertexShader();
	CreatePixelShader();
	CreateInputLayout();
}

Shader::~Shader()
{
	vertexShader->Release();
	pixelShader->Release();
	inputLayout->Release();

	vertexBlob->Release();
	pixelBlob->Release();

	if (geometryShader != nullptr)
		geometryShader->Release();
	if (geometryBlob != nullptr)
		geometryBlob->Release();
}

Shader* Shader::Add(wstring shaderFile, string vsName, string psName)
{
	if (totalShader.count(shaderFile) > 0)
		return totalShader[shaderFile];
	else
	{
		totalShader.insert({ shaderFile, new Shader(shaderFile, vsName, psName) });
		return totalShader[shaderFile];
	}
}

void Shader::Delete()
{
	for (auto shader : totalShader)
		delete shader.second;
}

void Shader::Set()
{
	DC->IASetInputLayout(inputLayout);
	DC->VSSetShader(vertexShader, nullptr, 0);	
	DC->PSSetShader(pixelShader, nullptr, 0);
	if (geometryShader != nullptr)
		DC->GSSetShader(geometryShader, nullptr, 0);
}

void Shader::CreateVertexShader()
{
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

	D3DX11CompileFromFile(shaderFile.c_str(), nullptr, nullptr,
		vsName.c_str(), "vs_5_0", flags, 0, nullptr, &vertexBlob, nullptr, nullptr);

	HRESULT hr = DEVICE->CreateVertexShader(vertexBlob->GetBufferPointer(),
		vertexBlob->GetBufferSize(), nullptr, &vertexShader);
	DEBUG(hr);
}

void Shader::CreatePixelShader()
{
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

	D3DX11CompileFromFile(shaderFile.c_str(), nullptr, nullptr,
		psName.c_str(), "ps_5_0", flags, 0, nullptr, &pixelBlob, nullptr, nullptr);

	HRESULT hr = DEVICE->CreatePixelShader(pixelBlob->GetBufferPointer(),
		pixelBlob->GetBufferSize(), nullptr, &pixelShader);
	DEBUG(hr);
}

void Shader::CreateGeometryShader(string gsName)
{
	this->gsName = gsName;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

	D3DX11CompileFromFile(shaderFile.c_str(), nullptr, nullptr,
		gsName.c_str(), "gs_5_0", flags, 0, nullptr, &geometryBlob, nullptr, nullptr);

	HRESULT hr = DEVICE->CreateGeometryShader(geometryBlob->GetBufferPointer(),
		geometryBlob->GetBufferSize(), nullptr, &geometryShader);
	DEBUG(hr);
}

void Shader::CreateInputLayout()
{
	HRESULT hr = D3DReflect(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(),
		IID_ID3D11ShaderReflection, (void**)&reflection);
	DEBUG(hr);

	//셰이더 정보 구조체
	D3D11_SHADER_DESC shaderDesc;
	//현재 셰이더 파일에 정보를 가져옴
	reflection->GetDesc(&shaderDesc);

	vector<D3D11_INPUT_ELEMENT_DESC> inputLayouts;

	for (UINT i = 0; i < shaderDesc.InputParameters; i++)
	{
		//hlsl 파일에 시멘틱 정보를 가져옴
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		reflection->GetInputParameterDesc(i, &paramDesc);

		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		string temp = paramDesc.SemanticName;
		int abc = paramDesc.SemanticIndex;

		if (temp == "POSITION")
			elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;

		if (temp == "INSTANCE")
		{
			elementDesc.InputSlot = 1;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
			elementDesc.InstanceDataStepRate = 1;
		}

		inputLayouts.push_back(elementDesc);
	}

	hr = DEVICE->CreateInputLayout(inputLayouts.data(), inputLayouts.size(),
		vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), &inputLayout);
	DEBUG(hr);
}
