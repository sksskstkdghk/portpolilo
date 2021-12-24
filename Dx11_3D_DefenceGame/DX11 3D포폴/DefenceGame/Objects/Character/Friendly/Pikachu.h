#pragma once

class Pikachu : public Character
{
private:

public:
	Pikachu();

	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

	virtual C_ManagerData Init(UINT lvl, Vector3 pos, vector<Vector3> path) override;
};