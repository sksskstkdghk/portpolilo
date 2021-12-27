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

	//�Ʊ�, ���� �Ǿ� �ĺ���
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
		UINT instanceID;		//������ �ν��Ͻ� ��ȣ

		int curHP, maxHP;		//����ü��, �ִ�ü��
		int attackDeal;			//���ݷ�
		int defence;			//����

		AnimState animState;	//�ִϸ��̼� ����

		Vector3 direction;		//���� ���� ����
		Vector3 velocity;		//ĳ���Ͱ� �����̴� ����ȭ�� ���� ��

		Vector3 destPos;		//������
		vector<Vector3> path;	//AStar���

		string enemyName;		//���� �������� �� ĳ���� �̸�
		UINT enemyInstanceID;	//���� �������� �� ĳ���� ID(������ ��ġ)

		bool isRender;			//�׸����ΰ�?(����ֳ�?)

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

	UINT dataCursor;	//����� �����Ϳ� ��ġ

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
	void Exporter(string name);	//�� ������ �����

	void ParentUpdate();
	void ParentRender();
	void ParentPostRender();

	void InstanceRenderDataSet();

	void Move(UINT instanceID);
	void MovePath(UINT instanceID);
	void Rotate(UINT instanceID);
	void ReadyToFight(UINT instanceID, C_ManagerData target);

public:	//set, get ��
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