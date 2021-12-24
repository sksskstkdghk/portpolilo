#include "stdafx.h"
#include "Character.h"


Character::Character(string name, int gender, int tribe)
{
	currentFloor = 0;
	this->name = name;
	this->gender = gender;
	this->tribe = tribe;

	pos = D3DXVECTOR2(-40.0f, 170.0f);
	scale = D3DXVECTOR2(80.0f, 80.0f);
	color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	home = new House();

	hp = new HpInfo();
	MonsterTribeInfo(tribe);

	currentFrameX = 0;
	currentFrameY = 0;
	gold = 0;

	time = 0.0f;
	workTime = 0.0f;

	aniSpeed = 0;
}

Character::~Character()
{
	SAFE_DELETE(colBox);
	SAFE_DELETE(demageBox);
	SAFE_DELETE(home);
	SAFE_DELETE(bullet);
	SAFE_DELETE(hp);
}

void Character::Update()
{
	if (home == nullptr)
		state = STATE_KICK;

	if(CHARACTERMANAGER->GetAllBrave().size() <= 0)
		pantsRun = false;

	if (img->GetPosition().x > -40.0f && hp->currentHP > 0)
	{
		if (CHARACTERMANAGER->GetAllBrave().size() != 0 && color.a >= 1.0f &&
			state != (int)STATE_BRAVE)
		{
			for (int i = 0; i < CHARACTERMANAGER->GetAllBrave().size(); i++)
			{
				if (CHARACTERMANAGER->GetAllBrave().at(i)->GetPos().x > -40.0f &&
					state != (int)STATE_IN_HOME)
				{
					state = (int)STATE_BATTLE;
					break;
				}
				else
				{
					if(state != (int)STATE_IN_HOME)
					state = (int)STATE_GOTO_HOME;
				}
			}
		}
	}

	if (hp->currentHP > 0)
	{
		Animation();

		if (state != (int)STATE_BRAVE)
			AI();

		if (state == (int)STATE_BRAVE)
		{
			if (img->GetPosition().x < -40.0f && time > 0.0f)
				time -= g_ETime;
			else
				BraveAI();
		}
	}
	else
		state = (int)STATE_DEAD;

	//cout << currentFloor << endl;
}

void Character::Render()
{
	if (hp->currentHP > 0)
	{
		img->SetCurrentFrameX(currentFrameX);
		img->SetCurrentFrameY(currentFrameY);
		img->Scale(scale);
		img->Color(color);
		img->Position(pos);
		img->Rotate(rotate);
		img->Update();

		if (state == (int)STATE_BATTLE || state == (int)STATE_BRAVE)
			hp->Render();

		if (img->GetPosition().x > -40.0f && color.a > 0.0f)
			img->Render();

		colBox->SetParent(img->GetRT());
		colBox->SetisFill(false);
		colBox->Update();
	}

	if(bullet != nullptr)
		bullet->Render();

	//colBox->Render();

	//ImGui::InputInt("goal_floor", &state);
}

void Character::Animation()
{
	aniSpeed += g_ETime;

	if (aniSpeed >= moveSpeed && currentFrameY != 3)
	{
		aniSpeed = 0.0f;

		if (currentFrameX < 1)
		{
			currentFrameX++;
		}
		else
		{
			currentFrameX = 0;;
		}

	}
	else if (aniSpeed >= attackSpeed && currentFrameY == 3)
	{
		aniSpeed = 0.0f;

		if (currentFrameX < 1)
			currentFrameX++;
		else
		{
			currentFrameX = 0;

			if(bullet != nullptr)
				bullet->Fire();
		}
	}
	
}

//각 종족별 객체값
void Character::MonsterTribeInfo(int tribe)
{
	colBox = new Rect();

	//피요리코족
	if (tribe / 100 == 0)
	{
		//체력E, 이동속도C, 공격속도S, 공격력E, 물방E, 마방E, 근접물리
		if (tribe % 100 == 0)	//피요리코
		{
			//GetFromIntTo(min, max) 랜덤 인트값
			hp->maxHP = GetFromIntTo(80, 100);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(6, 8);
			normalDEF = GetFromIntTo(0, 3);	//물리 방어력
			magicDEF = GetFromIntTo(0, 3);	//마법 방어력
			range = 0;						//사정거리(0이면 근접공격)
			bullet = nullptr;				//총알
			type = DEMAGE_NORMAL;			//공격 타입(물리공격 또는 마법공격)

			moveSpeed = MOVESPEED_C;
			attackSpeed = ATTACKSPEED_S;

			if (gender == (int)GENDER_MALE)	//성별
				tribeName = "piyo_piyo";
			else if (gender == (int)GENDER_FEMALE)
				tribeName = "piyo_piyo_f";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));	//충돌박스 크기
			colBox->Position(-2.0f, -12.0f);			//충돌박스 위치
		}
		//체력D, 이동속도C, 공격속도C, 공격력B, 물방E, 마방C, 원거리마법
		else if (tribe % 100 == 1)	//가쿠피요
		{
			hp->maxHP = GetFromIntTo(130, 150);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(15, 20);
			normalDEF = GetFromIntTo(0, 3);
			magicDEF = GetFromIntTo(4, 7);
			range = 300.0f;
			bullet = new Bullet(7, demage, D3DXVECTOR2(BULLET_X * 3.0f, 0.0f), D3DXVECTOR2(BULLET_X * 4.0f, BULLET_Y));
			type = DEMAGE_MAGIC;

			moveSpeed = MOVESPEED_C;
			attackSpeed = ATTACKSPEED_C;

			if (gender == (int)GENDER_MALE)
				tribeName = "piyo_gaku";
			else if (gender == (int)GENDER_FEMALE)
				tribeName = "piyo_gaku_f";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
		//체력D, 이동속도D, 공격속도B, 공격력E, 물방A, 마방C, 중거리물리
		else if (tribe % 100 == 2)	//카타피요
		{
			hp->maxHP = GetFromIntTo(130, 150);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(6, 8);
			normalDEF = GetFromIntTo(8, 11);
			magicDEF = GetFromIntTo(4, 7);
			range = 250.0f;
			bullet = new Bullet(7, demage, D3DXVECTOR2(BULLET_X * 3.0f, 0.0f), D3DXVECTOR2(BULLET_X * 4.0f, BULLET_Y));
			type = DEMAGE_NORMAL;

			moveSpeed = MOVESPEED_D;
			attackSpeed = ATTACKSPEED_B;

			if (gender == (int)GENDER_MALE)
				tribeName = "piyo_kata";
			else if (gender == (int)GENDER_FEMALE)
				tribeName = "piyo_kata_f";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
		//체력D, 이동속도C, 공격속도B, 공격력E, 물방E, 마방A, 중거리물리
		else if (tribe % 100 == 3)	//키라피요
		{
			hp->maxHP = GetFromIntTo(130, 150);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(6, 8);
			normalDEF = GetFromIntTo(0, 3);
			magicDEF = GetFromIntTo(8, 11);
			range = 250.0f;
			bullet = new Bullet(7, demage, D3DXVECTOR2(BULLET_X * 3.0f, 0.0f), D3DXVECTOR2(BULLET_X * 4.0f, BULLET_Y));
			type = DEMAGE_NORMAL;

			moveSpeed = MOVESPEED_C;
			attackSpeed = ATTACKSPEED_B;

			if (gender == (int)GENDER_MALE)
				tribeName = "piyo_kira";
			else if (gender == (int)GENDER_FEMALE)
				tribeName = "piyo_kira_f";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
	}
	//언데드 족
	else if(tribe / 100 == 1)
	{
		//체력D, 이동속도C, 공격속도B, 공격력E, 물방D, 마방E, 근접물리
		if (tribe % 100 == 0)	//호넬튼
		{
			hp->maxHP = GetFromIntTo(130, 150);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(6, 8);
			normalDEF = GetFromIntTo(2, 5);
			magicDEF = GetFromIntTo(0, 3);
			range = 0;
			bullet = nullptr;
			type = DEMAGE_NORMAL;

			moveSpeed = MOVESPEED_B;
			attackSpeed = ATTACKSPEED_C;

			if (gender == (int)GENDER_MALE)
				tribeName = "un_hone";
			else if (gender == (int)GENDER_FEMALE)
				tribeName = "un_hone_f";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
		//체력C, 이동속도E, 공격속도C, 공격력C, 물방D, 마방E, 근접물리
		else if (tribe % 100 == 1)	//좀비
		{
			hp->maxHP = GetFromIntTo(180, 200);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(12, 16);
			normalDEF = GetFromIntTo(2, 5);
			magicDEF = GetFromIntTo(0, 3);
			range = 0;
			bullet = nullptr;
			type = DEMAGE_NORMAL;

			moveSpeed = MOVESPEED_C;
			attackSpeed = ATTACKSPEED_B;

			if (gender == (int)GENDER_MALE)
				tribeName = "un_zonbi";
			else if (gender == (int)GENDER_FEMALE)
				tribeName = "un_zonbi_f";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
		//체력D, 이동속도C, 공격속도B, 공격력A, 물방B, 마방B, 근접물리
		else if (tribe % 100 == 2)	//호네무샤
		{
			hp->maxHP = GetFromIntTo(130, 150);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(18, 24);
			normalDEF = GetFromIntTo(6, 9);
			magicDEF = GetFromIntTo(6, 9);
			range = 0;
			bullet = nullptr;
			type = DEMAGE_NORMAL;

			moveSpeed = MOVESPEED_C;
			attackSpeed = ATTACKSPEED_B;

			if (gender == (int)GENDER_MALE)
				tribeName = "un_musya";
			else if (gender == (int)GENDER_FEMALE)
				tribeName = "un_musya_f";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
		//체력C, 이동속도B, 공격속도C, 공격력B, 물방S, 마방E, 초원거리마법
		else if (tribe % 100 == 3)	//오바키오
		{
			hp->maxHP = GetFromIntTo(180, 200);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(15, 20);
			normalDEF = GetFromIntTo(10, 13);
			magicDEF = GetFromIntTo(0, 3);
			range = 350.0f;
			bullet = new Bullet(7, demage, D3DXVECTOR2(BULLET_X * 3.0f, 0.0f), D3DXVECTOR2(BULLET_X * 4.0f, BULLET_Y));
			type = DEMAGE_MAGIC;

			moveSpeed = MOVESPEED_B;
			attackSpeed = ATTACKSPEED_C;

			if (gender == (int)GENDER_MALE)
				tribeName = "un_oba";
			else if (gender == (int)GENDER_FEMALE)
				tribeName = "un_oba_f";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
	}
	//엘리먼트 족
	else if (tribe / 100 == 2)
	{
		//체력E, 이동속도D, 공격속도D, 공격력D, 물방E, 마방E, 원거리마법
		if (tribe % 100 == 0)	//미즈닌
		{
			hp->maxHP = GetFromIntTo(80, 100);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(9, 12);
			normalDEF = GetFromIntTo(0, 3);
			magicDEF = GetFromIntTo(0, 3);
			range = 300.0f;
			bullet = new Bullet(7, demage, D3DXVECTOR2(BULLET_X * 3.0f, 0.0f), D3DXVECTOR2(BULLET_X * 4.0f, BULLET_Y));
			type = DEMAGE_MAGIC;

			moveSpeed = MOVESPEED_D;
			attackSpeed = ATTACKSPEED_D;

			if (gender == (int)GENDER_MALE)
				tribeName = "ele_miz";
			else if (gender == (int)GENDER_FEMALE)
				tribeName = "ele_miz_f";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
		//체력D, 이동속도D, 공격속도C, 공격력D, 물방E, 마방E, 초원거리물리
		else if (tribe % 100 == 1)	//모쿠하난
		{
			hp->maxHP = GetFromIntTo(130, 150);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(9, 12);
			normalDEF = GetFromIntTo(0, 3);
			magicDEF = GetFromIntTo(0, 3);
			range = 350.0f;
			bullet = new Bullet(7, demage, D3DXVECTOR2(BULLET_X * 3.0f, 0.0f), D3DXVECTOR2(BULLET_X * 4.0f, BULLET_Y));
			type = DEMAGE_NORMAL;

			moveSpeed = MOVESPEED_D;
			attackSpeed = ATTACKSPEED_C;

			if (gender == (int)GENDER_MALE)
				tribeName = "ele_moku";
			else if (gender == (int)GENDER_FEMALE)
				tribeName = "ele_moku_f";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
		//체력D, 이동속도C, 공격속도B, 공격력B, 물방C, 마방D, 중거리마법
		else if (tribe % 100 == 2)	//멜라민
		{
			hp->maxHP = GetFromIntTo(130, 150);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(15, 20);
			normalDEF = GetFromIntTo(4, 7);
			magicDEF = GetFromIntTo(2, 5);
			range = 250.0f;
			bullet = new Bullet(7, demage, D3DXVECTOR2(BULLET_X * 3.0f, 0.0f), D3DXVECTOR2(BULLET_X * 4.0f, BULLET_Y));
			type = DEMAGE_MAGIC;

			moveSpeed = MOVESPEED_C;
			attackSpeed = ATTACKSPEED_B;

			if (gender == (int)GENDER_MALE)
				tribeName = "ele_mera";
			else if (gender == (int)GENDER_FEMALE)
				tribeName = "ele_mera_f";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
		//체력C, 이동속도D, 공격속도C, 공격력E, 물방S, 마방S, 근접물리
		else if (tribe % 100 == 3)	//세키가렌
		{
			hp->maxHP = GetFromIntTo(180, 200);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(6, 8);
			normalDEF = GetFromIntTo(10, 13);
			magicDEF = GetFromIntTo(10, 13);
			range = 0;
			bullet = nullptr;
			type = DEMAGE_NORMAL;

			moveSpeed = MOVESPEED_D;
			attackSpeed = ATTACKSPEED_C;

			if (gender == (int)GENDER_MALE)
				tribeName = "ele_seki";
			else if (gender == (int)GENDER_FEMALE)
				tribeName = "ele_seki_f";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
	}
	//심연족
	else if (tribe / 100 == 3)
	{
		//체력D, 이동속도C, 공격속도B, 공격력D, 물방D, 마방D, 근접물리
		if (tribe % 100 == 0)	//시라스미스
		{
			hp->maxHP = GetFromIntTo(130, 150);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(9, 12);
			normalDEF = GetFromIntTo(2, 5);
			magicDEF = GetFromIntTo(2, 5);
			range = 0;
			bullet = nullptr;
			type = DEMAGE_NORMAL;

			moveSpeed = MOVESPEED_C;
			attackSpeed = ATTACKSPEED_B;

			if (gender == (int)GENDER_MALE)
				tribeName = "deep_sira";
			else if (gender == (int)GENDER_FEMALE)
				tribeName = "deep_sira_f";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
		//체력S, 이동속도E, 공격속도C, 공격력A, 물방E, 마방E, 근접물리
		else if (tribe % 100 == 1)	//구=챠구
		{
			hp->maxHP = GetFromIntTo(330, 350);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(18, 24);
			normalDEF = GetFromIntTo(0, 3);
			magicDEF = GetFromIntTo(0, 3);
			range = 0;
			bullet = nullptr;
			type = DEMAGE_NORMAL;

			moveSpeed = MOVESPEED_E;
			attackSpeed = ATTACKSPEED_C;

			if (gender == (int)GENDER_MALE)
				tribeName = "deep_gucha";
			else if (gender == (int)GENDER_FEMALE)
				tribeName = "deep_gucha_f";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
		//체력C, 이동속도D, 공격속도D, 공격력S, 물방C, 마방B, 근접물리
		else if (tribe % 100 == 2)	//다경
		{
			hp->maxHP = GetFromIntTo(180, 200);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(21, 28);
			normalDEF = GetFromIntTo(4, 7);
			magicDEF = GetFromIntTo(6, 9);
			range = 0;
			bullet = nullptr;
			type = DEMAGE_NORMAL;

			moveSpeed = MOVESPEED_D;
			attackSpeed = ATTACKSPEED_D;

			if (gender == (int)GENDER_MALE)
				tribeName = "deep_dagyo";
			else if (gender == (int)GENDER_FEMALE)
				tribeName = "deep_dagyo_f";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
		//체력C, 이동속도D, 공격속도D, 공격력S, 물방D, 마방S, 원거리마법
		else if (tribe % 100 == 3)	//타락한 옥좌
		{
			hp->maxHP = GetFromIntTo(180, 200);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(21, 28);
			normalDEF = GetFromIntTo(2, 5);
			magicDEF = GetFromIntTo(10, 13);
			range = 300.0f;
			bullet = new Bullet(7, demage, D3DXVECTOR2(BULLET_X * 3.0f, 0.0f), D3DXVECTOR2(BULLET_X * 4.0f, BULLET_Y));
			type = DEMAGE_MAGIC;

			moveSpeed = MOVESPEED_D;
			attackSpeed = ATTACKSPEED_D;

			if (gender == (int)GENDER_MALE)
				tribeName = "deep_master";
			else if (gender == (int)GENDER_FEMALE)
				tribeName = "deep_master_f";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
	}
	//아인족
	else if (tribe / 100 == 4)
	{
		//체력D, 이동속도A, 공격속도C, 공격력E, 물방E, 마방E, 중거리물리
		if (tribe % 100 == 0)	//켄타우마
		{
			hp->maxHP = GetFromIntTo(130, 150);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(6, 8);
			normalDEF = GetFromIntTo(0, 3);
			magicDEF = GetFromIntTo(0, 3);
			range = 250.0f;
			bullet = new Bullet(7, demage, D3DXVECTOR2(BULLET_X * 3.0f, 0.0f), D3DXVECTOR2(BULLET_X * 4.0f, BULLET_Y));
			type = DEMAGE_NORMAL;

			moveSpeed = MOVESPEED_A;
			attackSpeed = ATTACKSPEED_C;

			if (gender == (int)GENDER_MALE)
				tribeName = "demi_kenta";
			else if (gender == (int)GENDER_FEMALE)
				tribeName = "demi_kenta_f";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
		//체력D, 이동속도C, 공격속도A, 공격력D, 물방C, 마방D, 근접물리
		else if (tribe % 100 == 1)	//스몬
		{
			hp->maxHP = GetFromIntTo(130, 150);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(9, 12);
			normalDEF = GetFromIntTo(4, 7);
			magicDEF = GetFromIntTo(2, 5);
			range = 0;
			bullet = nullptr;
			type = DEMAGE_NORMAL;

			moveSpeed = MOVESPEED_C;
			attackSpeed = ATTACKSPEED_A;

			if (gender == (int)GENDER_MALE)
				tribeName = "demi_sumo";
			else if (gender == (int)GENDER_FEMALE)
				tribeName = "demi_sumo_f";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
		//체력D, 이동속도C, 공격속도C, 공격력S, 물방E, 마방C, 원거리마법
		else if (tribe % 100 == 2)	//유킨도
		{
			hp->maxHP = GetFromIntTo(130, 150);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(21, 28);
			normalDEF = GetFromIntTo(0, 3);
			magicDEF = GetFromIntTo(4, 7);
			range = 300.0f;
			bullet = new Bullet(7, demage, D3DXVECTOR2(BULLET_X * 3.0f, 0.0f), D3DXVECTOR2(BULLET_X * 4.0f, BULLET_Y));
			type = DEMAGE_MAGIC;

			moveSpeed = MOVESPEED_C;
			attackSpeed = ATTACKSPEED_A;

			if (gender == (int)GENDER_MALE)
				tribeName = "demi_yuki";
			else if (gender == (int)GENDER_FEMALE)
				tribeName = "demi_yuki_f";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
		//체력B, 이동속도A, 공격속도C, 공격력S, 물방A, 마방A, 근접마법
		else if (tribe % 100 == 3)	//QPD
		{
			hp->maxHP = GetFromIntTo(230, 250);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(21, 28);
			normalDEF = GetFromIntTo(8, 11);
			magicDEF = GetFromIntTo(8, 11);
			range = 0.0f;
			bullet = nullptr;
			type = DEMAGE_MAGIC;

			moveSpeed = MOVESPEED_A;
			attackSpeed = ATTACKSPEED_C;

			if (gender == (int)GENDER_MALE)
				tribeName = "demi_qpd";
			else if (gender == (int)GENDER_FEMALE)
				tribeName = "demi_qpd_f";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
	}
	//마족
	else if (tribe / 100 == 5)
	{
		//체력E, 이동속도C, 공격속도C, 공격력D, 물방E, 마방D, 원거리마법
		if (tribe % 100 == 0)	//마가타마고
		{
			hp->maxHP = GetFromIntTo(80, 100);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(9, 12);
			normalDEF = GetFromIntTo(0, 3);
			magicDEF = GetFromIntTo(2, 5);
			range = 300.0f;
			bullet = new Bullet(7, demage, D3DXVECTOR2(BULLET_X * 3.0f, 0.0f), D3DXVECTOR2(BULLET_X * 4.0f, BULLET_Y));
			type = DEMAGE_MAGIC;

			moveSpeed = MOVESPEED_C;
			attackSpeed = ATTACKSPEED_C;

			if (gender == (int)GENDER_MALE)
				tribeName = "demo_maga";
			else if (gender == (int)GENDER_FEMALE)
				tribeName = "demo_maga_f";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
		//체력D, 이동속도D, 공격속도D, 공격력B, 물방D, 마방B, 원거리마법
		else if (tribe % 100 == 1)	//서큐버스
		{
			hp->maxHP = GetFromIntTo(130, 150);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(15, 20);
			normalDEF = GetFromIntTo(2, 5);
			magicDEF = GetFromIntTo(6, 9);
			range = 300.0f;
			bullet = new Bullet(7, demage, D3DXVECTOR2(BULLET_X * 3.0f, 0.0f), D3DXVECTOR2(BULLET_X * 4.0f, BULLET_Y));
			type = DEMAGE_MAGIC;

			moveSpeed = MOVESPEED_D;
			attackSpeed = ATTACKSPEED_D;

			if (gender == (int)GENDER_MALE)
				tribeName = "demo_sachu";
			else if (gender == (int)GENDER_FEMALE)
				tribeName = "demo_sachu_f";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
		//체력B, 이동속도C, 공격속도B, 공격력B, 물방C, 마방C, 근접물리
		else if (tribe % 100 == 2)	//마톤
		{
			hp->maxHP = GetFromIntTo(230, 250);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(15, 20);
			normalDEF = GetFromIntTo(4, 7);
			magicDEF = GetFromIntTo(4, 7);
			range = 0.0f;
			bullet = nullptr;
			type = DEMAGE_NORMAL;

			moveSpeed = MOVESPEED_C;
			attackSpeed = ATTACKSPEED_B;

			if (gender == (int)GENDER_MALE)
				tribeName = "demo_maton";
			else if (gender == (int)GENDER_FEMALE)
				tribeName = "demo_maton_f";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
		//체력D, 이동속도B, 공격속도B, 공격력A, 물방B, 마방B, 근접마법
		else if (tribe % 100 == 3)	//드라큘라라
		{
			hp->maxHP = GetFromIntTo(130, 150);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(18, 24);
			normalDEF = GetFromIntTo(6, 9);
			magicDEF = GetFromIntTo(6, 9);
			range = 0.0f;
			bullet = nullptr;
			type = DEMAGE_MAGIC;

			moveSpeed = MOVESPEED_B;
			attackSpeed = ATTACKSPEED_B;

			if (gender == (int)GENDER_MALE)
				tribeName = "demo_dra";
			else if (gender == (int)GENDER_FEMALE)
				tribeName = "demo_dra_f";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
	}
	//애니멀 족
	else if (tribe / 100 == 6)
	{
		//체력D, 이동속도A, 공격속도A, 공격력C, 물방E, 마방E, 근접물리
		if (tribe % 100 == 0)	//울프가이
		{
			hp->maxHP = GetFromIntTo(100, 120);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(12, 16);
			normalDEF = GetFromIntTo(0, 3);
			magicDEF = GetFromIntTo(0, 3);
			range = 0;
			bullet = nullptr;
			type = DEMAGE_NORMAL;

			moveSpeed = MOVESPEED_A;
			attackSpeed = ATTACKSPEED_A;

			if (gender == (int)GENDER_MALE)
				tribeName = "ani_wolf";
			else if (gender == (int)GENDER_FEMALE)
				tribeName = "ani_wolf_f";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
		//체력D, 이동속도E, 공격속도E, 공격력S, 물방E, 마방E, 초원거리물리
		else if (tribe % 100 == 1)	//마이홈
		{
			hp->maxHP = GetFromIntTo(130, 150);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(21, 28);
			normalDEF = GetFromIntTo(0, 3);
			magicDEF = GetFromIntTo(0, 3);
			range = 350;
			bullet = new Bullet(7, demage, D3DXVECTOR2(BULLET_X * 3.0f, 0.0f), D3DXVECTOR2(BULLET_X * 4.0f, BULLET_Y));
			type = DEMAGE_NORMAL;

			moveSpeed = MOVESPEED_E;
			attackSpeed = ATTACKSPEED_E;

			if (gender == (int)GENDER_MALE)
				tribeName = "ani_mai";
			else if (gender == (int)GENDER_FEMALE)
				tribeName = "ani_mai_f";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
		//체력C, 이동속도A, 공격속도S, 공격력A, 물방D, 마방C, 근접물리
		else if (tribe % 100 == 2)	//냐이킹
		{
			hp->maxHP = GetFromIntTo(180, 200);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(18, 24);
			normalDEF = GetFromIntTo(2, 5);
			magicDEF = GetFromIntTo(4, 7);
			range = 0;
			bullet = nullptr;
			type = DEMAGE_NORMAL;

			moveSpeed = MOVESPEED_A;
			attackSpeed = ATTACKSPEED_S;

			if (gender == (int)GENDER_MALE)
				tribeName = "ani_nya";
			else if (gender == (int)GENDER_FEMALE)
				tribeName = "ani_nya_f";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
		//체력D, 이동속도C, 공격속도D, 공격력S, 물방C, 마방D, 초원거리마법
		else if (tribe % 100 == 3)	//베알라
		{
			hp->maxHP = GetFromIntTo(130, 150);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(21, 28);
			normalDEF = GetFromIntTo(4, 7);
			magicDEF = GetFromIntTo(2, 5);
			range = 350;
			bullet = new Bullet(7, demage, D3DXVECTOR2(BULLET_X * 3.0f, 0.0f), D3DXVECTOR2(BULLET_X * 4.0f, BULLET_Y));;
			type = DEMAGE_MAGIC;

			moveSpeed = MOVESPEED_C;
			attackSpeed = ATTACKSPEED_D;

			if (gender == (int)GENDER_MALE)
				tribeName = "ani_bear";
			else if (gender == (int)GENDER_FEMALE)
				tribeName = "ani_bear_f";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
	}
	else if (tribe / 100 == 7)
	{
		//
		if (tribe % 100 == 0)	//시민
		{
			hp->maxHP = GetFromIntTo(60, 70);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(6, 8);
			normalDEF = GetFromIntTo(0, 3);
			magicDEF = GetFromIntTo(0, 3);
			range = 0;
			bullet = nullptr;
			type = DEMAGE_NORMAL;

			moveSpeed = MOVESPEED_C;
			attackSpeed = ATTACKSPEED_D;
			tribeName = "brave_village";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
		//
		else if (tribe % 100 == 1)	//강도
		{
			hp->maxHP = GetFromIntTo(80, 100);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(9, 12);
			normalDEF = GetFromIntTo(2, 4);
			magicDEF = GetFromIntTo(1, 3);
			range = 0;
			bullet = nullptr;
			type = DEMAGE_NORMAL;

			moveSpeed = MOVESPEED_C;
			attackSpeed = ATTACKSPEED_C;
			tribeName = "brave_bad";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
		//
		else if (tribe % 100 == 2)	//마법사(파랑)
		{
			hp->maxHP = GetFromIntTo(80, 100);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(11, 15);
			normalDEF = GetFromIntTo(0, 3);
			magicDEF = GetFromIntTo(3, 6);
			range = 300.0f;
			bullet = new Bullet(7, demage, D3DXVECTOR2(BULLET_X * 3.0f, 0.0f), D3DXVECTOR2(BULLET_X * 4.0f, BULLET_Y));
			type = DEMAGE_MAGIC;

			moveSpeed = MOVESPEED_D;
			attackSpeed = ATTACKSPEED_C;
			tribeName = "brave_wiz_blue";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
		//
		else if (tribe % 100 == 3)	//도적
		{
			hp->maxHP = GetFromIntTo(75, 85);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(9, 12);
			normalDEF = GetFromIntTo(1, 3);
			magicDEF = GetFromIntTo(0, 3);
			range = 0;
			bullet = nullptr;
			type = DEMAGE_NORMAL;

			moveSpeed = MOVESPEED_B;
			attackSpeed = ATTACKSPEED_B;
			tribeName = "brave_thief";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
		//
		else if (tribe % 100 == 4)	//강도두목
		{
			hp->maxHP = GetFromIntTo(180, 200);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(15, 18);
			normalDEF = GetFromIntTo(4, 7);
			magicDEF = GetFromIntTo(4, 7);
			range = 0;
			bullet = nullptr;
			type = DEMAGE_NORMAL;

			moveSpeed = MOVESPEED_B;
			attackSpeed = ATTACKSPEED_B;
			tribeName = "brave_badmaster";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
		else if (tribe % 100 == 5)	//마법사(레드)
		{
			hp->maxHP = GetFromIntTo(80, 100);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(16, 16);
			normalDEF = GetFromIntTo(0, 3);
			magicDEF = GetFromIntTo(4, 7);
			range = 300.0f;
			bullet = new Bullet(7, demage, D3DXVECTOR2(BULLET_X * 3.0f, 0.0f), D3DXVECTOR2(BULLET_X * 4.0f, BULLET_Y));
			type = DEMAGE_MAGIC;

			moveSpeed = MOVESPEED_D;
			attackSpeed = ATTACKSPEED_B;
			tribeName = "brave_wiz_blue";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
		else if (tribe % 100 == 6)	//봉골
		{
			hp->maxHP = GetFromIntTo(90, 90);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(9, 9);
			normalDEF = GetFromIntTo(2, 2);
			magicDEF = GetFromIntTo(2, 2);
			range = 0;
			bullet = nullptr;
			type = DEMAGE_NORMAL;

			moveSpeed = MOVESPEED_B;
			attackSpeed = ATTACKSPEED_B;
			tribeName = "brave_bad";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));
			colBox->Position(-2.0f, -12.0f);
		}
	}

	colBox->SetParent(img->GetRT());
	hp->hp->SetParent(img->GetRT());
}

void Character::RandomState()
{
	state = GetInt(STATE_SIZE) + 1;
}

void Character::BulletSet()
{
	if (bullet != nullptr)
	{
		if(rotate.y == 180.0f * TORADIAN)
			bullet->setBuletPosNDir(pos, D3DXVECTOR2(1.0f, 0.0f));
		else if (rotate.y == 0.0f)
			bullet->setBuletPosNDir(pos, D3DXVECTOR2(-1.0f, 0.0f));
	}
}

//공격
void Character::Attack()
{
	//원거리 시 총알 셋팅
	BulletSet();

	//현재 존재하는 적들을 찾음
	vector<Character*> ctr = CHARACTERMANAGER->GetAllBrave();
	Character* nearCtr = nullptr;

	//가장 가까운 적 탐색하여
	//nearCtr이 가리키도록 함
	for (int i = 0; i < ctr.size(); i++)
	{
		
		if (nearCtr == nullptr)	//맨 처음 적을 넣어둠
			nearCtr = ctr.at(i);
		//다른적이 현재 같은층이고 현재 타겟이 다른층이라면
		else if (currentFloor == ctr.at(i)->GetCurrentFloor() &&
				 nearCtr->GetCurrentFloor() != ctr.at(i)->GetCurrentFloor())
			nearCtr = ctr.at(i);
		//현재 타겟과 다른적이 같은층이지만 다른적이 더 가깝다면
		else if (fabs(nearCtr->GetPos().x - pos.x) > fabs(ctr.at(i)->GetPos().x - pos.x))
			nearCtr = ctr.at(i);
	}
	//주변 적이 있다면
	if (nearCtr != nullptr)
	{
		//이동 방향에 따른 회전
		if (nearCtr->GetPos().x - pos.x < 0)
			rotate = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		if (nearCtr->GetPos().x - pos.x > 0)
			rotate = D3DXVECTOR3(0.0f, 180.0f * TORADIAN, 0.0f);

		//현재 층에 없을 경우
		//해당 층까지 이동
		if (currentFloor != nearCtr->GetCurrentFloor())
		{
			MovePos(nearCtr->GetCurrentFloor(), nearCtr->GetPos().x, STATE_BATTLE);
		}
		//현재 층에 있을 경우
		else
		{
			//근접캐릭
			if (range <= 0.0f)
			{
				//가까이 붙었다면
				if (IntersectRect(nearCtr->GetColboxRect(), colBox->GetRect()) && nearCtr->GetColor().a >= 1.0f && color.a >= 1.0f)
				{
					//애니메이션 공격으로 셋팅
					currentFrameY = 3;

					//현재 애니메이션이 적을 공격하는 모션이라면
					if (aniSpeed == 0.0f && currentFrameX == 0)
					{
						//공격이 들어감
						int a = 0;
						if (type == DEMAGE_NORMAL)	//물리공격
							a = demage - nearCtr->GetNormalDEF();	//물리방어력 만큼 빼기
						else if (type == DEMAGE_MAGIC)	//마법공격
							a = demage - nearCtr->GetMagicDEF();	//마법방어력 만큼 빼기

						//방어력이 더 높아 체력을 회복시키는 행위 방지
						if (a <= 0)
							a = 1;

						//대미지 적용
						nearCtr->SetCurrentHp(nearCtr->GetCurrentHP() - a);
					}
				}
				//거리가 멀다면
				else
					MovePosX(nearCtr->GetPos().x);	//이동
			}
			//원거리 캐릭
			else
			{
				if (fabs(nearCtr->GetPos().x - pos.x) > range)
					MovePosX(nearCtr->GetPos().x);
				else if(fabs(nearCtr->GetPos().x - pos.x) <= range && nearCtr->GetColor() >= 1.0f)
				{
					currentFrameY = 3;
				}
			}
		}
	}
	else
	{
		if(color.a >= 1.0f)
		state = (int)STATE_GOTO_HOME;
	}
}

void Character::MoveFloor(int targetFloor, float flootHeight)
{
	int goal = targetFloor - currentFloor;

	if (color.a > 0.0f && time <= 0.5f)
	{
		color.a -= 4 * g_ETime;
		currentFrameY = 2;
	}
	if (time >= 1.8f && color.a < 1.0f)
	{
		color.a += 4 * g_ETime;
		currentFrameY = 0;
	}

	if (goal > 0)
	{
		if (pos.y < flootHeight)
		{
			pos.y += 50.0f * 1.0f * g_ETime;
			time += g_ETime;
		}
		if (color.a >= 1.0f && pos.y >= flootHeight)
		{
			time = 0.0f;
			currentFloor++;
		}
	}

	if (goal < 0)
	{
		if (pos.y > flootHeight)
		{
			pos.y -= 50.0f * 1.0f * g_ETime;
			time += g_ETime;
		}
		
		if(color.a >= 1.0f && pos.y <= flootHeight)
		{
			time = 0.0f;
			currentFloor--;
		}
	}
}

void Character::MovePosX(float goalPosX)
{
	currentFrameY = 1;
	D3DXVECTOR2 dir;

	if (goalPosX - pos.x < 0)
	{
		dir = D3DXVECTOR2(-1.0f, 0.0f);
		rotate = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	
	if (goalPosX - pos.x > 0)
	{
		dir = D3DXVECTOR2(1.0f, 0.0f);
		rotate = D3DXVECTOR3(0.0f, 180.0f * TORADIAN, 0.0f);
	}

	if (pos.x > goalPosX + 0.5f || pos.x < goalPosX - 0.5f)
	{
		if (time <= 0.0f)
			pos.x += (40.0f / moveSpeed) * dir.x * g_ETime;
		else
			time -= g_ETime;
	}
	else
	{
		pos.x = goalPosX;
	}
}

//이동 층, 위치, 이동 후 바뀌는 상태(State)
void Character::MovePos(int targetFloor, float goalPosX, State st)
{
	if (targetFloor - currentFloor > 0)
	{
		MovePosX(upFloor.at(currentFloor).x);

		if (pos.x == upFloor.at(currentFloor).x)
			MoveFloor(targetFloor, downFloor.at(currentFloor).y);
	}
	else if (targetFloor - currentFloor < 0)
	{
		MovePosX(downFloor.at(currentFloor - 1).x);

		if (pos.x == downFloor.at(currentFloor - 1).x)
			MoveFloor(targetFloor, upFloor.at(currentFloor - 1).y);
	}
	else if (targetFloor == currentFloor)
	{
		MovePosX(goalPosX);

		if (pos.x == goalPosX)
			state = (int)st;
	}
}

//용사(적) AI
void Character::BraveAI()
{
	//원거리 캐릭 이라면
	//방향에 따른 총알 로테이트 수정
	BulletSet();

	//현재 전투상태인 몬스터들을 가져옴
	vector<Character*> ctr = CHARACTERMANAGER->GetSameStateMonsters((int)STATE_BATTLE);
	//가장 가까운 몬스터 가리키는 포인터
	Character* nearCtr = nullptr;

	for (int i = 0; i < ctr.size(); i++)
	{
		if (nearCtr == nullptr)
			nearCtr = ctr.at(i);
		else if (fabs(nearCtr->GetPos().x - pos.x) > fabs(ctr.at(i)->GetPos().x - pos.x) &&
			currentFloor == ctr.at(i)->GetCurrentFloor())
			nearCtr = ctr.at(i);
	}

	//가까운 적이 존재하고 같은층에 있고 자신이 계단을 이용중이 아니라면
	if (nearCtr != nullptr && currentFloor == nearCtr->GetCurrentFloor() && color.a >= 1.0f)
	{
		//근접공격 캐릭터라면
		if (range <= 0.0f)
		{
			//서로 밀착된 상태라면
			if (IntersectRect(nearCtr->GetColboxRect(), colBox->GetRect()))
			{
				//전투 상태 애니메이션으로 변경
				currentFrameY = 3;

				if (aniSpeed == 0.0f && currentFrameX == 0)
				{
					int a = 0;

					//물리 혹은 마법대미지에 따라 방어력 따로 적용
					if (type == DEMAGE_NORMAL)
						a = demage - nearCtr->GetNormalDEF();
					else if (type == DEMAGE_MAGIC)
						a =  demage - nearCtr->GetMagicDEF();

					//방어력이 더 높아 체력을 회복시키는 행위 방지
					if (a <= 0)
						a = 1;

					//대미지를 입음
					nearCtr->SetCurrentHp(nearCtr->GetCurrentHP() - a);
				}
			}
			//거리가 있다면 가까이 이동
			else
				MovePosX(nearCtr->GetPos().x);
		}
		//원거리 캐릭터라면
		else
		{
			if (fabs(nearCtr->GetPos().x - pos.x) > range)
				MovePosX(nearCtr->GetPos().x);
			else if(nearCtr->GetColor().a >= 1.0f && fabs(nearCtr->GetPos().x - pos.x) <= range)
			{
				currentFrameY = 3;

				if (aniSpeed == 0.0f && currentFrameX == 0)
					bullet->Fire();
			}
		}
	}
	else
	{
		if (gold <= 0)
		{
			if (kingDoor->GetStep() == currentFloor && kingDoor->GetPosX() == pos.x)
			{
				time += g_ETime;

				if (time < 3.0f)
				{
					currentFrameY = 2;

					if (color.a > 0.0f)
						color.a -= 2 * g_ETime;
				}
				else
				{
					currentFrameY = 0;

					if (color.a < 1.0f)
					{
						color.a += 2 * g_ETime;

						if(color.a >= 1.0f)
						{
							time = 0.0f;
							playerGold -= 50 * (QUESTMANAGER->GetOrderQuest()->GetQuestDif() + 1);
							gold += 50 * (QUESTMANAGER->GetOrderQuest()->GetQuestDif() + 1);
						}

					}
				}

			}
			else
				MovePos(kingDoor->GetStep(), kingDoor->GetPosX(), STATE_BRAVE);
		}
		else
			MovePos(0, -100.0f, STATE_DEAD);
	}
	
	if(bullet && nearCtr != nullptr)
		bullet->BulletHit(nearCtr, type);

	if (hp->currentHP <= 0)
	{
		if (gold > 0)
		{
			playerGold += gold;
			gold = 0;
		}
		time = 0.0f;
	}
}

//몬스터(아군)AI
void Character::AI()
{
	if (state == (int)STATE_NONE && pos.x > -40.0f &&
		CHARACTERMANAGER->GetAllBrave().size() <= 0 )
	{
		if (hp->currentHP < hp->maxHP)
		{
			if (time < 2.0f)
				time += g_ETime;

			if (time >= 2)
			{
				hp->currentHP += 1;
				time = 0.0f;
			}
		}

		if (workTime > 0.0f && gameTime / 60.0f / 60.0f >= 6.5f && gameTime / 60.0f / 60.0f <= 19.5f)
			workTime -= g_ETime;
		else if(workTime <= 0.0f)
		{
			workTime = (float)(GetInt(50) + 50);
			time = 0.0f;
			state = (int)STATE_OUT_HOME;
		}
	}

	//집으로 가기
	if (state == (int)STATE_GOTO_HOME)
	{
		//이동 층, 위치, 이동 후 바뀌는 상태(State)
		MovePos(home->step, home->doorPosX, STATE_IN_HOME);
	}
	//집에 들어가고 있을때
	else if (state == (int)STATE_IN_HOME)
	{
		//애니메이션 프레임 Y축 설정
		currentFrameY = 2;

		//디테일 묘사용 알파값
		if (color.a > 0.0f)
		{
			//play(사운드 이름, 볼륨)
			SOUNDMANAGER->play("door_open_sound", 0.3f);
			color.a -= 2 * g_ETime;//델타타임(g_ETime)
		}
		if (color.a <= 0.0f)
		{
			SOUNDMANAGER->play("door_close_sound", 0.3f);
			pantsRun = false;
			state = (int)STATE_NONE;
		}
	}
	else if (state == (int)STATE_GOTO_WORK)
	{
		MovePos(0, -50.0f, STATE_GOTO_WORK);

		if (pos.x <= -40.0f)
			workTime -= g_ETime;

		if (workTime <= 0.0f)
		{
			workTime = (float)(GetInt(50) + 50);
			time = 0.0f;
			state = (int)STATE_GOTO_HOME;
		}
	}
	else if (state == (int)STATE_OUT_HOME)
	{
		time = 0.0f;

		currentFrameY = 0;

		if (color.a < 1.0f)
		{
			SOUNDMANAGER->play("door_open_sound", 0.3f);
			color.a += 2 * g_ETime;
		}
		else
		{
			SOUNDMANAGER->play("door_close_sound", 0.3f);
			state = (int)STATE_GOTO_WORK;
		}
	}
	//전투 상태
	else if (state == (int)STATE_BATTLE)
	{
		//전투중 캐릭터 재 클릭시 집으로 도주
		//이후 일정시간마다 체력이 채워짐
		if (PtInRect(colBox->GetRect(), g_ptMouse) &&
			KEYMANAGER->isKeyDown(VK_LBUTTON))
		{
			pantsRun = true;
		}
		
		if(!pantsRun)	//공격
			Attack();		
		else	//도주
			MovePos(home->step, home->doorPosX, STATE_IN_HOME);
	}
	else if (state == (int)STATE_FIND_HOUSE)
	{
		vector<Character*> ccc = CHARACTERMANAGER->GetSameStateMonsters((int)STATE_FIND_HOUSE);

		for (int i = 0; i < ccc.size(); i++)
		{
			if (ccc.at(i)->GetName() != name)
			{
				if (state == (int)STATE_FIND_HOUSE &&
					IntersectRect(ccc.at(i)->GetColboxRect(), colBox->GetRect()))
				{
					if (pos.x < ccc.at(i)->GetPos().x)
						time = 1.0f;
				}
			}
		}

		ccc = CHARACTERMANAGER->GetSameStateMonsters((int)STATE_LIKE_ROOM);

		for (int i = 0; i < ccc.size(); i++)
		{
			if (ccc.at(i)->GetName() != name)
			{
				if (state == (int)STATE_FIND_HOUSE &&
					IntersectRect(ccc.at(i)->GetColboxRect(), colBox->GetRect()))
				{
					time = 1.0f;
				}
			}
		}

		MovePosX(WINSIZEX / 7 * 2 + 30.0f);

		if (time > 0.0f && pos.x < (WINSIZEX / 7 * 2 + 30.0f))
		{
			currentFrameX = 0;
		}

		if (pos.x == (WINSIZEX / 7 * 2 + 30.0f))
		{
			currentFrameX = 0;
			currentFrameY = 2;
			time += g_ETime;

			if (time > 1.5f)
			{
				workTime = (float)(GetInt(50) + 50);
				time = 0.0f;
				state = (int)STATE_LIKE_ROOM;
			}
		}
	}
	else if (state == (int)STATE_LIKE_ROOM)
	{
		currentFrameY = 2;

		time += g_ETime;

		if (time > 5.0f)
		{
			state = (int)STATE_KICK;
			time = 0;
		}
	}
	else if (state == (int)STATE_KICK)
	{
		MovePos(0, -50.0f, STATE_DEAD);
	}

	if (state == (int)STATE_DEAD)
	{
		hp->currentHP = 0;
	}

	for (int i = 0; i < CHARACTERMANAGER->GetAllBrave().size(); i++)
	{
		if (bullet && CHARACTERMANAGER->GetAllBrave().at(i) != nullptr)
			bullet->BulletHit(CHARACTERMANAGER->GetAllBrave().at(i), type);
	}
}
