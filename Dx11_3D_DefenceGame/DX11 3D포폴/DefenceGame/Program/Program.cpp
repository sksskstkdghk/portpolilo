#include "Framework.h"

Program::Program()
{
	USER->Create();
	WRITE->Create();
	SOUND->Create();

	SOUND->Init();

	scenes.push_back(new GameScene());

	isRender = -1;

	//SOUND->PlaySound("Sound/BGM/BGM01.mp3");
	//SOUND->PlaySound("Sound/BGM/BGM02.mp3");

	Environment::Get()->GetViewport()->Set();
}

Program::~Program()
{
	for (Scene* scene : scenes)
		delete scene;

	USER->Delete();
	WRITE->Delete();

	SOUND->Delete();
}

void Program::Update()
{
	VP->SetView(CAMERA->GetView());
	VP->SetProjection(Environment::Get()->GetPerspective()->GetMatrix());

	for (Scene* scene : scenes)
		scene->Update();

	USER->Update();

	CAMERA->Update();
	SOUND->Update();

	wheelValue = 0;

	if (KEYDOWN('Q'))
		isRender *= -1;
}

void Program::PreRender()
{
	//VP->SetView(CAMERA->GetView());
	//VP->SetProjection(Environment::Get()->GetPerspective()->GetMatrix());
	VP->SetVSBuffer(0);
	LIGHT->SetPSBuffer(0);

	for (Scene* scene : scenes)
		scene->PreRender();
}

void Program::Render()
{
	CAMERA->GetCulling()->Make(&(CAMERA->GetView() * PERSPECTIVE->GetMatrix()));

	for (Scene* scene : scenes)
		scene->Render();
}

void Program::PostRender()
{
	if (isRender < 0)
	{
		ASTAR->Render();

		ImGui::SetWindowFontScale(IMGUI_FONT_SCALE_SIZE);

		ImGui::Text("FPS : %d", (int)Timer::Get()->FPS());
		ImGui::SliderFloat("cullingSize", &cullingSize, -5.0f, 1.0f);
		//ImGui::Separator();
		//ImGui::SliderFloat3("CamPos", CAMERA->GetPos(), -1000, 1000);
		//ImGui::SliderFloat3("mPos", mousePos, -1000, 2000);
		//ImGui::SliderFloat2("CamRot", CAMERA->GetRotation(), -1000, 1000);
		//CAMERA->PostRender();
		//ImGui::Separator();
		//ImGui::SliderFloat3("LightDirection", LIGHT->data.direction, -1, 1);
		//ImGui::SliderFloat("SpecularExp", &LIGHT->data.specExp, 0, 20.0f);
		//ImGui::ColorEdit4("Ambient", LIGHT->data.ambient);
		//bool temp = LIGHT->data.isSpecularMap;
		//ImGui::Checkbox("IsSpecularMap", &temp);
		//LIGHT->data.isSpecularMap = temp;
		//temp = LIGHT->data.isNormalMap;
		//ImGui::Checkbox("IsNormalMap", &temp);
		//LIGHT->data.isNormalMap = temp;

		for (Scene* scene : scenes)
			scene->PostRender();
	}

	USER->PostRender();
}
