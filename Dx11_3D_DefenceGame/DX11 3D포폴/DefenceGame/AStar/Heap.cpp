#include "Framework.h"

Heap::Heap()
{
}

Heap::~Heap()
{
}

void Heap::Insert(Node* node)
{
	heap.push_back(node);
	UpdateUpper(heap.size() - 1);
}

//처음 노드가 들어올때
void Heap::UpdateUpper(int index)
{
	int curIndex = index;
	int parent = (curIndex - 1) / 2;

	while (curIndex != 0)
	{
		if (heap[parent]->f < heap[curIndex]->f)
			break;

		Swap(curIndex, parent);
		curIndex = parent;
		parent = (curIndex - 1) / 2;
	}
}

//가장 작은 노드가 트리에서 지워질 때
void Heap::UpdateLower(int index)
{
	int curIndex = index;
	int lChild = index * 2 + 1;
	int rChild = lChild + 1;
	int minNode = curIndex;

	while (true)
	{
		if (lChild < heap.size() && heap[lChild]->f < heap[minNode]->f)
			minNode = lChild;

		if (rChild < heap.size() && heap[rChild]->f < heap[minNode]->f)
			minNode = rChild;

		if (minNode == curIndex)
			break;

		Swap(curIndex, minNode);
		curIndex = minNode;
		lChild = curIndex * 2 + 1;
		rChild = lChild + 1;
	}
}

Node* Heap::DeleteRoot()
{
	if (heap.size() <= 0)
		return nullptr;

	Node* root = heap.front();
	Swap(0, heap.size() - 1);
	heap.pop_back();
	UpdateLower(0);

	return root;
}

void Heap::Swap(int n1, int n2)
{
	Node* temp = heap[n1];
	heap[n1] = heap[n2];
	heap[n2] = temp;
}

void Heap::Clear()
{
	heap.clear();
}
