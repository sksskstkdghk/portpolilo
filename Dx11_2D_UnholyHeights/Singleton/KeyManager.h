#pragma once
#include "SingletonBase.h"

class KeyManager :public SingletonBase<KeyManager>
{
private:
	bool KeyDown[256];
	BYTE prevKey;
	//0~ 255


public:
	KeyManager();
	~KeyManager();
	//눌렀는지 확인
	bool isKeyDown(int KeyCode);
	//계속 눌리고 있는지
	bool StayKeyDown(int KeyCode);
	//버튼을 떼었는지
	bool isKeyUp(int KeyCode);
	//한번 누르면 계속
	bool ToggleKeyDown(int KeyCode);

	short MouseWheel();
};

