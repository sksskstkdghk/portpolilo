#include "Framework.h"

Vector3 GameMath::Cross(Vector3& vec1, Vector3& vec2)
{
	float x = vec1.y * vec2.z - vec1.z * vec2.y;
	float y = vec1.z * vec2.x - vec1.x * vec2.z;
	float z = vec1.x * vec2.y - vec1.y * vec2.x;

	return Vector3(x, y, z);
}

float GameMath::Distance(Vector3& vec1, Vector3& vec2)
{
	return D3DXVec3Length(&(vec2 - vec1));
}

int GameMath::Random(const int& min,const int& max)
{
	return rand() % (max - min) + min;
}

float GameMath::Random(const float& min,const float& max)
{
	float normal = rand() / (float)RAND_MAX;

	return min + (max - min) * normal;
}

Vector3 GameMath::RandomVec3(const int& min, const int& max)
{
	Vector3 result;
	result.x = Random(min, max);
	result.y = Random(min, max);
	result.z = Random(min, max);

	return result;
}

Vector2 GameMath::RandomVec2(const int& min, const int& max)
{
	Vector2 result;
	result.x = Random(min, max);
	result.y = Random(min, max);

	return result;
}

bool GameMath::IntersectRect(Vector3 min, Vector3 max, Vector3 point)
{
	if (point.x < max.x &&	//왼쪽
		point.x > min.x &&	//오른쪽
		point.z < max.z &&	//아래
		point.z > min.z)	//위
		return true;

	return false;
}

Vector3 GameMath::MakeDirection(Vector3 start, Vector3 dest)
{
	Vector3 data = dest - start;
	D3DXVec3Normalize(&data, &data);

	return data;
}
