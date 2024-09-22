#include "Framework.h"

GameScene::GameScene()
{
	//testp = new AModelAnimator(Shader::Add(L"ModelAnimationInstancing"));

	mapWidth = 50;
	mapHeight = 50;

	//Export();

	SPAWN->Create();
	TERRAIN->Create();
	ASTAR->Create(mapWidth, mapHeight);
	MAZE->Create();

	CAMERA->SetPos(mapWidth / 2, 15, mapHeight / 4);

	castle = new Castle();
	TERRAIN->SetCastle();

	CHARACTER->Create();
}

GameScene::~GameScene()
{
	delete castle;
	ASTAR->Delete();
	TERRAIN->Delete();
	SPAWN->Delete();
	CHARACTER->Delete();
	MAZE->Delete();
}

void GameScene::Update()
{
	castle->Update();
	TERRAIN->Update();
	ASTAR->Update();
	SPAWN->Update();
	CHARACTER->Update();
}

void GameScene::PreRender()
{
}

void GameScene::Render()
{
	castle->Render();
	TERRAIN->Render();
	SPAWN->Render();
	CHARACTER->Render();
}

void GameScene::PostRender()
{
	ImGui::Begin("test");

	TERRAIN->PostRender();
	CHARACTER->PostRender();

	ImGui::End();

	//ASTAR->Render();
}

void GameScene::Export()
{
	string name = "pikachu";

	AssimpReader* reader = new AssimpReader("FbxData/Models/" + name + ".fbx");
	reader->ExporterMaterial(name + "/" + name);
	reader->ExporterMesh(name + "/" + name);
	delete reader;

	reader = new AssimpReader("FbxData/Animations/" + name + "/Idle.fbx");
	reader->ExporterAnimClip(0, name + "/Idle");
	delete reader;
	reader = new AssimpReader("FbxData/Animations/" + name + "/Run.fbx");
	reader->ExporterAnimClip(0, name + "/Run");
	delete reader;
	reader = new AssimpReader("FbxData/Animations/" + name + "/Attack.fbx");
	reader->ExporterAnimClip(0, name + "/Attack");
	delete reader;
}
