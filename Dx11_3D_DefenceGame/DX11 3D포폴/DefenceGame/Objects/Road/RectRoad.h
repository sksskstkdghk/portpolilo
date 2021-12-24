#pragma once

enum RectRandState
{
	RAND_NONE = 0,
	RAND_ROAD, RAND_WALL,
	RAND_MAIN_OFFICE, RAND_FRIENDLY_BASE, RAND_ENEMY_BASE
};

//피킹시 사각형 단위로 사용하기 위한 클래스
class RectRoad
{
private:
	typedef Vertex VertexType;

	UINT randState;
	Color color;
public:
	RectRoad();
	~RectRoad() {}

	UINT GetRandState() { return randState; }

	void SetColor(Color color) { this->color = color; }
	void SetState(UINT state) { randState = state; }

	Color StateByColor();
};