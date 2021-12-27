#include "Framework.h"

AStar* AStar::instance = nullptr;

AStar::AStar(UINT width, UINT height)
	: width(width), height(height)
{
	heap = new Heap();
	SetUp(TERRAIN);
}

AStar::~AStar()
{
	for (Node* node : nodes)
		delete node;

	delete heap;

	delete obstacles;
}

//�� �ʿ� ũ�⿡ ���� ��带 ��ġ
void AStar::SetUp(TerrainEditor* terrain)
{
	Vector2 size = terrain->GetSize();

	Vector2 interval;
	interval.x = size.x / width;
	interval.y = size.y / height;

	sphere = new SphereCollision(0.1f);
	obstacles = new BoxCollision();

	//��带 �����ϰ� �ʿ� ����(interval)�� �ΰ� ��ġ��
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			Vector3 pos = Vector3((x * interval.x) + (interval.x * 0.5f), 0, z * interval.y + (interval.y * 0.5f));	//��� ��ġ

			int index = z * width + x;
			Node* node = new Node(pos, index, interval);
			node->SetState(OBSTACLE);

			//��ֹ� ����
			obstacles->GetTransform(index)->position = pos + Vector3(0.0f, 0.5f, 0.0f);
			sphere->GetTransform(index)->position = pos + Vector3(0.0f, 0.5f, 0.0f);

			nodes.push_back(node);
		}
	}

	for (UINT i = 0; i < nodes.size(); i++)
	{
		//�¿� ���
		if (i % width != width - 1)	//������ �� ��尡 �ƴ϶��
		{
			nodes[i + 0]->AddEdge(nodes[i + 1]);	//(i + 0)��° ��忡 ������ ��������� ����
			nodes[i + 1]->AddEdge(nodes[i + 0]);	//(i + 1)��° ��忡 ���� ��������� ����
		}

		//���� ���
		if (i < nodes.size() - width)	//���� ���� ��尡 �ƴ϶��
		{
			nodes[i + 0]->AddEdge(nodes[i + width]);	//(i + 0)��° ��忡 ���� ��������� ����
			nodes[i + width]->AddEdge(nodes[i + 0]);	//(i + width)��° ��忡 �Ʒ��� ��������� ����
		}
	}

	obstacles->UpdateTransforms();
	sphere->UpdateTransforms();

	sphere->UpdateBuffer();
	obstacles->UpdateBuffer();
}

void AStar::Update()
{
	for (Node* node : nodes)
	{
		State temp = node->GetState();

		if(temp == NONE || temp == FRIENDLY)
			sphere->SetColor(node->GetIndex(), Color(1.0f, 1.0f, 1.0f, 1.0f));
		else if(temp == OPEN || temp == MONSTER_OPEN || temp == MAZE_OPEN)
			sphere->SetColor(node->GetIndex(), Color(0.0f, 0.0f, 1.0f, 1.0f));
		else if(temp == CLOSED || temp == MONSTER_CLOSED || temp == MAZE_CLOSED)
			sphere->SetColor(node->GetIndex(), Color(1.0f, 0.0f, 0.0f, 1.0f));
		else
			sphere->SetColor(node->GetIndex(), Color(0.0f, 0.0f, 0.0f, 1.0f));

		if(temp == MONSTER_ROAD)
			sphere->SetColor(node->GetIndex(), Color(0.0f, 1.0f, 0.0f, 1.0f));

		if(temp == MAZE_ROAD)
			sphere->SetColor(node->GetIndex(), Color(1.0f, 1.0f, 0.0f, 1.0f));

		if (node->GetState() != OBSTACLE)
			obstacles->SetIsRender(node->index, false);
	}
}

void AStar::Render()
{
	int sphereCount = 0;
	int obsCount = 0;

	for (int i =0; i<nodes.size(); i++)
	{
		if (CAMERA->GetCulling()->PointCulling(&nodes[i]->pos))
		{
			sphere->SetRenderData(sphereCount, i);
			sphereCount++;

			if (obstacles->GetIsRender(i))
			{
				obstacles->SetRenderData(obsCount, i);
				obsCount++;
			}
		}
	}

	sphere->UpdateBuffer();
	obstacles->UpdateBuffer();

	//��ֹ�(��)
	obstacles->InstancingRender(obsCount);
	sphere->InstancingRender(sphereCount);
}

vector<Vector3> AStar::FindPath(int start, int end)
{
	vector<Vector3> path;
	path.clear();

	float G = 0.0f;
	float H = GetManhattanDistance(start, end);

	nodes[start]->g = G;
	nodes[start]->h = H;
	nodes[start]->f = G + H;
	nodes[start]->via = start;

	int temp = (int)nodes[start]->state;
	nodes[start]->state = (State)(temp + 1);

	heap->Insert(nodes[start]);

	int curIndex = 0;

	Reset();

	while (true)
	{
		curIndex = GetMinNode();

		if (curIndex < 0)
			return path;

		//������ �ֺ� Ž�� �� ����ġ ����
		Extend(curIndex, end);

		//��� ���¿����� ���� ����
		if(nodes[curIndex]->state < OBSTACLE)
			nodes[curIndex]->state = CLOSED;
		else if(nodes[curIndex]->state >= CASTLE &&
				nodes[curIndex]->state < MAZE_ROAD)
			nodes[curIndex]->state = MONSTER_CLOSED;
		else if(nodes[curIndex]->state >= MAZE_ROAD)
			nodes[curIndex]->state = MAZE_CLOSED;

		//������ ������ break
		if (curIndex == end)
			break;
	}

	//������ �θ��带 �����ϸ� �������
	while (curIndex != start)
	{
		path.push_back(nodes[curIndex]->pos);
		curIndex = nodes[curIndex]->via;
	}

	//������ �������� ������� �� heapŬ����
	path.push_back(nodes[curIndex]->pos);
	heap->Clear();

	Reset();
	return path;
}

vector<Vector3> AStar::FindCloseFBasePath(int start)
{
	vector<Vector3> closePath;
	closePath.clear();

	Reset();

	for (UINT i = 0; i < SPAWN->GetFBaseSize(); i++)
	{
		if (SPAWN->GetFBase(i)->GetCharacterCount() <= 0)
			continue;

		bool check = true;

		vector<Vector3> path;
		path.clear();

		int end = FindCloseNode(SPAWN->GetFBase(i)->GetPos());
		float G = 0.0f;
		float H = GetManhattanDistance(start, end);

		nodes[start]->g = G;
		nodes[start]->h = H;
		nodes[start]->f = G + H;
		nodes[start]->via = start;
		nodes[start]->state = OPEN;
		heap->Insert(nodes[start]);

		while (true)
		{
			int curIndex = GetMinNode();

			if (curIndex < 0)
			{
				check = false;
				break;
			}

			CloseFBaseExtend(curIndex, end);
			nodes[curIndex]->state = CLOSED;
			
			if (curIndex == end)
				break;
		}

		if (!check)
			continue;

		int curIndex = end;
		while (curIndex != start)
		{
			path.push_back(nodes[curIndex]->pos);
			curIndex = nodes[curIndex]->via;
		}

		path.push_back(nodes[curIndex]->pos);
		heap->Clear();

		if (closePath.size() <= 0)
			closePath = path;
		else
		{
			if (closePath.size() > path.size())
			{
				closePath.clear();
				closePath = path;
			}
		}
	}

	Reset();
	return closePath;
}

int AStar::FindCloseNode(Vector3 pos, bool cantObs)
{
	float minDistance = FLT_MAX;
	int index = -1;

	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i]->state == OBSTACLE && cantObs == true)
			continue;

		float distance = Distance(pos, nodes[i]->pos);

		if (distance < minDistance)
		{
			minDistance = distance;
			index = i;
		}
	}

	return index;
}

void AStar::MakeDirectPath(IN Vector3 start, IN Vector3 end, OUT vector<Vector3>& path)
{
	int cutNodeNum = 0;
	Ray ray;
	ray.position = start;

	for (int i = 0; i < path.size(); i++)
	{
		ray.direction = path[i] - ray.position;
		float distance = D3DXVec3Length(&ray.direction);
		D3DXVec3Normalize(&ray.direction, &ray.direction);

		if (!isCollisionObstalce(ray, distance))
		{
			cutNodeNum = path.size() - i - 1;
			break;
		}
	}

	for (int i = 0; i < cutNodeNum; i++)
	{
		path.pop_back();
	}

	cutNodeNum = 0;
	ray.position = end;

	for (int i = 0; i < path.size(); i++)
	{
		ray.direction = path[path.size() - i - 1] - ray.position;
		float distance = D3DXVec3Length(&ray.direction);
		D3DXVec3Normalize(&ray.direction, &ray.direction);

		if (!isCollisionObstalce(ray, distance))
		{
			cutNodeNum = path.size() - i - 1;
			break;
		}
	}

	for (int i = 0; i < cutNodeNum; i++)
		path.erase(path.begin());
}

bool AStar::isWorkObstalce(Vector3 pickingPos)
{
	UINT index = (UINT)pickingPos.x + (UINT)pickingPos.z * width;

	if (index >= width * height)
		return true;

	if (nodes[index]->GetState() == OBSTACLE)
		return true;

	return false;
}

bool AStar::isCollisionObstalce(Ray ray, float destDistance)
{
	for (UINT i = 0 ; i < mapWidth * mapHeight; i++)
	{
		if (obstacles->IsInstanceCollision(ray, i) &&
			nodes[i]->GetState() == OBSTACLE)
		{
			float distance = Distance(ray.position, obstacles->GetTransform(i)->position);

			if (distance < destDistance)
				return true;
		}
	}

	return false;
}

void AStar::Reset()
{
	for (Node* node : nodes)
	{
		if ((UINT)node->state < (UINT)FRIENDLY)
			node->state = NONE;
		else if ((UINT)node->state > (UINT)MONSTER_ROAD &&
			(UINT)node->state < (UINT)MAZE_ROAD)
			node->state = MONSTER_ROAD;
	}

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
		{
			UINT index = mapWidth / 2 + (mapHeight / 2 - i) * mapWidth - j;
			ASTAR->GetNodes().at(index)->SetState(CASTLE);
		}

	//�ٸ������ �𸣰ڴ�...
	for (int i = 0; i < SPAWN->GetFBaseSize(); i++)
	{
		Vector3 temp = SPAWN->GetFBase(i)->GetPos();

		int nodeIndex = (UINT)temp.x + (UINT)temp.z * mapWidth;
		nodes[nodeIndex];
		
		for(int x = -1; x < 2; x++)
			for (int z = -1; z < 2; z++)
			{
				nodes[nodeIndex + x + (z * mapWidth)]->SetState(FRIENDLY);
			}
	}
}

float AStar::GetManhattanDistance(int curIndex, int end)
{
	Vector3 curpos = nodes[curIndex]->pos;
	Vector3 endpos = nodes[end]->pos;

	Vector3 temp = curpos - endpos;

	return abs(temp.x) + abs(temp.y) + abs(temp.z);
}

//�� �Ⱦ�
float AStar::GetDistance(int curIndex, int end)
{
	return Distance(nodes[curIndex]->pos, nodes[end]->pos);
}

//�Ű����� (���� ���, �� ���)
void AStar::Extend(int center, int end)
{
	vector<EdgeInfo*> edges = nodes[center]->edges;

	for (int i = 0; i < edges.size(); i++)
	{
		//���� ��忡 �ֺ� ��� �ε���
		int index = edges[i]->index;

		//���� �ִ� ������� Ȯ��
		if (nodes[index]->state == CLOSED || nodes[index]->state == OBSTACLE ||
			nodes[index]->state == MONSTER_CLOSED || nodes[index]->state == MAZE_OBSTACLE ||
			nodes[index]->state == MAZE_CLOSED)
			continue;

		//�ֺ� ��� �� ����
		float G = nodes[center]->g + edges[i]->edgeCost;
		float H = GetManhattanDistance(index, end);
		float F = G + H;

		//�̹� Ž���� �� �����
		if (nodes[index]->state == OPEN || nodes[index]->state == MONSTER_OPEN ||
			nodes[index]->state == MAZE_OPEN)
		{
			//�ֺ� ��忡 ���� ���� ��忡 �� ���� �۴ٸ� ����
			if (F < nodes[index]->f)
			{
				nodes[index]->g = G;
				nodes[index]->f = F;
				nodes[index]->via = center;
			}
		}
		//ó�� Ž���ϴ� �����
		else if (nodes[index]->state == MONSTER_ROAD || nodes[index]->state == CASTLE ||
				 nodes[index]->state == NONE || nodes[index]->state == FRIENDLY || nodes[index]->state == MAZE_ROAD)
		{
			//����ġ ���� �� �θ��� ����
			nodes[index]->g = G;
			nodes[index]->f = F;
			nodes[index]->h = H;
			nodes[index]->via = center;

			//open���� ����
			if(nodes[index]->state == MONSTER_ROAD)
				nodes[index]->state = MONSTER_OPEN;
			else if(nodes[index]->state == NONE)
				nodes[index]->state = OPEN;
			else if(nodes[index]->state == MAZE_ROAD)
				nodes[index]->state = MAZE_OPEN;

			//����
			heap->Insert(nodes[index]);
		}
	}
}

void AStar::CloseFBaseExtend(int center, int end)
{
	vector<EdgeInfo*> edges = nodes[center]->edges;

	for (int i = 0; i < edges.size(); i++)
	{
		int index = edges[i]->index;

		//���� �ִ� ������� Ȯ��
		if (nodes[index]->state != NONE &&
			nodes[index]->state != FRIENDLY &&
			nodes[index]->state != OPEN)
			continue;

		float G = nodes[center]->g + edges[i]->edgeCost;
		float H = GetManhattanDistance(index, end);
		float F = G + H;

		if (nodes[index]->state == OPEN)
		{
			if (F < nodes[index]->f)
			{
				nodes[index]->g = G;
				nodes[index]->f = F;
				nodes[index]->via = center;
			}
		}
		else if (nodes[index]->state == NONE || nodes[index]->state == FRIENDLY)
		{
			nodes[index]->g = G;
			nodes[index]->f = F;
			nodes[index]->h = H;
			nodes[index]->via = center;
			nodes[index]->state = OPEN;
			heap->Insert(nodes[index]);
		}
	}
}

int AStar::GetMinNode()
{
	Node* node = heap->DeleteRoot();

	if (node == nullptr)
		return -1;

	return node->index;
}

