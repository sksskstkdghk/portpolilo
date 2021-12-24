#pragma once
#include "GameNode.h"
#include "RoomInfo.h"
#include "Room.h"

class Apart : public GameNode
{
private:
	Image* img;			//����Ʈ �̹���
	int step;			//����Ʈ �ܲ�
	string ApartName;	//����Ʈ �̸�
	int apartHeight;	//����Ʈ ����

	float roukaScale, roukaPosY, roukaStartPos;
	float apartPosX;

	vector<Room*> room;

	bool isRoomRender;

public:
	Apart();
	~Apart();

	// GameNode��(��) ���� ��ӵ�
	virtual HRESULT init() override;
	virtual void release() override;
	virtual void update() override;
	virtual void render() override;

	bool GetIsRoomRender() { return isRoomRender; }
	void SetApartStep(ApartStep step) { this->step = (int)step; }
	int GetApartStep() { return step; }
	vector<Room*> GetEveryRoom() { return room; }
	bool emptyRoom()
	{
		for (int i = 0; i < (step + 1) * 4; i++)
		{
			if (room.at(i)->GetMonsterName() == "")
				return true;
		}

		return false;
	}

	void RoukaRender();
	void RoomRender();
	void MonthGoldSupply();
	void IsUpgrade();
	void MonRoomClick();

	void ApartCurrentStep();
	void UpgradeApart();
};

