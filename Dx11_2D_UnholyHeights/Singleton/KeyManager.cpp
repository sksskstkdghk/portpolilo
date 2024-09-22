#include "stdafx.h"
#include "KeyManager.h"


KeyManager::KeyManager()
	:prevKey(NULL)
{
	ZeroMemory(KeyDown, sizeof(KeyDown));
}


KeyManager::~KeyManager()
{
}

bool KeyManager::isKeyDown(int KeyCode)
{			//000000		//1000000
			//100000
			//100001
			//000001
	if (GetAsyncKeyState(KeyCode) & 0x8000)
	{
		if (!KeyDown[KeyCode])
		{
			KeyDown[KeyCode] = true;
			return true;
		}
	}
	else KeyDown[KeyCode] = false;

	return false;
}

bool KeyManager::StayKeyDown(int KeyCode)
{
	if (GetAsyncKeyState(KeyCode) & 0x8000)
	{
		prevKey = KeyCode;
		return true;
	}

	return false;
}

bool KeyManager::isKeyUp(int KeyCode)
{
	if (!StayKeyDown(KeyCode))
	{
		if (prevKey == KeyCode)
		{
			prevKey = NULL;
			return true;
		}
	}
	return false;
}

bool KeyManager::ToggleKeyDown(int KeyCode)
{
	if (GetKeyState(KeyCode) & 0x0001)
	{
		return true;
	}
	return false;
}

short KeyManager::MouseWheel()
{
	return wheel;
}
