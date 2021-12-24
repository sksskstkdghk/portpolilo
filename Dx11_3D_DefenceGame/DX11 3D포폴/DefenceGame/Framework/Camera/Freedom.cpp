#include "Framework.h"

Freedom::Freedom()
	: moveSpeed(30), rotationSpeed(0.005f), wheelSpeed(0.1f)
{
	frustum = new FrustumCulling();
}

Freedom::~Freedom()
{
	delete frustum;
}

void Freedom::Update()
{
	{//Move
		if (mousePos.y >= WIN_HEIGHT - MOVE_RANGE && CAMERA->GetPos().z > -3)
		{
			position -= Vector3(0, 0, 1) * moveSpeed * DELTA;
		}
		if (mousePos.y <= MOVE_RANGE && CAMERA->GetPos().z < (mapHeight / 10) * 7)
		{
			position += Vector3(0, 0, 1) * moveSpeed * DELTA;
		}
		if (mousePos.x <= MOVE_RANGE && CAMERA->GetPos().x > 5)
		{
			position -= Vector3(1, 0, 0) * moveSpeed * DELTA;
		}
		if (mousePos.x >= WIN_WIDTH - MOVE_RANGE && CAMERA->GetPos().x < mapWidth - 5)
		{
			position += Vector3(1, 0, 0) * moveSpeed * DELTA;
		}

		//position += Vector3(0, 1, 0) * wheelValue * wheelSpeed * DELTA;
	}

	//{//Rotation
	//	Vector3 val = mousePos - oldPos;

	//	if (KEYPRESS(VK_RBUTTON))
	//	{
	//		rotation.x += val.y * rotationSpeed;
	//		rotation.y += val.x * rotationSpeed;

	//		Rotation();
	//	}

	//	oldPos = mousePos;
	//}

	//frustum->Make(&(VP->GetView() * VP->GetProjection()));

	View();
}
