#pragma once

class Sphere;

enum State
{
	NONE,
	OPEN,
	CLOSED,
	FRIENDLY,
	OBSTACLE,
	CASTLE,
	MONSTER_ROAD,
	MONSTER_OPEN,
	MONSTER_CLOSED,

	MAZE_ROAD,
	MAZE_OPEN,
	MAZE_CLOSED,
	MAZE_OBSTACLE
};

struct EdgeInfo
{
	int index;
	float edgeCost;
};

class Node
{
public:
	friend class AStar;
	friend class Heap;
	friend class MazeAlgorithm;

private:
	//SphereCollision* sphere;	//디버그용 공
	//ColorBuffer* colorBuffer;

	Vector3 pos;//위치
	int index;	//현재 노드에 인덱스
	int via;	//부모노드

	float f, g, h;

	State state;

	Vector2 interval;	//노드 간 간격

	vector<EdgeInfo*> edges;	//주변 노드 인덱스, 거리

public:
	Node(Vector3 pos, int index, Vector2 interval);
	~Node();

	void AddEdge(Node* node);
	BoxCollision* SetObstalce();

	State GetState() { return state; }
	Vector3 GetPos() { return pos; }
	vector<EdgeInfo*> GetEdge() { return edges; }
	int GetIndex() { return index; }

	void SetState(State state) { this->state = state; }

};