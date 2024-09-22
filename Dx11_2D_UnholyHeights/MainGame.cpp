#include "stdafx.h"
#include "MainGame.h"
#include "MiniGame/Loading.h"
#include "Mension/GameScene.h"
#include "Mension/MainScene.h"

MainGame::MainGame()
{
}


MainGame::~MainGame()
{
	//싱글톤 해제

	SCENEMANAGER->ReleaseAll();
	SCENEMANAGER->ReleaseSingleton();
	KEYMANAGER->ReleaseSingleton();
	TIMEMANAGER->ReleaseSingleton();
	SOUNDMANAGER->ReleaseSingleton();
	CAMMANAGER->ReleaseSingleton();
	IMGMANAGER->DeleteAll();
	IMGMANAGER->ReleaseSingleton();
	WRITEMANAGER->ReleaseSingleton();
	CHARACTERMANAGER->DeleteAll();
	CHARACTERMANAGER->ReleaseSingleton();
	QUESTMANAGER->DeleteAll();
	QUESTMANAGER->ReleaseSingleton();
}

HRESULT MainGame::init()
{
	//윈도우가 시작되서 지금까지 흐른 시간을 1/1000초 단위로
	//DWORD형을 리턴하는 함수다
	srand(GetTickCount());
	////////////////////////////////////////////////////

	AddResource();
	AddQuest();
	AddSound();

	SCENEMANAGER->AddScene("Main", new MainScene);
	SCENEMANAGER->AddScene("Loading", new Loading);
	SCENEMANAGER->AddScene("Game", new GameScene);
	
	SCENEMANAGER->ChangeScene("Main");
	////////////////////////////////////////////////////
	ImGui::Create(g_hwnd, Device, DeviceContext);
	ImGui::StyleColorsDark();
	return S_OK;
}

void MainGame::release()
{
	////////////////////////////////////////////////////
	




	////////////////////////////////////////////////////
	ImGui::Delete();
}

void MainGame::update()
{
	////////////////////////////////////////////////////
	
	
	if (KEYMANAGER->isKeyDown(VK_F1))
	{
		/*if (onOffIM)
			onOffIM = false;
		else
			onOffIM = true;*/

		playerGold += 10000;
	}

	SCENEMANAGER->CurrentSceneUpdate();

	////////////////////////////////////////////////////
	SOUNDMANAGER->Update();
	ImGui::Update();
}

void MainGame::render()
{
	D3DXCOLOR background = D3DXCOLOR(1, 1, 1, 1);
	DeviceContext->ClearRenderTargetView(RTV, (float *)background);
	CAMMANAGER->SetBuffers();
	WRITEMANAGER->GetDc()->BeginDraw();
	//TIMEMANAGER->render();
	////////////////////////////////////////////////////






	SCENEMANAGER->CurrentSceneRender();
	wheel = 0;
	////////////////////////////////////////////////////

#ifdef _DEBUG
	ImGui::Render();
#endif
	
	WRITEMANAGER->GetDc()->EndDraw();
	SwapChain->Present(0, 0);

}

void MainGame::AddResource()
{
	//게임 내 UI 인터페이스
	IMGMANAGER->AddImage("sel_des", "window/sel_des.png");
	IMGMANAGER->AddImage("window_monster", "window/window_monster0.png", D3DXVECTOR2(-0.5f, -1.0f), D3DXVECTOR2(0.5f, 0.0f));
	IMGMANAGER->AddImage("window_room", "window/window_room.png", D3DXVECTOR2(-0.5f, -1.0f), D3DXVECTOR2(0.5f, 0.0f));
	IMGMANAGER->AddImage("window_maou", "window/window_maou0.png", D3DXVECTOR2(-0.5f, -1.0f), D3DXVECTOR2(0.5f, 0.0f));
	IMGMANAGER->AddImage("window_kanban", "window/window_kanban0.png", D3DXVECTOR2(-0.5f, -1.0f), D3DXVECTOR2(0.5f, 0.0f));
	IMGMANAGER->AddImage("female", "window/female.png");
	IMGMANAGER->AddImage("male", "window/male.png");
	IMGMANAGER->AddImage("star", "window/star.png", 1, 8);
	IMGMANAGER->AddImage("kanban_header", "title/kanban_header0.png", 1, 3);
	IMGMANAGER->AddImage("kanban_paper", "title/kanban_paper0.png");
	IMGMANAGER->AddImage("kanban_window", "title/kanban_window.png");
	IMGMANAGER->AddImage("warning", "warning.png");
	IMGMANAGER->AddImage("quest_win", "quest_win0.png");
	IMGMANAGER->AddImage("quest_lose", "quest_lose0.png");
	IMGMANAGER->AddImage("bullet", "effect.png", 6, 4);
	IMGMANAGER->AddImage("cursor_green", "winmenu/cursor_furniture_green.png");
	IMGMANAGER->AddImage("cursor_orange", "winmenu/cursor_furniture_orange.png");
	IMGMANAGER->AddImage("money_window", "window/money_window.png");
	IMGMANAGER->AddImage("button_driveout", "winmenu/button_driveout.png");
	IMGMANAGER->AddImage("gamespeed_01_on", "winmenu/menu_bar_speed1.png");
	IMGMANAGER->AddImage("gamespeed_01_off", "winmenu/menu_bar_speed1_off.png");
	IMGMANAGER->AddImage("gamespeed_02_on", "winmenu/menu_bar_speed2.png");
	IMGMANAGER->AddImage("gamespeed_02_off", "winmenu/menu_bar_speed2_off.png");
	IMGMANAGER->AddImage("gamespeed_03_on", "winmenu/menu_bar_speed3.png");
	IMGMANAGER->AddImage("gamespeed_03_off", "winmenu/menu_bar_speed3_off.png");
	IMGMANAGER->AddImage("menu_bar", "winmenu/menu_bar0.png");

	//메인배경
	IMGMANAGER->AddImage("title", "title/chara0.png");
	IMGMANAGER->AddImage("title_back", "title/title_back0.png");
	IMGMANAGER->AddImage("logo", "title/logo0.png");
	IMGMANAGER->AddImage("start", "title/start0.png");

	//배경
	IMGMANAGER->AddImage("sun", "mansion/sun.png", 1, 3);
	IMGMANAGER->AddImage("moon", "mansion/moon.png", 1, 3);
	IMGMANAGER->AddImage("kanban", "mansion/kanban.png");
	IMGMANAGER->AddImage("field", "mansion/field.png");
	IMGMANAGER->AddImage("sky", "mansion/sky.png");

	//아파트(맨션)1단계
	IMGMANAGER->AddImage("apart1", "mansion/apart1.png", D3DXVECTOR2(-0.5f, 0.0f), D3DXVECTOR2(0.5f, 1.0f));
	IMGMANAGER->AddImage("apart1_doorClose", "mansion/apart1_doorClose.png");
	IMGMANAGER->AddImage("apart1_doorOpen", "mansion/apart1_doorOpen.png");
	IMGMANAGER->AddImage("apart1_rouka", "mansion/apart1_rouka.png");

	//아파트(맨션)2단계
	IMGMANAGER->AddImage("apart2", "mansion/apart2.png", D3DXVECTOR2(-0.5f, 0.0f), D3DXVECTOR2(0.5f, 1.0f));
	IMGMANAGER->AddImage("apart2_doorClose", "mansion/apart2_doorClose.png");
	IMGMANAGER->AddImage("apart2_doorOpen", "mansion/apart2_doorOpen.png");
	IMGMANAGER->AddImage("apart2_rouka", "mansion/apart2_rouka.png");
	IMGMANAGER->AddImage("apart2_doorCloseM", "mansion/apart2_doorCloseM.png");
	IMGMANAGER->AddImage("apart2_doorOpenM", "mansion/apart2_doorOpenM.png");

	//아파트(맨션)3단계
	IMGMANAGER->AddImage("apart3", "mansion/apart3.png", D3DXVECTOR2(-0.5f, 0.0f), D3DXVECTOR2(0.5f, 1.0f));
	IMGMANAGER->AddImage("apart3_doorClose", "mansion/apart3_doorClose.png");
	IMGMANAGER->AddImage("apart3_doorOpen", "mansion/apart3_doorOpen.png");
	IMGMANAGER->AddImage("apart3_rouka", "mansion/apart3_rouka.png");
	IMGMANAGER->AddImage("apart3_doorCloseM", "mansion/apart3_doorCloseM.png");
	IMGMANAGER->AddImage("apart3_doorOpenM", "mansion/apart3_doorOpenM.png");

	//아파트(맨션)4단계
	IMGMANAGER->AddImage("apart4", "mansion/apart4.png", D3DXVECTOR2(-0.5f, 0.0f), D3DXVECTOR2(0.5f, 1.0f));
	IMGMANAGER->AddImage("apart4_doorClose", "mansion/apart4_doorClose.png");
	IMGMANAGER->AddImage("apart4_doorOpen", "mansion/apart4_doorOpen.png");
	IMGMANAGER->AddImage("apart4_rouka", "mansion/apart4_rouka.png");
	IMGMANAGER->AddImage("apart4_doorCloseM", "mansion/apart4_doorCloseM.png");
	IMGMANAGER->AddImage("apart4_doorOpenM", "mansion/apart4_doorOpenM.png");

	//방 인터페이스
	IMGMANAGER->AddImage("room_window", "inroom/room_window.png");
	IMGMANAGER->AddImage("room_contents", "inroom/room_contents0.png");
	IMGMANAGER->AddImage("furniture", "inroom/furniture_ex0.png");
	IMGMANAGER->AddImage("furni_icon", "furniture/furni_icon.png", 4, 5);
	IMGMANAGER->AddImage("air", "furniture/air.png", 5, 2);
	IMGMANAGER->AddImage("audio", "furniture/audio.png", 5, 2);
	IMGMANAGER->AddImage("bath", "furniture/bath.png", 5, 2);
	IMGMANAGER->AddImage("bed", "furniture/bed.png", 5, 2);
	IMGMANAGER->AddImage("computer", "furniture/computer.png", 5, 2);
	IMGMANAGER->AddImage("desk", "furniture/desk.png", 5, 2);
	IMGMANAGER->AddImage("kitchen", "furniture/kitchen.png", 5, 2);
	IMGMANAGER->AddImage("tv", "furniture/tv.png", 5, 2);
	IMGMANAGER->AddImage("wallpaper", "furniture/wallpaper.png", 5, 2);
	IMGMANAGER->AddImage("furniture_title", "inroom/furniture_title0.png");
	IMGMANAGER->AddImage("plus", "winmenu/plus.png");
	IMGMANAGER->AddImage("minus", "winmenu/minus.png");
	IMGMANAGER->AddImage("piyomaster", "const/const_piyomaster.png"); 
	IMGMANAGER->AddImage("room_fukidashi", "inroom/room_fukidashi.png");

	//피요족
	//남
	IMGMANAGER->AddImage("piyo_piyo", "monster/piyo_piyo.png", 2, 4);
	IMGMANAGER->AddImage("piyo_gaku", "monster/piyo_gaku.png", 2, 4);
	IMGMANAGER->AddImage("piyo_kata", "monster/piyo_kata.png", 2, 4);
	IMGMANAGER->AddImage("piyo_kira", "monster/piyo_kira.png", 2, 4);
	//여
	IMGMANAGER->AddImage("piyo_piyo_f", "monster/piyo_piyo_f.png", 2, 4);
	IMGMANAGER->AddImage("piyo_gaku_f", "monster/piyo_gaku_f.png", 2, 4);
	IMGMANAGER->AddImage("piyo_kata_f", "monster/piyo_kata_f.png", 2, 4);
	IMGMANAGER->AddImage("piyo_kira_f", "monster/piyo_kira_f.png", 2, 4);

	//언데드 족
	//남
	IMGMANAGER->AddImage("un_hone", "monster/un_hone.png", 2, 4);
	IMGMANAGER->AddImage("un_zonbi", "monster/un_zonbi.png", 2, 4);
	IMGMANAGER->AddImage("un_musya", "monster/un_musya.png", 2, 4);
	IMGMANAGER->AddImage("un_oba", "monster/un_oba.png", 2, 4);
	//여
	IMGMANAGER->AddImage("un_hone_f", "monster/un_hone_f.png", 2, 4);
	IMGMANAGER->AddImage("un_zonbi_f", "monster/un_zonbi_f.png", 2, 4);
	IMGMANAGER->AddImage("un_musya_f", "monster/un_musya_f.png", 2, 4);
	IMGMANAGER->AddImage("un_oba_f", "monster/un_oba_f.png", 2, 4);

	//엘레멘탈 족
	//남
	IMGMANAGER->AddImage("ele_miz", "monster/ele_miz.png", 2, 4);
	IMGMANAGER->AddImage("ele_moku", "monster/ele_moku.png", 2, 4);
	IMGMANAGER->AddImage("ele_mera", "monster/ele_mera.png", 2, 4);
	IMGMANAGER->AddImage("ele_seki", "monster/ele_seki.png", 2, 4);
	//여
	IMGMANAGER->AddImage("ele_miz_f", "monster/ele_miz_f.png", 2, 4);
	IMGMANAGER->AddImage("ele_moku_f", "monster/ele_moku_f.png", 2, 4);
	IMGMANAGER->AddImage("ele_mera_f", "monster/ele_mera_f.png", 2, 4);
	IMGMANAGER->AddImage("ele_seki_f", "monster/ele_seki_f.png", 2, 4);

	//심연 족
	//남
	IMGMANAGER->AddImage("deep_sira", "monster/deep_sira.png", 2, 4);
	IMGMANAGER->AddImage("deep_gucha", "monster/deep_gucha.png", 2, 4);
	IMGMANAGER->AddImage("deep_dagyo", "monster/deep_dagyo.png", 2, 4);
	IMGMANAGER->AddImage("deep_master", "monster/deep_master.png", 2, 4);
	//여
	IMGMANAGER->AddImage("deep_sira_f", "monster/deep_sira_f.png", 2, 4);
	IMGMANAGER->AddImage("deep_gucha_f", "monster/deep_gucha_f.png", 2, 4);
	IMGMANAGER->AddImage("deep_dagyo_f", "monster/deep_dagyo_f.png", 2, 4);
	IMGMANAGER->AddImage("deep_master_f", "monster/deep_master_f.png", 2, 4);

	//아인 족
	//남
	IMGMANAGER->AddImage("demi_kenta", "monster/demi_kenta.png", 2, 4);
	IMGMANAGER->AddImage("demi_sumo", "monster/demi_sumo.png", 2, 4);
	IMGMANAGER->AddImage("demi_yuki", "monster/demi_yuki.png", 2, 4);
	IMGMANAGER->AddImage("demi_qpd", "monster/demi_qpd.png", 2, 4);
	//여
	IMGMANAGER->AddImage("demi_kenta_f", "monster/demi_kenta_f.png", 2, 4);
	IMGMANAGER->AddImage("demi_sumo_f", "monster/demi_sumo_f.png", 2, 4);
	IMGMANAGER->AddImage("demi_yuki_f", "monster/demi_yuki_f.png", 2, 4);
	IMGMANAGER->AddImage("demi_qpd_F", "monster/demi_qpd_f.png", 2, 4);

	//마족
	//남
	IMGMANAGER->AddImage("demo_maga", "monster/demo_maga.png", 2, 4);
	IMGMANAGER->AddImage("demo_sachu", "monster/demo_sachu.png", 2, 4);
	IMGMANAGER->AddImage("demo_maton", "monster/demo_maton.png", 2, 4);
	IMGMANAGER->AddImage("demo_dra", "monster/demo_dra.png", 2, 4);
	//여
	IMGMANAGER->AddImage("demo_maga_f", "monster/demo_maga_f.png", 2, 4);
	IMGMANAGER->AddImage("demo_sachu_f", "monster/demo_sachu_f.png", 2, 4);
	IMGMANAGER->AddImage("demo_maton_f", "monster/demo_maton_f.png", 2, 4);
	IMGMANAGER->AddImage("demo_dra_f", "monster/demo_dra_f.png", 2, 4);

	//애니멀 족
	//남
	IMGMANAGER->AddImage("ani_wolf", "monster/ani_wolf.png", 2, 4);
	IMGMANAGER->AddImage("ani_mai", "monster/ani_mai.png", 2, 4);
	IMGMANAGER->AddImage("ani_nya", "monster/ani_nya.png", 2, 4);
	IMGMANAGER->AddImage("ani_bear", "monster/ani_bear.png", 2, 4);
	//여
	IMGMANAGER->AddImage("ani_wolf_f", "monster/ani_wolf_f.png", 2, 4);
	IMGMANAGER->AddImage("ani_mai_f", "monster/ani_mai_f.png", 2, 4);
	IMGMANAGER->AddImage("ani_nya_f", "monster/ani_nya_f.png", 2, 4);
	IMGMANAGER->AddImage("ani_bear_f", "monster/ani_bear_f.png", 2, 4);

	//용사
	IMGMANAGER->AddImage("brave_village", "hero/brave_village.png", 2, 4);
	IMGMANAGER->AddImage("brave_bad", "hero/brave_bad.png", 2, 4);
	IMGMANAGER->AddImage("brave_wiz_red", "hero/brave_wiz_red.png", 2, 4);
	IMGMANAGER->AddImage("brave_wiz_blue", "hero/brave_wiz_blue.png", 2, 4);
	IMGMANAGER->AddImage("brave_wiz_black", "hero/brave_wiz_black.png", 2, 4);
	IMGMANAGER->AddImage("brave_thief", "hero/brave_thief.png", 2, 4);
	IMGMANAGER->AddImage("brave_trade", "hero/brave_trade.png", 2, 4);
	IMGMANAGER->AddImage("brave_badmaster", "hero/brave_badmaster.png", 2, 4);
	IMGMANAGER->AddImage("brave_sword", "hero/brave_sword.png", 2, 4);
	IMGMANAGER->AddImage("brave_rune", "hero/brave_rune.png", 2, 4);
	IMGMANAGER->AddImage("brave_princess", "hero/brave_princess.png", 2, 4);
	IMGMANAGER->AddImage("brave_archer", "hero/brave_archer.png", 2, 4);
	IMGMANAGER->AddImage("brave_bomgole", "hero/brave_bomgole.png", 2, 4);
	IMGMANAGER->AddImage("brave_brave", "hero/brave_brave.png", 2, 4);
	IMGMANAGER->AddImage("brave_bravemaster", "hero/brave_bravemaster.png", 2, 4);
	IMGMANAGER->AddImage("brave_cannon", "hero/brave_cannon.png", 2, 4);
	IMGMANAGER->AddImage("brave_doctor", "hero/brave_doctor.png", 2, 4);
	IMGMANAGER->AddImage("brave_kinghorse", "hero/brave_kinghorse.png", 2, 4);
	IMGMANAGER->AddImage("brave_magicrobo", "hero/brave_magicrobo.png", 2, 4);
	IMGMANAGER->AddImage("brave_monk", "hero/brave_monk.png", 2, 4);
	IMGMANAGER->AddImage("brave_ninja", "hero/brave_ninja.png", 2, 4);
	IMGMANAGER->AddImage("brave_pirate", "hero/brave_pirate.png", 2, 4);

	//마왕
	IMGMANAGER->AddImage("ma_ou", "monster/ma_ou.png", 2, 4);

}

//시작 시 퀘스트 추가
void MainGame::AddQuest()
{
	wstring info;
	Quest* q;
	
	info = L"마을 폐허에 몬스터가\n숨어있다고 한다.\n그 소문을 들은 마을 사람\n들이 폐허로간다";
	q = QUESTMANAGER->AddQuest(2, 0, L"마을의 몬스터", info, "1마을의 몬스터", 250);
	q->SetWaveBrave(1, TRIBE_BRAVE_VILLAGE);
	q->SetWaveBrave(1, TRIBE_BRAVE_VILLAGE);
	q->SetWaveBrave(2, TRIBE_BRAVE_VILLAGE);

	info = L"(마왕이 퍼트린)보물의 소문\n을 들은 변변찮은 모험자들이\n아파트를 향해서 오고 있다!";
	q = QUESTMANAGER->AddQuest(2, 1, L"보물이 있습니다", info, "2보물이 있습니다", 400);
	q->SetWaveBrave(1, TRIBE_BRAVE_BAD);
	q->SetWaveBrave(2, TRIBE_BRAVE_BAD);
	q->SetWaveBrave(2, TRIBE_BRAVE_WIZ_BLUE);

	info = L"건물 전체를 마법 실험으로\n해방합니다! 라는\n전단지를 본 마법사들이 마법\n연습을 하러 온다";
	q = QUESTMANAGER->AddQuest(2, 1, L"마법 실험소", info, "3마법 실험소", 500);
	q->SetWaveBrave(1, TRIBE_BRAVE_WIZ_BLUE);
	q->SetWaveBrave(1, TRIBE_BRAVE_WIZ_BLUE);
	q->SetWaveBrave(2, TRIBE_BRAVE_WIZ_BLUE);
	q->SetWaveBrave(2, TRIBE_BRAVE_WIZ_BLUE);
	q->SetWaveBrave(2, TRIBE_BRAVE_WIZ_RED);

	info = L"세계를 구할 용사가 될거야!\n봉고레는 그렇게 선언하고,\n가장 먼저 마을 외곽의\n몬스터를 퇴치하기로 했다!";
	q = QUESTMANAGER->AddQuest(1, 1, L"내 이름은 봉고레", info, "4내 이름은 봉고레", 200);
	q->SetWaveBrave(1, TRIBE_BRAVE_BOMGOLE);

	info = L"마을의 마물을 내몰기 위한\n보디가드로써, 도적들이\n고용되었다!\n그들을 처치하면 많은\n보상을 얻을 수 있다!";
	q = QUESTMANAGER->AddQuest(3, 2, L"새로운 보디가드", info, "5새로운 보디가드", 1200);
	q->SetWaveBrave(1, TRIBE_BRAVE_BAD);
	q->SetWaveBrave(1, TRIBE_BRAVE_BAD);
	q->SetWaveBrave(2, TRIBE_BRAVE_BAD);
	q->SetWaveBrave(2, TRIBE_BRAVE_THIEF);
	q->SetWaveBrave(3, TRIBE_BRAVE_BADMASTER);
}

void MainGame::AddSound()
{
	//bgm
	SOUNDMANAGER->addSound("main_sound", "sound/carnival_love.mp3", true);
	SOUNDMANAGER->addSound("game_sound01", "sound/every_day_killing.mp3", false);
	SOUNDMANAGER->addSound("game_sound02", "sound/morning_for_a_deadmand.mp3", false);
	SOUNDMANAGER->addSound("game_sound03", "sound/sky_hike.mp3", false);
	SOUNDMANAGER->addSound("game_sound04", "sound/sunny_side_up.mp3", false);
	SOUNDMANAGER->addSound("battle_sound", "sound/three_little_witches.mp3", true);
	SOUNDMANAGER->addSound("warning_sound", "sound/Warning_sound.mp3", true);

	//효과음
	SOUNDMANAGER->addSound("door_open_sound", "sound/문_닫기.mp3", false);
	SOUNDMANAGER->addSound("door_close_sound", "sound/문_열기.mp3", false);
}
