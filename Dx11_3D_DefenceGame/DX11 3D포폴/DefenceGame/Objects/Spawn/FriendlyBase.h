#pragma once

class FriendlyBase : public SpawnBase
{
private:
	float time;
	
	BoxCollision* collision;
public:

	FriendlyBase();
	~FriendlyBase();

	// SpawnBase��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void Render() override;
	virtual void CreateCharacter() override;

private:
	void CreateFriendly();

	void SetBattle();
	void EraseSummonData();

};