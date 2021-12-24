#pragma once

class RectCollision : public Collision
{
private:
	

public:
	//RectCollision

	// Collision을(를) 통해 상속됨
	virtual bool IsCollision(Collision* collision) override;

	virtual bool IsCollision(IN Ray ray, OUT float* distance = nullptr, OUT Vector3* contact = nullptr) override;

};