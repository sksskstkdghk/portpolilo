#pragma once

//종족
enum Tribe
{
	TRIBE_NONE = -1,

	TRIBE_PIYO_PIYO = 0,
	TRIBE_PIYO_GAKU, TRIBE_PIYO_KATA, TRIBE_PIYO_KIRA,

	TRIBE_UNDEAD_HONE = 100,
	TRIBE_UNDEAD_ZOMBIE, TRIBE_UNDEAD_HONEMUSA, TRIBE_UNDEAD_OBA,

	TRIBE_ELEMENTS_MIZ = 200,
	TRIBE_ELEMENTS_MOKU, TRIBE_ELEMENTS_MERA, TRIBE_ELEMENTS_SEKI,

	TRIBE_DEEP_SIRA = 300,
	TRIBE_DEEP_GUCHA, TRIBE_DEEP_DAGYO, TRIBE_DEEP_MASTER,

	TRIBE_DEMI_KENTA = 400,
	TRIBE_DEMI_SUMO, TRIBE_DEMI_YUKI, TRIBE_DEMI_QPD,

	TRIBE_DEMON_MAGA = 500,
	TRIBE_DEMON_SUCCU, TRIBE_DEMON_MATON, TRIBE_DEMON_DRA,

	TRIBE_ANIMAL_WOLF = 600,
	TRIBE_ANIMAL_MYHOME, TRIBE_ANIMAL_NYA, TRIBE_ANIMAL_BEAR,

	TRIBE_BRAVE_VILLAGE = 700,
	TRIBE_BRAVE_BAD, TRIBE_BRAVE_WIZ_BLUE, TRIBE_BRAVE_THIEF,
	TRIBE_BRAVE_BADMASTER, TRIBE_BRAVE_WIZ_RED, TRIBE_BRAVE_BOMGOLE
};

//성별
enum Gender
{
	GENDER_NONE = 0,
	GENDER_MALE, GENDER_FEMALE
};

//AI상태
enum State
{
	STATE_DEAD = -1,

	STATE_NONE = 0,
	STATE_OUT_HOME,
	STATE_GOTO_HOME, STATE_BATTLE, 

	STATE_IN_HOME = 100,
	STATE_GOTO_WORK, STATE_FIND_HOUSE, STATE_KICK,
	STATE_LIKE_ROOM, STATE_BRAVE
};

enum DemageType
{
	DEMAGE_NONE = 0,
	DEMAGE_NORMAL, DEMAGE_MAGIC
};

#define STATE_SIZE 2

#define BULLET_X (1.0f / 6.0f)
#define BULLET_Y (1.0f / 4.0f)

#define ATTACKSPEED_S (1.0f / 6.0f)
#define ATTACKSPEED_A (1.0f / 5.0f)
#define ATTACKSPEED_B (1.0f / 4.0f)
#define ATTACKSPEED_C (1.0f / 3.0f)
#define ATTACKSPEED_D (1.0f / 2.0f)
#define ATTACKSPEED_E (1.0f / 1.0f)

#define MOVESPEED_S (1.0f / 4.0f)
#define MOVESPEED_A (1.0f / 3.4f)
#define MOVESPEED_B (1.0f / 2.8f)
#define MOVESPEED_C (1.0f / 2.2f)
#define MOVESPEED_D (1.0f / 1.6f)
#define MOVESPEED_E (1.0f / 1.0f)

struct House
{
	float doorPosX;
	int step;

	House()
	{
		doorPosX = NULL;
		step = NULL;
	}
};

struct HpInfo
{
	Rect* hp;
	int currentHP, maxHP;

	HpInfo()
	{
		hp = new Rect(D3DXVECTOR2(0.0f, -0.5f), D3DXVECTOR2(1.0f, 0.5f));
		currentHP = 0;
		maxHP = 0;
	}

	~HpInfo()
	{
		SAFE_DELETE(hp);
	}

	void Render()
	{
		hp->Position(-22.0f, 25.0f);
		hp->Scale(D3DXVECTOR2(44.0f, 5.0f));
		hp->Color(1.0f, 0.0f, 0.0f);
		hp->Render();

		float a = (float)currentHP / (float)maxHP;
		hp->Scale(D3DXVECTOR2(44.0f * a, 5.0f));
		hp->Color(0.0f, 1.0f, 0.0f);
		hp->Render();
	}
};

class Character
{
private:
	Image* img;			//캐릭터 이미지
	Rect* colBox;		//캐릭터 충돌박스
	Rect* demageBox;	//공격판정박스
	House* home;		//집 위치(용사일 경우 마왕집)
	Bullet* bullet;		//총알
	HpInfo* hp;			//체력
	int state;			//현재 상태(용사일경우 none)

	int currentFloor;	//현재있는 층
	string name, tribeName;		//이름, 종족 명
	bool pantsRun;		//어른인지 아이인지
	int gender;			//성별
	int tribe;			//종족(사람이면 -1)
	int gold;			//용사 약탈금
	float range;

	DemageType type;			//공격타입(물리, 마법)
	int demage;					//공격력
	int normalDEF, magicDEF;	//물리방어력, 마법방어력
	float aniSpeed;				//애니메이션 스피드
	float attackSpeed, moveSpeed;
	float time, workTime;

	int currentFrameX, currentFrameY;
	D3DXVECTOR2 pos, scale;
	D3DXVECTOR3 rotate;
	D3DXCOLOR color;

public:
	Character(string name, int gender, int tribe);
	~Character();

	void SetFrameY(int y) { currentFrameY = y; }
	void SetPosition(D3DXVECTOR2 pos) { this->pos = pos; }
	void SetCurrentHp(int hp) { this->hp->currentHP = hp; }
	void SetState(State st) { state = (int)st; }
	void SetTime(float t) { time = t; }
	void SetCurrentFloor(int floor) { currentFloor = floor; }
	void SetHome(float posX, int step)
	{
		home->doorPosX = posX;
		home->step = step;
	}
	void Kick()
	{
		SAFE_DELETE(home);
	}

	string GetTribeName() { return tribeName; }
	string GetName() { return name; }
	FRECT GetColboxRect() { return colBox->GetRect(); }
	House& GetHome() { return *home; }
	D3DXVECTOR2 GetPos() { return pos; }
	D3DXVECTOR3 GetRotate() { return rotate; }
	D3DXCOLOR GetColor() { return color; }
	DemageType GetType() { return type; }
	int GetCurrentFloor() { return currentFloor; }
	int GetCurrentHP() { return hp->currentHP; }
	int GetMaxHP() { return hp->maxHP; }
	int GetDemage() { return demage; }
	int GetState() { return state; }
	float GetTime() { return time; }
	int GetNormalDEF() { return normalDEF; }
	int GetMagicDEF() { return magicDEF; }
	int GetGold() { return gold; }

	void Update();
	void Render();
	void Animation();
	void MonsterTribeInfo(int tribe);
	void RandomState();
	void BulletSet();
	void Attack();

	void MoveFloor(int targetFloor, float flootHeight);
	void MovePosX(float goalPosX);
	void MovePos(int targetFloor, float goalPosX, State st);
	void BraveAI();
	void AI();
};
