#pragma once

class Transform
{
public:
	string tag;

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	Vector3 forward;
	Vector3 right;
	Vector3 up;

	Vector3 worldPosition;
protected:
	Vector3 pivot;

	Matrix world;
	Matrix* parent;
	
	WorldBuffer* worldBuffer;

public:
	Transform(string tag = "Untagged");
	virtual ~Transform();

	void UpdateWorld();
	void SetVS(UINT slot = 1);

	Matrix GetWorld() { return world; }
	Matrix GetTransposeWorld() { return worldBuffer->GetWorld(); }
};