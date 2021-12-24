#include "stdafx.h"
#include "GameScene.h"


GameScene::GameScene()
{
	waitTime = 0.0f;
	tribe = 0;
	isMale = 0;
	gameSpeed = 1;
}


GameScene::~GameScene()
{
}

HRESULT GameScene::init()
{
	D3DXMatrixTranslation(&T, WINSIZEX / 2.0f, WINSIZEY / 2.0f, 0);

	apart = new Apart();
	apart->init();

	btn = new Button("gamespeed_0" + to_string(1) + "_on", D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(56.0f, 46.0f));

	sunMove = 90.0f;
	playerGold = 1000;
	gameTime = 60 * 60 * 12.8f;
	soundName = "game_sound01";

	backColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	currentQ = nullptr;

	CHARACTERMANAGER->AddMonster("01", (int)GENDER_MALE, (int)TRIBE_PIYO_PIYO);
	

	//TIMEMANAGER->SetFrame(FIXFRAME);
	return S_OK;
}

void GameScene::release()
{
	apart->release();
}

void GameScene::update()
{
	if(!questRender && !apart->GetIsRoomRender())
		kingDoor->Update();

	for (int i = 0; i < gameSpeed; i++)
	{
		if (g_ETime > 0.1f)
		{
			TIMEMANAGER->ResetTime(0.0f);
		}

		if (!questRender && !kingDoor->InKingRoom()[0])
			apart->update();

		if (gameTime / 60.0f / 60.0f >= 6.5f && gameTime / 60.0f / 60.0f <= 19.5f)
			CreateMonster();

		if (!apart->GetIsRoomRender() && !questRender && !kingDoor->InKingRoom()[0])
		{
			sunMove += 1.84f * g_ETime;

			if (sunMove >= 360.0f)
				sunMove = 0.0f;

			CHARACTERMANAGER->UpdateAll();

			RoomSet();

			if (currentQ != nullptr)
			{
				currentQ->SoundTime();

				if(gameTime / 60.0f / 60.0f >= 6.5f && gameTime / 60.0f / 60.0f <= 19.5f)
					currentQ->Update();
			}
				

			GameTime();
		}


		if (currentQ == nullptr && QUESTMANAGER->GetOrderQuest())
		{
			currentQ = QUESTMANAGER->GetOrderQuest();
			currentQ->SetClear(QUEST_PROGRESS);
		}
		else if (currentQ != nullptr)
			questRender = false;
	}

	apart->IsUpgrade();
	SoundControl();

	//cout << HOUR << " " << MINUTE << " " << SECOEND << endl;
}

void GameScene::render()
{
	Background();

	//아파트 랜더
	apart->render();
	kingDoor->Render();

	CHARACTERMANAGER->RenderAll();

	//아파트 복도 랜더
	apart->RoukaRender();
	//방내부 랜더
	apart->RoomRender();

	if (questRender)
	{
		img = IMGMANAGER->GetImage("kanban_window");
		img->Scale(D3DXVECTOR2(WINSIZEX / 10.0f * 8.0f, WINSIZEY));
		img->Position(D3DXVECTOR2(WINSIZEX / 2.0f, WINSIZEY / 2.0f));
		img->Render();

		QUESTMANAGER->RenderFive(0);
	}

	EnemyView();
	img = IMGMANAGER->GetImage("menu_bar");
	img->Scale(D3DXVECTOR2(WINSIZEX, 82.0f));
	img->Position(D3DXVECTOR2(WINSIZEX / 2.0f, 41.0f));
	img->Render();

	if (!questRender)
	{
		if (!apart->GetIsRoomRender() && !kingDoor->InKingRoom()[0])
		{
			MoneyRender();
			GameSpeedCon();
		}
	}

	kingDoor->KingRoomInside(apart->GetApartStep());
}

void GameScene::EnemyView()
{
	if (currentQ != nullptr)
	{
		if (CHARACTERMANAGER->GetAllBrave().size() > 0)
		{
			if (CHARACTERMANAGER->GetAllBrave().at(0)->GetTime() > 1.5f)
			{
				pos.x -= 500.0f * g_ETime;
				img = IMGMANAGER->GetImage("warning");
				img->Scale(D3DXVECTOR2(2040.0f, 350.0f));
				img->Position(pos);
				img->Render();
			}
		}
		else
		{
			if (currentQ->GetTime() > 0.0f)
			{
				if (currentQ->GetClear() == QUEST_CLEAR)
				{
					img = IMGMANAGER->GetImage("quest_win");
					img->Scale(D3DXVECTOR2(1280.0f, 600.0f));
					img->Position(WINSIZEX / 2.0f, WINSIZEY / 2.0f);
					img->Render();
				}

				if (currentQ->GetClear() == QUEST_FAIL)
				{
					img = IMGMANAGER->GetImage("quest_lose");
					img->Scale(D3DXVECTOR2(1280.0f, 600.0f));
					img->Position(WINSIZEX / 2.0f, WINSIZEY / 2.0f);
					img->Render();
				}
			}
			else if (currentQ->GetTime() <= 0.0f &&
				currentQ->GetClear() != QUEST_PROGRESS)
			{
				if(currentQ->GetClear() != QUEST_CLEAR)
					currentQ->SetClear(QUEST_NONE);

				currentQ = nullptr;
			}

			pos = D3DXVECTOR2(WINSIZEX * 2.0f, WINSIZEY / 2.0f);
		}
	}
}

void GameScene::CreateMonster()
{
	if (waitTime <= 0.0f)
		randomTime = GetFloat(20.0f);

	if (currentQ != nullptr)
	{
		if (currentQ->GetTime() < 35.0f && CHARACTERMANAGER->GetAllBrave().size() <= 0)
		{
			createMon = true;
		}
		else
			createMon = false;
	}
	else if (currentQ == nullptr)
	{
		createMon = true;
	}

	if (createMon && CHARACTERMANAGER->GetSameStateMonsters(STATE_FIND_HOUSE).size() < 2)
	{
		waitTime += g_ETime;

		if (waitTime >= randomTime)
		{

			tribe = GetInt(7) * 100;
			isMale = GetInt(2) + 1;

			tribe += rand() % (apart->GetApartStep() + 1);

			waitTime = 0.0f;
			CHARACTERMANAGER->AddMonster("01", isMale, tribe);

			CHARACTERMANAGER->DeleteDeadCTR();
		}
	}
}

void GameScene::MoneyRender()
{
	img = IMGMANAGER->GetImage("money_window");
	img->Scale(D3DXVECTOR2(WINSIZEX / 5.0f, 40.0f));
	img->Position(D3DXVECTOR2(img->GetScale().x / 2.0f, 80.0f));
	img->Color(0.0f, 0.0f, 0.0f, 1.0f);
	img->Render();

	RECT frc = { WINSIZEX / 10 - (to_wstring(playerGold).size() * 10) + 100, WINSIZEY - 90, WINSIZEX,  WINSIZEY };
	WRITEMANAGER->RenderText(to_wstring(playerGold) + L"G", frc, 20.0f, L"돋움체", D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), DWRITE_FONT_WEIGHT_HEAVY);
}

void GameScene::GameTime()
{
	if (gameTime / 60.0f / 60.0f >= 24.0f)
		gameTime = 0.0f;

	gameTime += g_ETime * 300.0f;

	if (gameTime / 60.0f / 60.0f <= 17.0f && gameTime / 60.0f / 60.0f >= 7.0f)
	{
		BackColorChange(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		if (gold)
		{
			apart->MonthGoldSupply();
			gold = false;
		}
	}

	if (gameTime / 60.0f / 60.0f > 17.0f && gameTime / 60.0f / 60.0f <= 20.0f)
		BackColorChange(D3DXCOLOR(1.0f, 0.75f, 0.75f, 1.0f));

	if (gameTime / 60.0f / 60.0f <= 6.0f || gameTime / 60.0f / 60.0f > 20.0f)
	{
		BackColorChange(D3DXCOLOR(0.5f, 0.5f, 0.7f, 1.0f));

		gold = true;
	}

	if (gameTime / 60.0f / 60.0f > 7.0f && gameTime / 60.0f / 60.0f < 7.1f)
	{
		changeSun = true;
		sunMove = 335.0f;
	}

	if (gameTime / 60.0f / 60.0f > 20.5f && gameTime / 60.0f / 60.0f < 20.6f)
	{
		changeSun = false;
		sunMove = 335.0f;
	}
}

void GameScene::BackColorChange(D3DXCOLOR color)
{
	if (backColor.r != color.r)
	{
		if (backColor.r < color.r)
		{
			backColor.r += 0.02f * g_ETime;
		}
		else
			backColor.r -= 0.02f * g_ETime;
	}

	if (backColor.g != color.g)
	{
		if (backColor.g < color.g)
		{
			backColor.g += 0.02f * g_ETime;
		}
		else
			backColor.g -= 0.02f * g_ETime;
	}

	if (backColor.b != color.b)
	{
		if (backColor.b < color.b)
		{
			backColor.b += 0.02f * g_ETime;
		}
		else
			backColor.b -= 0.02f * g_ETime;
	}
}

void GameScene::GameSpeedCon()
{
	for (int i = 1; i < 4; i++)
	{
		if (i == gameSpeed)
		{
			btn->SetImgName("gamespeed_0" + to_string(i) + "_on");
			btn->SetPosition(D3DXVECTOR2(WINSIZEX - 302 + (i * 87.0f), 23.0f));
			btn->Update();
		}
		else
		{
			btn->SetImgName("gamespeed_0" + to_string(i) + "_off");
			btn->SetPosition(D3DXVECTOR2(WINSIZEX - 302 + (i * 87.0f), 23.0f));
			btn->Update();

			if (btn->IsKeyDown(VK_LBUTTON))
			{
				gameSpeed = i;
			}
		}

		btn->Render();
	}

	if (KEYMANAGER->isKeyDown('1'))
	{
		gameSpeed = 1;
	}

	if (KEYMANAGER->isKeyDown('2'))
	{
		gameSpeed = 2;
	}

	if (KEYMANAGER->isKeyDown('3'))
	{
		gameSpeed = 3;
	}
}

void GameScene::SunAndMoon()
{
	if (!changeSun)
		img = IMGMANAGER->GetImage("moon");
	else
		img = IMGMANAGER->GetImage("sun");

	img->Scale(D3DXVECTOR2(150.0f, 150.0f));
	img->Position(D3DXVECTOR2(540.0f * cosf(sunMove * TORADIAN), 280.0f * sinf(sunMove * TORADIAN)));
	img->Color(0.0f, 0.0f, 0.0f, 1.0f);
	img->SetParent(&T);

	if (CHARACTERMANAGER->GetAllBrave().size() > 0)
		img->Render(0, 1 / 3.0f * 2.0f, 1, 1);
	else
		img->Render();
}

void GameScene::Background()
{
	img = IMGMANAGER->GetImage("sky");
	img->Scale(D3DXVECTOR2(WINSIZEX, WINSIZEY / 3 * 2));
	img->Position(D3DXVECTOR2(WINSIZEX / 2, img->GetScale().y));
	img->Color(backColor);
	img->Render();

	SunAndMoon();

	img = IMGMANAGER->GetImage("field");
	img->Scale(D3DXVECTOR2(WINSIZEX, WINSIZEY / 7 * 4));
	img->Position(D3DXVECTOR2(WINSIZEX / 2, img->GetScale().y / 2));
	img->Color(backColor);
	img->Render();

	img = IMGMANAGER->GetImage("kanban");
	img->Scale(D3DXVECTOR2(100.0f, 80.0f));
	img->Position(D3DXVECTOR2(WINSIZEX / 7 * 2 + 30, 190.0f));
	img->Color(backColor);
	img->Render();

	if (!apart->GetIsRoomRender() && !kingDoor->InKingRoom()[0])
	{
		if (!questRender && currentQ == nullptr)
		{
			if (KEYMANAGER->isKeyDown(VK_LBUTTON) &&
				PtInRect(img->GetRect(), g_ptMouse))
				questRender = true;
		}

		if (KEYMANAGER->isKeyDown(VK_RBUTTON) &&
			questRender)
			questRender = false;

		if (PtInRect(img->GetRect(), g_ptMouse) && currentQ != nullptr)
		{
			img = IMGMANAGER->GetImage("window_kanban");
			img->Scale(D3DXVECTOR2(320.0f, 250.0f));
			img->Position(D3DXVECTOR2(WINSIZEX / 5, 650.0f));
			img->Color(1.0f, 1.0f, 1.0f, 1.0f);
			img->Render();

			RECT frc = { WINSIZEX / 5 - 50, 650, WINSIZEX, 142 };
			WRITEMANAGER->RenderText(currentQ->GetQuestName(), frc, 16.0f, L"돋움체", D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), DWRITE_FONT_WEIGHT_HEAVY);

			img = IMGMANAGER->GetImage("star");
			img->Scale(D3DXVECTOR2(189.0f, 27.0f));
			img->Position(D3DXVECTOR2(WINSIZEX / 5.0f + 40.0f, 527.0f));
			img->SetCurrentFrameY(currentQ->GetQuestDif());
			img->Render();

			frc = { WINSIZEX / 5 - 50, 650, WINSIZEX,  225 };
			WRITEMANAGER->RenderText(L"곧 " + to_wstring(currentQ->GetCurrentWave()) + L"차전(총 " + to_wstring(currentQ->GetendWave()) + L"전)",
				frc, 16.0f, L"돋움체", D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), DWRITE_FONT_WEIGHT_HEAVY);
		}
	}
}

void GameScene::RoomSet()
{
	Character* ctr = CHARACTERMANAGER->GetStateMonster((int)STATE_LIKE_ROOM);

	for (int i = 0; i < (apart->GetApartStep() + 1) * 4; i++)
	{
		if (ctr != nullptr && apart->emptyRoom() && apart->GetEveryRoom().at(i)->GetMonsterName() == "" &&
			PtInRect(ctr->GetColboxRect(), g_ptMouse))
		{
			if (KEYMANAGER->isKeyDown(VK_LBUTTON))
			{
				apart->GetEveryRoom().at(i)->SetLiveMonster(ctr);
				ctr->SetTime(0.0f);
				ctr->SetState(STATE_GOTO_HOME);
				break;
			}
		}
		else if (ctr != nullptr && !apart->emptyRoom())
		{
			ctr->SetTime(0.0f);
			ctr->SetState(STATE_KICK);
		}
	}
}

void GameScene::SoundControl()
{
	if (CHARACTERMANAGER->GetAllBrave().size() > 0)
	{
		SOUNDMANAGER->stop(soundName);

		if (currentQ->GetSoundTime() > 1.5f)
		{
			SOUNDMANAGER->play("warning_sound", 0.3f);

			gameSpeed = 1;
		}
		else
		{
			SOUNDMANAGER->stop("warning_sound");
			SOUNDMANAGER->play("battle_sound", 0.3f);
		}
	}

	if (!SOUNDMANAGER->isPlaySound(soundName) && CHARACTERMANAGER->GetAllBrave().size() <= 0)
	{
		SOUNDMANAGER->stop("battle_sound");

		randomSound = GetInt(4);

		if (randomSound == 0)
			soundName = "game_sound01";
		if (randomSound == 1)
			soundName = "game_sound02";
		if (randomSound == 2)
			soundName = "game_sound03";
		if (randomSound == 3)
			soundName = "game_sound04";

		SOUNDMANAGER->play(soundName, 0.3f);
	}
}
