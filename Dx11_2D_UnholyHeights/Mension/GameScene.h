#pragma once
#include "GameNode.h"
#include "./MensionObject/Apart.h"
#include "./Character/Character.h"

#define SECOEND (int)gameTime % 60
#define MINUTE (int)gameTime / 60 % 60
#define HOUR (int)gameTime / 60 / 60

class GameScene : public GameNode
{
private:
	Image* img;
	Apart* apart;
	Quest* currentQ;
	Button* btn;

	bool changeSun = true;
	bool questRender = false;
	bool createMon = true;
	bool gold = false;

	D3DXMATRIX T;

	float sunMove;
	float waitTime, randomTime;
	int randomSound;
	string soundName;
	int gameSpeed;

	int tribe, isMale;
	D3DXVECTOR2 pos;
public:
	GameScene();
	~GameScene();

	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;
	void EnemyView();
	void CreateMonster();
	void MoneyRender();
	void GameTime();
	void BackColorChange(D3DXCOLOR color);
	void GameSpeedCon();

	void SunAndMoon();
	void Background();
	void RoomSet();
	void SoundControl();
};

