#pragma once

class FriendlyBase : public SpawnBase
{
private:
	float time;
	
	BoxCollision* collision;
public:

	FriendlyBase();
	~FriendlyBase();

	// SpawnBase을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render() override;
	virtual void CreateCharacter() override;

private:
	void CreateFriendly();

	void SetBattle();
	void EraseSummonData();

};