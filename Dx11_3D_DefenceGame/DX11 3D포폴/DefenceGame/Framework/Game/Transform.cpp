#include "Framework.h"

Transform::Transform(string tag)
	: tag(tag), position(0, 0, 0), rotation(0, 0, 0), scale(1, 1, 1),
	pivot(0, 0, 0), parent(nullptr)
{
	D3DXMatrixIdentity(&world);
	worldBuffer = new WorldBuffer();
}

Transform::~Transform()
{
	delete worldBuffer;
}

void Transform::UpdateWorld()
{
	Matrix S, R, T, P, IP;
	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
	D3DXMatrixRotationYawPitchRoll(&R, rotation.y, rotation.x, rotation.z);
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);
	D3DXMatrixTranslation(&P, pivot.x, pivot.y, pivot.z);
	D3DXMatrixInverse(&IP, nullptr, &P);

	world = IP * S * R * T * P;

	if (parent != nullptr)
		world *= *parent;

	D3DXVec3TransformNormal(&forward, &Vector3(0, 0, 1), &world);
	D3DXVec3TransformNormal(&right, &Vector3(1, 0, 0), &world);
	D3DXVec3TransformNormal(&up, &Vector3(0, 1, 0), &world);

	D3DXVec3Normalize(&forward, &forward);
	D3DXVec3Normalize(&right, &right);
	D3DXVec3Normalize(&up, &up);

	worldPosition = Vector3(world._41, world._42, world._43);

	worldBuffer->SetWorld(world);
}

void Transform::SetVS(UINT slot)
{
	worldBuffer->SetVSBuffer(slot);
}
