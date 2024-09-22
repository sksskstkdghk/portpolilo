#pragma once

namespace GameMath
{
	Vector3 Cross(Vector3& vec1, Vector3& vec2);

	float Distance(Vector3& vec1, Vector3& vec2);

	int Random(const int& min,const int& max);
	float Random(const float& min,const float& max);
	Vector3 RandomVec3(const int& min, const int& max);
	Vector2 RandomVec2(const int& min, const int& max);
	bool IntersectRect(Vector3 min, Vector3 max, Vector3 point);

	Vector3 MakeDirection(Vector3 start, Vector3 dest);
}