#pragma once

#define KEYMAX 255

class Keyboard
{
private:
	enum
	{
		NONE,
		DOWN,
		UP,
		PRESS
	};

	byte curState[KEYMAX];
	byte oldState[KEYMAX];
	byte mapState[KEYMAX];

	static Keyboard* instance;

	Keyboard();
	~Keyboard();

public:
	static Keyboard* Get() { return instance; }
	static void Create() { instance = new Keyboard(); }
	static void Delete() { delete instance; }

	void Update();

	bool Down(UINT key) { return mapState[key] == DOWN; }
	bool Up(UINT key) { return mapState[key] == UP; }
	bool Press(UINT key) { return mapState[key] == PRESS; }
};