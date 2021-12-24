#pragma once

class Material
{
private:
	string name;

	Shader* shader;

	MaterialBuffer* buffer;

	Texture* diffuseMap;
	Texture* specularMap;
	Texture* normalMap;

public:
	Material();
	Material(wstring shaderFile);
	Material(Shader* shader);
	~Material();

	void Set();

	void SetShader(Shader* value) { shader = value; }
	void SetShader(wstring file);
	Shader* GetShader() { return shader; }

	MaterialBuffer* GetBuffer() { return buffer; }

	void SetDiffuseMap(wstring file);
	Texture* GetDiffuseMap() { return diffuseMap; }

	void SetSpecularMap(wstring file);
	Texture* GetSpecularMap() { return specularMap; }

	void SetNormalMap(wstring file);
	Texture* GetNormalMap() { return normalMap; }

	string Name() { return name; }
	void Name(string value) { name = value; }
};