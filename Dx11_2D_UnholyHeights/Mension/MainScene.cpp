#include "stdafx.h"
#include "MainScene.h"


MainScene::MainScene()
{
}


MainScene::~MainScene()
{
}

HRESULT MainScene::init()
{
	/*img2 = IMGMANAGER->GetImage("start");
	img2->Scale(220.0f, 40.0f);
	img2->Position(WINSIZEX / 2.0f, 250.0f);

	rc = new Rect();
	rc->Scale(160.0f, 40.0f);
	rc->Position(WINSIZEX / 2.0f, 250.0f);
	rc->SetisFill(false);*/

	button = new Button("start", D3DXVECTOR2(WINSIZEX / 2.0f, 250.0f),
		D3DXVECTOR2(220.0f, 40.0f), D3DXVECTOR2(160.0f, 40.0f));
	button->Init();

	return S_OK;
}

void MainScene::release()
{
}

void MainScene::update()
{
	SOUNDMANAGER->play("main_sound", 0.3f);

	button->Update();

	if (button->MouseInColBox())
		button->SetImgScale(D3DXVECTOR2(240.0f, 50.0f));
	else
		button->SetImgScale(D3DXVECTOR2(220.0f, 40.0f));

	if (button->IsKeyDown(VK_LBUTTON))
	{
		SCENEMANAGER->ChangeScene("Loading");
		SCENEMANAGER->CurrentSceneUpdate();
		SOUNDMANAGER->stop("main_sound");
	}
}

void MainScene::render()
{
	img = IMGMANAGER->GetImage("title_back");
	img->Scale(WINSIZEX, WINSIZEY);
	img->Position(WINSIZEX / 2.0f, WINSIZEY / 2.0f);
	img->Render();

	img = IMGMANAGER->GetImage("title");
	img->Scale(WINSIZEX, WINSIZEY);
	img->Position(WINSIZEX / 2.0f, WINSIZEY / 2.0f);
	img->Render();

	img = IMGMANAGER->GetImage("logo");
	img->Scale(WINSIZEX / 2.0f, WINSIZEY / 2.0f);
	img->Position(WINSIZEX / 2.0f, WINSIZEY / 3.0f * 2);
	img->Render();

	button->Render();
}
