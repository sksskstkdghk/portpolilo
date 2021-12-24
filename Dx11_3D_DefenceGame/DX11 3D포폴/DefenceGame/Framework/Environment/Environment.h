#pragma once

class Environment
{
private:
	Viewport* viewport;
	Perspective* perspective;

	ViewProjectionBuffer* viewProjection;
	LightBuffer* light;

	Camera* mainCamera;

	static Environment* instance;

	Environment();
	~Environment();
public:
	static Environment* Get() { return instance; }
	static void Create() { instance = new Environment(); }
	static void Delete() { delete instance; }

	ViewProjectionBuffer* GetVP() { return viewProjection; }
	LightBuffer* GetLight() { return light; }
	Camera* GetCamera() { return mainCamera; }
	Viewport* GetViewport() { return viewport; }
	Perspective* GetPerspective() { return perspective; }
};