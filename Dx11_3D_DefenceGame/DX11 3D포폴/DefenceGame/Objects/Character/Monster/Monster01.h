#pragma once

class Monster01 : public Character
{
private:
	vector<Vector3> prevPos;	//�� üũ ���ѿ� ���� ��ġ

public:
	Monster01();

	// Character��(��) ���� ��ӵ�
	virtual C_ManagerData Init(UINT lvl, Vector3 pos, vector<Vector3> path) override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

private:
	void CheckCrossRoad(UINT index);	//�����¿� ��尡 NONE(������ ��Ƴ��� ��)�ΰ�?
};