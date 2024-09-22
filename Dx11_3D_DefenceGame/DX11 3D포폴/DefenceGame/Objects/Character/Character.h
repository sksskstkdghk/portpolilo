#pragma once

class Character : public AModelAnimator
{
protected:
	enum AnimState
	{
		IDLE,
		RUN,
		ATTACK,
		DEATH
	};

	//아군, 적군 피아 식별용
	enum CharacterDiscrimination
	{
		CHAR_NONE = -1,
		CHAR_FRIENDLY, CHAR_ENEMY
	}charDiscrim;

	enum CharacterState
	{
		
	};

	struct CharData
	{
		UINT instanceID;		//생성된 인스턴스 번호

		int curHP, maxHP;		//현재체력, 최대체력
		int attackDeal;			//공격력
		int defence;			//방어력

		AnimState animState;	//애니메이션 상태

		Vector3 direction;		//방향 길이 계산용
		Vector3 velocity;		//캐릭터가 움직이는 정규화된 방향 값

		Vector3 destPos;		//목적지
		vector<Vector3> path;	//AStar경로

		string enemyName;		//현재 공격중인 적 캐릭터 이름
		UINT enemyInstanceID;	//현재 공격중인 적 캐릭터 ID(데이터 위치)

		bool isRender;			//그릴것인가?(살아있나?)

		bool isAttack;

		CharData()
		{
			destPos = Vector3(0, 0, 0);
			path.clear();
			direction = Vector3(0, 0, 0);
			velocity = Vector3(0, 0, 0);
			enemyName = "";
			enemyInstanceID = 0;
			isAttack = true;
		}
	};

	string name;

	float moveSpeed;
	float range;

	UINT dataCursor;	//사용할 데이터에 위치

	vector<CharData> charData;

public:
	Character();
	~Character();

	virtual C_ManagerData Init(UINT lvl, Vector3 pos, vector<Vector3> path) = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void PostRender() = 0;

	void Attack(UINT instanceID);
	void Dead(UINT instanceID);

	void EnemyPath(UINT instanceID);

protected:
	void SetMinCursor();
	void Exporter(string name);	//모델 없으면 추출용

	void ParentUpdate();
	void ParentRender();
	void ParentPostRender();

	void InstanceRenderDataSet();

	void Move(UINT instanceID);
	void MovePath(UINT instanceID);
	void Rotate(UINT instanceID);
	void ReadyToFight(UINT instanceID, C_ManagerData target);

public:	//set, get 용
	int GetDataSize() { return charData.size(); }
	int GetCharDiscrim() { return (int)charDiscrim; }
	int GetCurrentHP(UINT instanceID) { return charData.at(instanceID).curHP; }
	bool IsAlive(UINT instanceID) { return charData[instanceID].isRender; }

	void Demage(UINT instanceID, int Deal) { charData.at(instanceID).curHP -= (Deal - charData.at(instanceID).defence); }
	void SetBattle(UINT instanceID) { charData.at(instanceID).animState = ATTACK; }
	void SetIDLE(UINT instanceID) { charData.at(instanceID).animState = IDLE; }

	string GetName() { return name; }

	Vector3 GetPos(UINT instanceID) { return transforms[instanceID]->position; }
	UINT GetDataCursor() { return dataCursor; }
};