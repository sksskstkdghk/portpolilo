#pragma once

class SphereCollision : public Collision
{
private:
	float radius;

	UINT stackCount;
	UINT sliceCount;

public:
	SphereCollision(float radius = 1.0f, UINT stackCount = 20, UINT sliceCount = 20);
	~SphereCollision();

	// Collision을(를) 통해 상속됨
	virtual bool IsCollision(Collision* collision) override;
	virtual bool IsCollision(IN Ray ray, OUT float* distance = nullptr, OUT Vector3* contact = nullptr) override;

	Transform* GetTransform(int index) { return transforms[index]; }

	

private:
	void Create(UINT size);

	bool IsCollision(SphereCollision* collision);
};