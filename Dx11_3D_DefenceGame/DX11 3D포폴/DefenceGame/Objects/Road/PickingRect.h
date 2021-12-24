#pragma once

class PickingRect
{
private:
	typedef Vertex VertexType;

	vector<VertexType> vertices;
public:
	PickingRect(int width, int height);
	~PickingRect();


	bool Picking(Vector3* position);
};

