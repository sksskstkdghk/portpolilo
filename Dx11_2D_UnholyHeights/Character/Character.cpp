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

//�� ������ ��ü��
void Character::MonsterTribeInfo(int tribe)
{
	colBox = new Rect();

	//�ǿ丮����
	if (tribe / 100 == 0)
	{
		//ü��E, �̵��ӵ�C, ���ݼӵ�S, ���ݷ�E, ����E, ����E, ��������
		if (tribe % 100 == 0)	//�ǿ丮��
		{
			//GetFromIntTo(min, max) ���� ��Ʈ��
			hp->maxHP = GetFromIntTo(80, 100);
			hp->currentHP = hp->maxHP;
			demage = GetFromIntTo(6, 8);
			normalDEF = GetFromIntTo(0, 3);	//���� ����
			magicDEF = GetFromIntTo(0, 3);	//���� ����
			range = 0;						//�����Ÿ�(0�̸� ��������)
			bullet = nullptr;				//�Ѿ�
			type = DEMAGE_NORMAL;			//���� Ÿ��(�������� �Ǵ� ��������)

			moveSpeed = MOVESPEED_C;
			attackSpeed = ATTACKSPEED_S;

			if (gender == (int)GENDER_MALE)	//����
				tribeName = "piyo_piyo";
			else if (gender == (int)GENDER_FEMALE)
				tribeName = "piyo_piyo_f";

			img = IMGMANAGER->GetImage(tribeName);

			colBox->Scale(D3DXVECTOR2(50.0f, 50.0f));	//�浹�ڽ� ũ��
			colBox->Position(-2.0f, -12.0f);			//�浹�ڽ� ��ġ
		}
		//ü��D, �̵��ӵ�C, ���ݼӵ�C, ���ݷ�B, ����E, ����C, ���Ÿ�����
		else if (tribe % 100 == 1)	//�����ǿ�
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
		//ü��D, �̵��ӵ�D, ���ݼӵ�B, ���ݷ�E, ����A, ����C, �߰Ÿ�����
		else if (tribe % 100 == 2)	//īŸ�ǿ�
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
		//ü��D, �̵��ӵ�C, ���ݼӵ�B, ���ݷ�E, ����E, ����A, �߰Ÿ�����
		else if (tribe % 100 == 3)	//Ű���ǿ�
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
	//�𵥵� ��
	else if(tribe / 100 == 1)
	{
		//ü��D, �̵��ӵ�C, ���ݼӵ�B, ���ݷ�E, ����D, ����E, ��������
		if (tribe % 100 == 0)	//ȣ��ư
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
		//ü��C, �̵��ӵ�E, ���ݼӵ�C, ���ݷ�C, ����D, ����E, ��������
		else if (tribe % 100 == 1)	//����
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
		//ü��D, �̵��ӵ�C, ���ݼӵ�B, ���ݷ�A, ����B, ����B, ��������
		else if (tribe % 100 == 2)	//ȣ�׹���
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
		//ü��C, �̵��ӵ�B, ���ݼӵ�C, ���ݷ�B, ����S, ����E, �ʿ��Ÿ�����
		else if (tribe % 100 == 3)	//����Ű��
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
	//������Ʈ ��
	else if (tribe / 100 == 2)
	{
		//ü��E, �̵��ӵ�D, ���ݼӵ�D, ���ݷ�D, ����E, ����E, ���Ÿ�����
		if (tribe % 100 == 0)	//�����
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
		//ü��D, �̵��ӵ�D, ���ݼӵ�C, ���ݷ�D, ����E, ����E, �ʿ��Ÿ�����
		else if (tribe % 100 == 1)	//�����ϳ�
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
		//ü��D, �̵��ӵ�C, ���ݼӵ�B, ���ݷ�B, ����C, ����D, �߰Ÿ�����
		else if (tribe % 100 == 2)	//����
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
		//ü��C, �̵��ӵ�D, ���ݼӵ�C, ���ݷ�E, ����S, ����S, ��������
		else if (tribe % 100 == 3)	//��Ű����
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
	//�ɿ���
	else if (tribe / 100 == 3)
	{
		//ü��D, �̵��ӵ�C, ���ݼӵ�B, ���ݷ�D, ����D, ����D, ��������
		if (tribe % 100 == 0)	//�ö󽺹̽�
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
		//ü��S, �̵��ӵ�E, ���ݼӵ�C, ���ݷ�A, ����E, ����E, ��������
		else if (tribe % 100 == 1)	//��=í��
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
		//ü��C, �̵��ӵ�D, ���ݼӵ�D, ���ݷ�S, ����C, ����B, ��������
		else if (tribe % 100 == 2)	//�ٰ�
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
		//ü��C, �̵��ӵ�D, ���ݼӵ�D, ���ݷ�S, ����D, ����S, ���Ÿ�����
		else if (tribe % 100 == 3)	//Ÿ���� ����
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
	//������
	else if (tribe / 100 == 4)
	{
		//ü��D, �̵��ӵ�A, ���ݼӵ�C, ���ݷ�E, ����E, ����E, �߰Ÿ�����
		if (tribe % 100 == 0)	//��Ÿ�츶
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
		//ü��D, �̵��ӵ�C, ���ݼӵ�A, ���ݷ�D, ����C, ����D, ��������
		else if (tribe % 100 == 1)	//����
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
		//ü��D, �̵��ӵ�C, ���ݼӵ�C, ���ݷ�S, ����E, ����C, ���Ÿ�����
		else if (tribe % 100 == 2)	//��Ų��
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
		//ü��B, �̵��ӵ�A, ���ݼӵ�C, ���ݷ�S, ����A, ����A, ��������
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
	//����
	else if (tribe / 100 == 5)
	{
		//ü��E, �̵��ӵ�C, ���ݼӵ�C, ���ݷ�D, ����E, ����D, ���Ÿ�����
		if (tribe % 100 == 0)	//����Ÿ����
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
		//ü��D, �̵��ӵ�D, ���ݼӵ�D, ���ݷ�B, ����D, ����B, ���Ÿ�����
		else if (tribe % 100 == 1)	//��ť����
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
		//ü��B, �̵��ӵ�C, ���ݼӵ�B, ���ݷ�B, ����C, ����C, ��������
		else if (tribe % 100 == 2)	//����
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
		//ü��D, �̵��ӵ�B, ���ݼӵ�B, ���ݷ�A, ����B, ����B, ��������
		else if (tribe % 100 == 3)	//���ŧ���
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
	//�ִϸ� ��
	else if (tribe / 100 == 6)
	{
		//ü��D, �̵��ӵ�A, ���ݼӵ�A, ���ݷ�C, ����E, ����E, ��������
		if (tribe % 100 == 0)	//��������
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
		//ü��D, �̵��ӵ�E, ���ݼӵ�E, ���ݷ�S, ����E, ����E, �ʿ��Ÿ�����
		else if (tribe % 100 == 1)	//����Ȩ
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
		//ü��C, �̵��ӵ�A, ���ݼӵ�S, ���ݷ�A, ����D, ����C, ��������
		else if (tribe % 100 == 2)	//����ŷ
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
		//ü��D, �̵��ӵ�C, ���ݼӵ�D, ���ݷ�S, ����C, ����D, �ʿ��Ÿ�����
		else if (tribe % 100 == 3)	//���˶�
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
		if (tribe % 100 == 0)	//�ù�
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
		else if (tribe % 100 == 1)	//����
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
		else if (tribe % 100 == 2)	//������(�Ķ�)
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
		else if (tribe % 100 == 3)	//����
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
		else if (tribe % 100 == 4)	//�����θ�
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
		else if (tribe % 100 == 5)	//������(����)
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
		else if (tribe % 100 == 6)	//����
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

//����
void Character::Attack()
{
	//���Ÿ� �� �Ѿ� ����
	BulletSet();

	//���� �����ϴ� ������ ã��
	vector<Character*> ctr = CHARACTERMANAGER->GetAllBrave();
	Character* nearCtr = nullptr;

	//���� ����� �� Ž���Ͽ�
	//nearCtr�� ����Ű���� ��
	for (int i = 0; i < ctr.size(); i++)
	{
		
		if (nearCtr == nullptr)	//�� ó�� ���� �־��
			nearCtr = ctr.at(i);
		//�ٸ����� ���� �������̰� ���� Ÿ���� �ٸ����̶��
		else if (currentFloor == ctr.at(i)->GetCurrentFloor() &&
				 nearCtr->GetCurrentFloor() != ctr.at(i)->GetCurrentFloor())
			nearCtr = ctr.at(i);
		//���� Ÿ�ٰ� �ٸ����� ������������ �ٸ����� �� �����ٸ�
		else if (fabs(nearCtr->GetPos().x - pos.x) > fabs(ctr.at(i)->GetPos().x - pos.x))
			nearCtr = ctr.at(i);
	}
	//�ֺ� ���� �ִٸ�
	if (nearCtr != nullptr)
	{
		//�̵� ���⿡ ���� ȸ��
		if (nearCtr->GetPos().x - pos.x < 0)
			rotate = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		if (nearCtr->GetPos().x - pos.x > 0)
			rotate = D3DXVECTOR3(0.0f, 180.0f * TORADIAN, 0.0f);

		//���� ���� ���� ���
		//�ش� ������ �̵�
		if (currentFloor != nearCtr->GetCurrentFloor())
		{
			MovePos(nearCtr->GetCurrentFloor(), nearCtr->GetPos().x, STATE_BATTLE);
		}
		//���� ���� ���� ���
		else
		{
			//����ĳ��
			if (range <= 0.0f)
			{
				//������ �پ��ٸ�
				if (IntersectRect(nearCtr->GetColboxRect(), colBox->GetRect()) && nearCtr->GetColor().a >= 1.0f && color.a >= 1.0f)
				{
					//�ִϸ��̼� �������� ����
					currentFrameY = 3;

					//���� �ִϸ��̼��� ���� �����ϴ� ����̶��
					if (aniSpeed == 0.0f && currentFrameX == 0)
					{
						//������ ��
						int a = 0;
						if (type == DEMAGE_NORMAL)	//��������
							a = demage - nearCtr->GetNormalDEF();	//�������� ��ŭ ����
						else if (type == DEMAGE_MAGIC)	//��������
							a = demage - nearCtr->GetMagicDEF();	//�������� ��ŭ ����

						//������ �� ���� ü���� ȸ����Ű�� ���� ����
						if (a <= 0)
							a = 1;

						//����� ����
						nearCtr->SetCurrentHp(nearCtr->GetCurrentHP() - a);
					}
				}
				//�Ÿ��� �ִٸ�
				else
					MovePosX(nearCtr->GetPos().x);	//�̵�
			}
			//���Ÿ� ĳ��
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

//�̵� ��, ��ġ, �̵� �� �ٲ�� ����(State)
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

//���(��) AI
void Character::BraveAI()
{
	//���Ÿ� ĳ�� �̶��
	//���⿡ ���� �Ѿ� ������Ʈ ����
	BulletSet();

	//���� ���������� ���͵��� ������
	vector<Character*> ctr = CHARACTERMANAGER->GetSameStateMonsters((int)STATE_BATTLE);
	//���� ����� ���� ����Ű�� ������
	Character* nearCtr = nullptr;

	for (int i = 0; i < ctr.size(); i++)
	{
		if (nearCtr == nullptr)
			nearCtr = ctr.at(i);
		else if (fabs(nearCtr->GetPos().x - pos.x) > fabs(ctr.at(i)->GetPos().x - pos.x) &&
			currentFloor == ctr.at(i)->GetCurrentFloor())
			nearCtr = ctr.at(i);
	}

	//����� ���� �����ϰ� �������� �ְ� �ڽ��� ����� �̿����� �ƴ϶��
	if (nearCtr != nullptr && currentFloor == nearCtr->GetCurrentFloor() && color.a >= 1.0f)
	{
		//�������� ĳ���Ͷ��
		if (range <= 0.0f)
		{
			//���� ������ ���¶��
			if (IntersectRect(nearCtr->GetColboxRect(), colBox->GetRect()))
			{
				//���� ���� �ִϸ��̼����� ����
				currentFrameY = 3;

				if (aniSpeed == 0.0f && currentFrameX == 0)
				{
					int a = 0;

					//���� Ȥ�� ����������� ���� ���� ���� ����
					if (type == DEMAGE_NORMAL)
						a = demage - nearCtr->GetNormalDEF();
					else if (type == DEMAGE_MAGIC)
						a =  demage - nearCtr->GetMagicDEF();

					//������ �� ���� ü���� ȸ����Ű�� ���� ����
					if (a <= 0)
						a = 1;

					//������� ����
					nearCtr->SetCurrentHp(nearCtr->GetCurrentHP() - a);
				}
			}
			//�Ÿ��� �ִٸ� ������ �̵�
			else
				MovePosX(nearCtr->GetPos().x);
		}
		//���Ÿ� ĳ���Ͷ��
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

//����(�Ʊ�)AI
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

	//������ ����
	if (state == (int)STATE_GOTO_HOME)
	{
		//�̵� ��, ��ġ, �̵� �� �ٲ�� ����(State)
		MovePos(home->step, home->doorPosX, STATE_IN_HOME);
	}
	//���� ���� ������
	else if (state == (int)STATE_IN_HOME)
	{
		//�ִϸ��̼� ������ Y�� ����
		currentFrameY = 2;

		//������ ����� ���İ�
		if (color.a > 0.0f)
		{
			//play(���� �̸�, ����)
			SOUNDMANAGER->play("door_open_sound", 0.3f);
			color.a -= 2 * g_ETime;//��ŸŸ��(g_ETime)
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
	//���� ����
	else if (state == (int)STATE_BATTLE)
	{
		//������ ĳ���� �� Ŭ���� ������ ����
		//���� �����ð����� ü���� ä����
		if (PtInRect(colBox->GetRect(), g_ptMouse) &&
			KEYMANAGER->isKeyDown(VK_LBUTTON))
		{
			pantsRun = true;
		}
		
		if(!pantsRun)	//����
			Attack();		
		else	//����
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
