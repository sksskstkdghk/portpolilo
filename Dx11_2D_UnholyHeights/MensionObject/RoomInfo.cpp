#include "stdafx.h"
#include "RoomInfo.h"


RoomInfo::RoomInfo()
{
	img = IMGMANAGER->GetImage("room_window");
	liveMonster = nullptr;
	choiceFurni = nullptr;

	Init();
	rc = new Rect(D3DXVECTOR2(0.0f, -0.5f), D3DXVECTOR2(1.0f, 0.5f));
	GoldPM = new Button("plus", D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(22.0f, 25.0f));

	time = 0.0f;
}


RoomInfo::~RoomInfo()
{
}

void RoomInfo::Init()
{
	for (int i = 0; i < 9; i++)
	{
		int a;	//가구 갯수
		string name;	//가구 이름

		switch (i)
		{
			case 0: //벽지
				a = 9;
				name = "wallpaper";
			break;
			case 1: //침대
				a = 6;
				name = "bed";
				break;
			case 2: //냉장고
				a = 5;
				name = "kitchen";
				break;
			case 3: //화장실
				a = 6;
				name = "bath";
				break;
			case 4: //냉온방기
				a = 5;
				name = "air";
				break;
			case 5: //오디오
				a = 4;
				name = "audio";
				break;
			case 6: //텔레비전
				a = 5;
				name = "tv";
				break;
			case 7: //책상
				a = 4;
				name = "desk";
				break;
			case 8: //컴퓨터
				a = 5;
				name = "computer";
				break;
		}

		furni.emplace_back(new Furniture(D3DXVECTOR2(i % 4, i / 4), D3DXVECTOR2(300.0f + i % 4 * 60.0f, 280.0f - i / 4 * 60.0f)));

		for (int j = 0; j < a; j++)
			furni.at(i)->allFurniture.emplace_back(new Button(name, D3DXVECTOR2(650.0f + (j % 5 * 100.0f), 500.0f - (j / 5 * 100.0f)), D3DXVECTOR2(70.0f, 70.0f)));
	}

	Price();

	furni.at(0)->currentFur->SetImgName("wallpaper");
	furni.at(0)->frameX = 0;
	furni.at(0)->frameY = 0;

	furni.at(1)->currentFur->SetImgName("bed");
	furni.at(1)->frameX = 0;
	furni.at(1)->frameY = 0;

	furni.at(2)->currentFur->SetImgName("kitchen");
	furni.at(2)->frameX = 0;
	furni.at(2)->frameY = 0;

	furni.at(3)->currentFur->SetImgName("bath");
	furni.at(3)->frameX = 0;
	furni.at(3)->frameY = 0;

	monthGold = 15;
}

void RoomInfo::ClickFurni()
{
	for (int i = 0; i < furni.size(); i++)
	{
		if (furni.at(i)->currentFur->MouseInColBox())
		{
			/*img = IMGMANAGER->GetImage("furniture");
			img->Scale(WINSIZEX / 10.0f * 4.0f, WINSIZEY / 10.0f * 3.0f);
			img->Position(WINSIZEX / 3.0f * 2.0f, WINSIZEY / 10.0f * 3.0f);
			img->Render();

			img = IMGMANAGER->GetImage("sel_des");
			img->Scale(WINSIZEX / 10.0f * 4.0f, WINSIZEY / 10.0f * 3.0f);
			img->Position(WINSIZEX / 3.0f * 2.0f, WINSIZEY / 3.0f * 2.0f);
			img->Render();*/

			img = IMGMANAGER->GetImage("cursor_orange");
			img->Scale(furni.at(i)->currentFur->GetImgScale());
			img->Position(furni.at(i)->currentFur->GetImgPosition());
			img->Render();

			if (KEYMANAGER->isKeyDown(VK_LBUTTON))
			{
				choiceFurni = furni.at(i);
				select = D3DXVECTOR2(i % 4, i / 4);
				break;
			}
		}
	}
}

void RoomInfo::IsChangeFurni()
{
	//선택된 방 가구가 없다면
	if (choiceFurni == nullptr)
	{
		//선택할 수 있음
		ClickFurni();
	}

	//있다면
	if (choiceFurni != nullptr)
	{

		RECT frc = { WINSIZEX / 2 , WINSIZEY / 10 * 2 - 16, WINSIZEX,  WINSIZEY};
		WRITEMANAGER->RenderText(choiceFurni->name, frc, 20.0f, L"돋움체", D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), DWRITE_FONT_WEIGHT_NORMAL);

		//가구 설명서
		/*img = IMGMANAGER->GetImage("furniture");
		img->Scale(WINSIZEX / 10.0f * 4.0f, WINSIZEY / 10.0f * 3.0f);
		img->Position(WINSIZEX / 3.0f * 2.0f, WINSIZEY / 10.0f * 3.0f);
		img->Render();*/

		//선택된 가구 목
		img = IMGMANAGER->GetImage("furni_icon");
		img->Scale(50.0f, 50.0f);
		img->Position(WINSIZEX / 3.0f * 1.4f, WINSIZEY / 10.0f * 8.0f);
		img->Render(select.x * 0.25f, select.y * 0.2f, (select.x + 1.0f) * 0.25f, (select.y + 1.0f) * 0.2f);

		//사는로고
		img = IMGMANAGER->GetImage("furniture_title");
		img->Scale(209.0f, 54.0f);
		img->Position(WINSIZEX / 3.0f * 1.55f, WINSIZEY / 10.0f * 8.9f);
		img->Render();

		for (int i = 0; i < choiceFurni->allFurniture.size(); i++)
		{
			//선택된 가구 세부품목 랜더
			choiceFurni->allFurniture.at(i)->Update();
			choiceFurni->allFurniture.at(i)->RenderFrame(i % 5, i / 5);

			//구비된 가구 위치
			if (choiceFurni->currentFur->GetImgName() == choiceFurni->allFurniture.at(i)->GetImgName() &&
				choiceFurni->frameX == i % 5 && choiceFurni->frameY == i / 5)
			{
				img = IMGMANAGER->GetImage("cursor_green");
				img->Scale(choiceFurni->allFurniture.at(i)->GetImgScale());
				img->Position(choiceFurni->allFurniture.at(i)->GetImgPosition());
				img->Render();
			}

			//선택된 가구 세부품목 마우스 위치 충돌시 커서 색 변경
			if (choiceFurni->allFurniture.at(i)->MouseInColBox())
			{
				img = IMGMANAGER->GetImage("cursor_orange");
				img->Scale(choiceFurni->allFurniture.at(i)->GetImgScale());
				img->Position(choiceFurni->allFurniture.at(i)->GetImgPosition());
				img->Render();

				//선택된 가구가 존재하면 되팔기 가격 보여줌
				if (choiceFurni->currentFur->GetImgName() != "furni_icon")
				{
					LONG size = to_wstring(choiceFurni->furniPrice.at(choiceFurni->frameX + choiceFurni->frameY * 5) / 2).size();

					frc = { WINSIZEX / 10 * 7 - 48, WINSIZEY / 10 * 1 + 30, WINSIZEX,  WINSIZEY };
					WRITEMANAGER->RenderText(L"되팔기", frc, 24.0f, L"돋움체",
						D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), DWRITE_FONT_WEIGHT_HEAVY);

					frc = { WINSIZEX / 10 * 7 - (size * 12) + 160, WINSIZEY / 10 * 1 + 30, WINSIZEX,  WINSIZEY };
					WRITEMANAGER->RenderText(to_wstring(choiceFurni->furniPrice.at(choiceFurni->frameX + choiceFurni->frameY * 5) / 2) + L"G", frc, 24.0f, L"돋움체",
						D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), DWRITE_FONT_WEIGHT_HEAVY);
				}
				

				//구비된 가구 선택시 판매
				if (choiceFurni->currentFur->GetImgName() == choiceFurni->allFurniture.at(i)->GetImgName() &&
					choiceFurni->frameX == i % 5 && choiceFurni->frameY == i / 5)
				{

					if (KEYMANAGER->isKeyDown(VK_LBUTTON))
					{
						playerGold += choiceFurni->furniPrice.at(i) / 2;
						choiceFurni->currentFur->SetImgName("furni_icon");
					}
				}
				//다른 가구 선택시 구비된 가구 판매 후 새로 구입
				else if (choiceFurni->frameX != i % 5 || choiceFurni->frameY != i / 5 ||
					choiceFurni->currentFur->GetImgName() == "furni_icon")
				{
					int buy; //상황따른 구매가격

					//선택된 가구가 구비된 가구면 구매가격에서 되팔기 가격 뺀 상태로 출력
					if(choiceFurni->currentFur->GetImgName() != "furni_icon")
						buy = choiceFurni->furniPrice.at(i) - choiceFurni->furniPrice.at(choiceFurni->frameX + choiceFurni->frameY * 5) / 2;
					else
						buy = choiceFurni->furniPrice.at(i);

					frc = { WINSIZEX / 10 * 7 - 72, WINSIZEY / 10 + 60, WINSIZEX,  WINSIZEY };
					WRITEMANAGER->RenderText(L"구매가격", frc, 24.0f, L"돋움체",
						D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), DWRITE_FONT_WEIGHT_HEAVY);

					frc = { WINSIZEX / 10 * 7 - ((LONG)to_wstring(buy).size() * 12) + 160, WINSIZEY / 10 + 60, WINSIZEX,  WINSIZEY };
					WRITEMANAGER->RenderText(to_wstring(buy) + L"G", frc, 24.0f, L"돋움체",
						D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), DWRITE_FONT_WEIGHT_HEAVY);

					if (KEYMANAGER->isKeyDown(VK_LBUTTON))
					{
						if (playerGold >= buy)
						{
							choiceFurni->currentFur->SetImgName(choiceFurni->allFurniture.at(i)->GetImgName());
							choiceFurni->frameX = i % 5;
							choiceFurni->frameY = i / 5;

							playerGold -= buy;
						}
						else
						{
							//불가능 효과음

						}
					}
				}
			}
		}
		
		if (KEYMANAGER->isKeyDown(VK_RBUTTON))
			choiceFurni = nullptr;
	}
}

void RoomInfo::release()
{
	choiceFurni = nullptr;

	for (int i = 0; i < furni.size(); i++)
	{
		furni.at(i)->Release();
	}

	furni.clear();
	furni.shrink_to_fit();
}

void RoomInfo::update()
{
	
}

void RoomInfo::render()
{
	img = IMGMANAGER->GetImage("room_window");
	img->Scale(WINSIZEX / 10.0f * 8.0f, WINSIZEY);
	img->Position(WINSIZEX / 2.0f, WINSIZEY / 2.0f);
	img->Render();

	img = IMGMANAGER->GetImage("room_contents");
	img->Scale(WINSIZEX / 10.0f * 2.5f, WINSIZEY / 10.0f * 7.0f);
	img->Position(WINSIZEX / 10.0f * 2.8f, WINSIZEY / 5.0f * 2.2f);
	img->Render();

	//월세비율
	rc->Scale(134.0f, 24.0f);
	rc->Color(0.0f, 0.0f, 0.0f);
	rc->Position(WINSIZEX / 10.0f * 2.3f - 2.0f, WINSIZEY / 10.0f * 6.0f);
	rc->Render();

	rc->Scale(130.0f, 20.0f);
	rc->Color(1.0f, 1.0f, 1.0f);
	rc->Position(WINSIZEX / 10.0f * 2.3f, WINSIZEY / 10.0f * 6.0f);
	rc->Render();

	rc->Scale(130.0f / 47.0f * monthGold, 20.0f);
	rc->Color(0.0f, 0.7f, 0.58f);
	rc->Position(WINSIZEX / 10.0f * 2.3f, WINSIZEY / 10.0f * 6.0f);
	rc->Render();

	//살고 있다면 표시
	if (liveMonster != nullptr)
	{
		liveMonster->Position(D3DXVECTOR2(300.0f, 530.0f));
		liveMonster->Color(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		liveMonster->Rotate(0.0f, 0.0f, 0.0f);
		liveMonster->Render(0.0f, 0.25f, 0.5f, 0.5f);

		KickLiveMon();
	}

	//가구 인터페이스
	for (int i = 0; i < furni.size(); i++)
	{
		if (furni.at(i)->currentFur->GetImgName() == "furni_icon")
		{
			furni.at(i)->frameX = i % 4;
			furni.at(i)->frameY = i / 4;
		}

		furni.at(i)->currentFur->Update();
		furni.at(i)->Render();
	}

	//가구 구입 및 매각
	IsChangeFurni();
	MonthGoldPM();

	//월세
	RECT frc = { WINSIZEX / 10.0f * 3.7, WINSIZEY / 10 * 3.8, WINSIZEX,  WINSIZEY };
	WRITEMANAGER->RenderText(to_wstring(monthGold) + L"G", frc, 20.0f, L"돋움체", D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), DWRITE_FONT_WEIGHT_HEAVY);
	
	//현재 돈
	frc = { WINSIZEX / 10 * 7 - 24, WINSIZEY / 10 * 1, WINSIZEX,  WINSIZEY };
	WRITEMANAGER->RenderText(L"지갑", frc, 24.0f, L"돋움체", D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), DWRITE_FONT_WEIGHT_NORMAL);

	frc = { WINSIZEX / 10 * 7 - ((LONG)to_wstring(playerGold).size() * 12) + 160, WINSIZEY / 10, WINSIZEX,  WINSIZEY };
	WRITEMANAGER->RenderText(to_wstring(playerGold) + L"G", frc, 24.0f, L"돋움체", D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), DWRITE_FONT_WEIGHT_NORMAL);

	//현재 룸 번호
	frc = { WINSIZEX / 10 * 2 - 80, WINSIZEY / 10 * 1, WINSIZEX,  WINSIZEY };
	WRITEMANAGER->RenderText(to_wstring(roomNum), frc, 30.0f, L"돋움체", D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), DWRITE_FONT_WEIGHT_HEAVY);
}

void RoomInfo::Price()
{
	for (int i = 0; i < 9; i++)
	{
		int a;
		wstring name;

		switch (i)
		{
		case 0: //벽지
			name = L"Wallpaper";
			break;
		case 1: //침대
			name = L"Bedding";
			break;
		case 2: //냉장고
			name = L"Kitchen";
			break;
		case 3: //화장실
			name = L"Bathroom";
			break;
		case 4: //냉온방기
			name = L"Air conditioner";
			break;
		case 5: //오디오
			name = L"Music";
			break;
		case 6: //텔레비전
			name = L"TV";
			break;
		case 7: //책상
			name = L"Desk";
			break;
		case 8: //컴퓨터
			name = L"PC";
			break;
		}

		furni.at(i)->name = name;
	}

	//벽지
	furni.at(0)->furniPrice.emplace_back(10);
	furni.at(0)->furniPrice.emplace_back(100);
	furni.at(0)->furniPrice.emplace_back(250);
	furni.at(0)->furniPrice.emplace_back(300);
	furni.at(0)->furniPrice.emplace_back(320);
	furni.at(0)->furniPrice.emplace_back(400);
	furni.at(0)->furniPrice.emplace_back(450);
	furni.at(0)->furniPrice.emplace_back(500);
	furni.at(0)->furniPrice.emplace_back(750);

	//침대
	furni.at(1)->furniPrice.emplace_back(20);
	furni.at(1)->furniPrice.emplace_back(200);
	furni.at(1)->furniPrice.emplace_back(500);
	furni.at(1)->furniPrice.emplace_back(800);
	furni.at(1)->furniPrice.emplace_back(2000);
	furni.at(1)->furniPrice.emplace_back(10000);

	//주방
	furni.at(2)->furniPrice.emplace_back(100);
	furni.at(2)->furniPrice.emplace_back(400);
	furni.at(2)->furniPrice.emplace_back(800);
	furni.at(2)->furniPrice.emplace_back(1500);
	furni.at(2)->furniPrice.emplace_back(5500);

	//화장실
	furni.at(3)->furniPrice.emplace_back(80);
	furni.at(3)->furniPrice.emplace_back(300);
	furni.at(3)->furniPrice.emplace_back(1000);
	furni.at(3)->furniPrice.emplace_back(1200);
	furni.at(3)->furniPrice.emplace_back(3000);
	furni.at(3)->furniPrice.emplace_back(6500);

	//냉, 온방기
	furni.at(4)->furniPrice.emplace_back(120);
	furni.at(4)->furniPrice.emplace_back(500);
	furni.at(4)->furniPrice.emplace_back(1000);
	furni.at(4)->furniPrice.emplace_back(1500);
	furni.at(4)->furniPrice.emplace_back(5000);

	//오디오
	furni.at(5)->furniPrice.emplace_back(50);
	furni.at(5)->furniPrice.emplace_back(250);
	furni.at(5)->furniPrice.emplace_back(800);
	furni.at(5)->furniPrice.emplace_back(2000);

	//tv
	furni.at(6)->furniPrice.emplace_back(200);
	furni.at(6)->furniPrice.emplace_back(800);
	furni.at(6)->furniPrice.emplace_back(1500);
	furni.at(6)->furniPrice.emplace_back(3000);
	furni.at(6)->furniPrice.emplace_back(8000);

	//책상
	furni.at(7)->furniPrice.emplace_back(5);
	furni.at(7)->furniPrice.emplace_back(300);
	furni.at(7)->furniPrice.emplace_back(800);
	furni.at(7)->furniPrice.emplace_back(4000);

	//pc
	furni.at(8)->furniPrice.emplace_back(30);
	furni.at(8)->furniPrice.emplace_back(300);
	furni.at(8)->furniPrice.emplace_back(700);
	furni.at(8)->furniPrice.emplace_back(1500);
	furni.at(8)->furniPrice.emplace_back(9000);
}

void RoomInfo::MonthGoldPM()
{
	GoldPM->SetImgName("minus");
	GoldPM->SetPosition(D3DXVECTOR2(WINSIZEX / 10.0f * 2.1f, WINSIZEY / 10.0f * 6.0f));
	GoldPM->Update();
	GoldPM->Render();

	if (monthGold > 0)
	{
		if (GoldPM->IsKeyDown(VK_LBUTTON))
		{
			monthGold -= 1;
			time = -0.5f;
		}

		if (GoldPM->MouseInColBox())
		{
			if (KEYMANAGER->StayKeyDown(VK_LBUTTON))
			{
				if (time >= 0.1f)
				{
					monthGold -= 1;
					time = 0.0f;
				}

				time += g_ETime;
			}
		}

		if (KEYMANAGER->MouseWheel() < 0)
		{
			monthGold -= 1;
		}
	}
	GoldPM->SetImgName("plus");
	GoldPM->SetPosition(D3DXVECTOR2(WINSIZEX / 10.0f * 3.5f, WINSIZEY / 10.0f * 6.0f));
	GoldPM->Update();
	GoldPM->Render();

	if (monthGold < 47)
	{
		if (GoldPM->IsKeyDown(VK_LBUTTON))
		{
			monthGold += 1;
			time = -0.5f;
		}

		if (GoldPM->MouseInColBox())
		{
			if (KEYMANAGER->StayKeyDown(VK_LBUTTON))
			{

				if (time >= 0.1f)
				{
					monthGold += 1;
					time = 0.0f;
				}

				time += g_ETime;
			}
		}

		if (KEYMANAGER->MouseWheel() > 0)
		{
			monthGold += 1;
		}
	}
	
}

void RoomInfo::KickLiveMon()
{
	if (PtInRect(liveMonster->GetRect(), g_ptMouse))
	{
		img = IMGMANAGER->GetImage("button_driveout");
		img->Scale(26.0f, 25.0f);
		img->Position(liveMonster->GetPosition().x + liveMonster->GetScale().x / 2 - 13.0f ,
					  liveMonster->GetPosition().y + liveMonster->GetScale().y / 2 - 12.5f);
		img->Render();

		if (KEYMANAGER->isKeyDown(VK_LBUTTON))
		{
			CHARACTERMANAGER->GetMonster(name)->SetState(STATE_OUT_HOME);

			liveMonster = nullptr;
			name = "";
		}
	}
}
