#pragma once
#include "GameNode.h"

class MainGame :public GameNode
{

public:
	MainGame();
	~MainGame() override;
	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;

	void AddResource();
	void AddQuest();
	void AddSound();

};

