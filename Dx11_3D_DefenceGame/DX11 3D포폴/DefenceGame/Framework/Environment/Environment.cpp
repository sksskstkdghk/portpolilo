#include "Framework.h"

Environment* Environment::instance = nullptr;

Environment::Environment()
{
	perspective = new Perspective();
	viewport = new Viewport();

	viewProjection = new ViewProjectionBuffer();
	viewProjection->SetProjection(perspective->GetMatrix());

	light = new LightBuffer();
	mainCamera = new Freedom();
	mainCamera->SetPos(0, 0, 0);
	mainCamera->SetRotation(D3DXToRadian(60.0f), 0.0f);
}

Environment::~Environment()
{
	delete perspective;
	delete viewport;

	delete viewProjection;
	delete light;

	delete mainCamera;
}
