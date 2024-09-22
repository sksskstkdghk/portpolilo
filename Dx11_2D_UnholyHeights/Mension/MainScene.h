#pragma once
#include "GameNode.h"

class MainScene : public GameNode
{
	Image* img;
	Button* button;

public:
	MainScene();
	~MainScene();
	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;
};

