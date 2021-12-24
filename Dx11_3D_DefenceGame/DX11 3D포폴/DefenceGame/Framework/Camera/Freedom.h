#pragma once

class Freedom : public Camera
{
private:
#define MOVE_RANGE 20

	float moveSpeed;
	float rotationSpeed;
	float wheelSpeed;
	
public:
	Freedom();
	~Freedom();

	void Update() override;
};