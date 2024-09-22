#include "Framework.h"

Camera::Camera()
	: position(0, 0, 0), rotation(0, 0), forward(0, 0, 1), right(1, 0, 0), up(0, 1, 0)
{
	oldPos = mousePos;
	D3DXMatrixIdentity(&matRotation);
	D3DXMatrixIdentity(&matView);

	Move();
	Rotation();
}

Camera::~Camera()
{
}

void Camera::Update()
{
}

void Camera::Move()
{
	View();
}

void Camera::Rotation()
{
	Matrix rotX, rotY;
	D3DXMatrixRotationX(&rotX, rotation.x);
	D3DXMatrixRotationY(&rotY, rotation.y);

	matRotation = rotX * rotY;

	D3DXVec3TransformNormal(&forward, &Vector3(0, 0, 1), &matRotation);
	D3DXVec3TransformNormal(&right, &Vector3(1, 0, 0), &matRotation);
	D3DXVec3TransformNormal(&up, &Vector3(0, 1, 0), &matRotation);
}

void Camera::View()
{
	D3DXMatrixLookAtLH(&matView, &position, &(position + forward), &up);
}

Ray Camera::GetPickingRay()
{
	//현재 프로그램 화면 크기
	Vector2 screenSize = Vector2(WIN_WIDTH, WIN_HEIGHT);

	Vector2 point;

	point.x = (2 * mousePos.x / screenSize.x) - 1.0f;			//(왼쪽~오른쪽)-1.0f~1.0f
	point.y = ((2 * mousePos.y / screenSize.y) - 1.0f) * -1.0f;	//(아래~위)-1.0f~1.0f

	Matrix projection = VP->GetProjection();

	point.x /= projection._11;	//투영행렬의 [1][1]값은 x값의 변환에 영향을 줌
	point.y /= projection._22;	//투영행렬의 [2][2]값은 y값의 변환에 영향을 줌

	Matrix invView;
	D3DXMatrixTranspose(&invView, &matView);

	Ray ray;
	ray.position = position;

	D3DXVec3TransformNormal(&ray.direction, &Vector3(point.x, point.y, 1), &invView);

	return ray;
}

void Camera::SetRotation(float x, float y)
{
	rotation = Vector2(x, y);

	Rotation();
}
