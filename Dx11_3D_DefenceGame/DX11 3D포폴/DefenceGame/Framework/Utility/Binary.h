#pragma once

class BinaryWriter
{
private:
	HANDLE file;
	DWORD size;

public:
	BinaryWriter(wstring filePath);
	~BinaryWriter();

	void Int(int data);
	void UInt(UINT data);
	void Float(float data);
	void String(string data);
	void Matrix(D3DXMATRIX data);

	void Byte(void* data, UINT dataSize);
};

////////////////////////////////////////////////////////////////////////////

class BinaryReader
{
private:
	HANDLE file;
	DWORD size;

public:
	BinaryReader(wstring filePath);
	~BinaryReader();

	int Int();
	UINT UInt();
	float Float();
	string String();
	D3DXMATRIX Matrix();

	void Byte(void** data, UINT dataSize);
};