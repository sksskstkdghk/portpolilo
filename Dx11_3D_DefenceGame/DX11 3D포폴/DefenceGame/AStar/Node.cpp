#include "Framework.h"

Node::Node(Vector3 pos, int index, Vector2 interval)
	: pos(pos), index(index), via(-1), f(0), g(0), h(0), state(NONE),
	interval(interval)
{
	//sphere = new SphereCollision(0.1f);
	//sphere->position = pos;
}

Node::~Node()
{
	//delete sphere;
}

void Node::AddEdge(Node* node)
{
	EdgeInfo* edge = new EdgeInfo();
	edge->index = node->index;					//ÁÂ¿ì¾ç¿· ³ëµå ÀÎµ¦½º(¹øÈ£)
	edge->edgeCost = Distance(pos, node->pos);	//ÁÂ¿ì¾ç¿· ³ëµå°£ °Å¸®

	edges.push_back(edge);
}

BoxCollision* Node::SetObstalce()
{
	state = OBSTACLE;

	Vector3 min = { -interval.x * 0.5f, -1.0f, -interval.y * 0.5f };
	Vector3 max = min * -1.0f;
	max.y = 1.0f;

	BoxCollision* box = new BoxCollision(min, max);
	box->position = pos;

	return box;
}
