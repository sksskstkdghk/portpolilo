#include "Framework.h"

RectRoad::RectRoad()
{
	randState = (UINT)RAND_NONE;
	color = Color(0.0f, 0.0f, 0.0f, 1.0f);
}

Color RectRoad::StateByColor()
{
	if (randState == (UINT)RAND_NONE || randState == (UINT)RAND_WALL)
	{
		color = Color(0.0f, 0.0f, 0.0f, 1.0f);
	}
	else if (randState == (UINT)RAND_ROAD)
	{
		color = Color(0.0f, 1.0f, 0.0f, 1.0f);
	}
	else if (randState == (UINT)RAND_MAIN_OFFICE)
	{
		color = Color(1.0f, 1.0f, 0.0f, 1.0f);
	}
	else if (randState == (UINT)RAND_FRIENDLY_BASE)
	{
		color = Color(0.0f, 0.0f, 1.0f, 1.0f);
	}
	else if (randState == (UINT)RAND_ENEMY_BASE)
	{
		color = Color(1.0f, 0.0f, 1.0f, 1.0f);
	}

	return color;
}
