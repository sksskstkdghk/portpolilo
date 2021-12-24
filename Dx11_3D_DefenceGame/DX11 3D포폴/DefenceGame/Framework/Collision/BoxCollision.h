#pragma once

struct Obb
{
	Vector3 position;

	Vector3 axisX;
	Vector3 axisY;
	Vector3 axisZ;

	Vector3 halfSize;
};

class BoxCollision : public Collision
{
private:
	Vector3 minBox;
	Vector3 maxBox;

	Vertex vertices[8];
public:
	BoxCollision(Vector3 min = Vector3(-0.5f, -0.5f, -0.5f),
		Vector3 max = Vector3(0.5f, 0.5f, 0.5f));
	~BoxCollision();
	
	virtual bool IsCollision(Collision* collision) override;
	virtual bool IsCollision(IN Ray ray, OUT float* distance = nullptr, OUT Vector3* contact = nullptr) override;
	bool IsInstanceCollision(IN Ray ray, int index, OUT float* distance = nullptr, OUT Vector3* contact = nullptr);

	Vector3 GetMin();
	Vector3 GetMax();

private:
	void Create(UINT size);

	bool AABB(BoxCollision* collision);
	bool OBB(BoxCollision* collision);

	Obb GetObb();

	bool SeperateAxis(Vector3 position, Vector3 direction, Obb box1, Obb box2);		
};