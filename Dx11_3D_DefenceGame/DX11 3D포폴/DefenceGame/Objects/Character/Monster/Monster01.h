#pragma once

class Monster01 : public Character
{
private:
	vector<Vector3> prevPos;	//길 체크 제한용 이전 위치

public:
	Monster01();

	// Character을(를) 통해 상속됨
	virtual C_ManagerData Init(UINT lvl, Vector3 pos, vector<Vector3> path) override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

private:
	void CheckCrossRoad(UINT index);	//상하좌우 노드가 NONE(유저가 깔아놓은 길)인가?
};