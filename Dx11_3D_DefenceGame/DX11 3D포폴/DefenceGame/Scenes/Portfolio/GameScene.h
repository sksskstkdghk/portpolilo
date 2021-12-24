#pragma once

class GameScene : public Scene
{
private:
	RenderTarget* renderTarget;
	Render2D* targetTexture;

	Castle* castle;

public:
	GameScene();
	~GameScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

private:
	void Export();
};