#include "stdafx.h"
#include "Bullet.h"
#include "./Character/Character.h"


Bullet::Bullet(int bulletSize, int demage, D3DXVECTOR2 uvStart, D3DXVECTOR2 uvEnd)
{
	img_bullet = IMGMANAGER->GetImage("bullet");
	img_bullet->Position(-100.0f, -200.0f);
	img_bullet->Scale(50.0f, 50.0f);

	col_bullet = new Rect();
	col_bullet->Scale(35.0f, 35.0f);
	col_bullet->SetParent(img_bullet->GetRT());
	
	for (int i = 0; i < bulletSize; i++)
		isFire.push_back(new Bullet_Info());

	BSpeed = 150.0f;

	deal = demage;

	start = uvStart;
	end = uvEnd;
}


Bullet::~Bullet()
{
	SAFE_DELETE(col_bullet);

	isFire.clear();
	isFire.shrink_to_fit();
}

bool Bullet::Fire()
{
	for (int i = 0; i < isFire.size(); i++)
	{
		if (isFire.at(i)->isFire == false)
		{
			isFire.at(i)->isFire = true;

			return true;
		}
	}

	return false;
}

void Bullet::setBuletPosNDir(D3DXVECTOR2 characterPos, D3DXVECTOR2 characterdir)
{
	for (int i = 0; i < isFire.size(); i++)
	{
		if (isFire.at(i)->isFire == true &&
			isFire.at(i)->pos == D3DXVECTOR2(0.0f, 0.0f))
		{
			isFire.at(i)->pos = characterPos;
			isFire.at(i)->dir = characterdir;

			break;
		}
	}
}

void Bullet::BulletHit(Character* colTarget, DemageType type)
{
	for (int i = 0; i < isFire.size(); i++)
	{
		if (isFire.at(i)->isFire == true)
		{
			FRECT col = RectMakeCenter(isFire.at(i)->pos.x, isFire.at(i)->pos.y,
				col_bullet->GetScale().x, col_bullet->GetScale().y);

			if (IntersectRect(col, colTarget->GetColboxRect()))
			{
				isFire.at(i)->isFire = false;
				isFire.at(i)->pos = D3DXVECTOR2(0.0f, 0.0f);
				isFire.at(i)->dir = D3DXVECTOR2(0.0f, 0.0f);

				int a;

				if (type == DEMAGE_NORMAL)
				{
					a = deal - colTarget->GetNormalDEF();
				}
				else if (type == DEMAGE_MAGIC)
				{
					a = deal - colTarget->GetMagicDEF();
				}

				if (a <= 0)
					a = 1;

				colTarget->SetCurrentHp(colTarget->GetCurrentHP() - a);
			}
		}
	}
}

void Bullet::Render()
{
	for (int i = 0; i < isFire.size(); i++)
	{
		if (isFire.at(i)->isFire == true)
		{
			isFire.at(i)->pos += BSpeed * g_ETime * isFire.at(i)->dir;
			img_bullet->Position(isFire.at(i)->pos);
			
			//col_bullet->Render();
			col_bullet->Update();
			img_bullet->Render(start.x, start.y, end.x, end.y);
		}

		if (isFire.at(i)->pos.x < g_ptCam.x - 10.0f || isFire.at(i)->pos.x > g_ptCam.x + WINSIZEX + 10.0f)
		{
			isFire.at(i)->isFire = false;
			isFire.at(i)->pos = D3DXVECTOR2(0.0f, 0.0f);
			isFire.at(i)->dir = D3DXVECTOR2(0.0f, 0.0f);
		}
	}
}
