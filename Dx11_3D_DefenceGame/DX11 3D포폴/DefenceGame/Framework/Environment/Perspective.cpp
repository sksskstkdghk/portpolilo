#include "Framework.h"

Perspective::Perspective(float width, float height, float fov, float zn, float zf)
{
	D3DXMatrixPerspectiveFovLH(&projection, fov, width / height, zn, zf);
}

Perspective::~Perspective()
{

}
