#pragma once

class FrustumCulling;

struct Ray
{
	Vector3 position;
	Vector3 direction;
};

class Camera
{
protected:
	Vector3 position;
	Vector2 rotation;

	Vector3 forward;
	Vector3 right;
	Vector3 up;

	Matrix matRotation;
	Matrix matView;

	Vector3 oldPos;

	FrustumCulling* frustum;
public:
	Camera();
	virtual ~Camera();

	virtual void Update();
	virtual void Move();
	virtual void Rotation();
	virtual void View();

	virtual void PostRender() {}
	virtual void SetTarget(Transform* target) {}

	Ray GetPickingRay();

	void SetRotation(float x, float y);
	Vector2 GetRotation() { return rotation; }

	Vector3 GetPos() { return position; }
	void SetPos(float x, float y, float z) { position = Vector3(x, y, z); }
	
	Matrix GetView() { return matView; }

	FrustumCulling* GetCulling() { return frustum; }
};