#pragma once

class Character;
enum DemageType;

class Bullet
{
private:
	struct Bullet_Info
	{
		bool isFire;
		D3DXVECTOR2 pos;
		D3DXVECTOR2 dir;

		Bullet_Info()
		{
			isFire = false;
			pos = D3DXVECTOR2(0.0f, 0.0f);
		}
	};

	Rect* col_bullet;
	vector<Bullet_Info*> isFire;
	Image* img_bullet;

	D3DXVECTOR2 start, end;

	float BSpeed;
	int deal;

public:
	Bullet(int bulletSize, int demage, D3DXVECTOR2 uvStart, D3DXVECTOR2 uvEnd);
	~Bullet();

	bool Fire();
	void setBuletPosNDir(D3DXVECTOR2 characterPos, D3DXVECTOR2 characterdir);
	void SetDeal(int demage) { deal = demage; }

	void BulletHit(Character* colTarget, DemageType type);
	void Render();
};

