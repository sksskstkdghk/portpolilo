#include "Framework.h"
#include "FrustumCulling.h"

FrustumCulling::FrustumCulling()
{
	ZeroMemory(vtx, sizeof(vtx[0]) * 8);
	ZeroMemory(plane, sizeof(plane[0]) * 6);

	cullingSize = 1.0f;
}

FrustumCulling::~FrustumCulling()
{
}

bool FrustumCulling::Make(Matrix* mat)
{
	/*
	프러스텀컬링을 하기위한 평면 6개를 만드는 방법
	1. 프로젝션 행렬까지 거친 월드좌표계의 경계들을 이루는 정점들을 알아온다. 이 정점들의 값은 (-1, -1, 0) ~ (1, 1, 1)사이의 값
	2. 뷰, 프로젝션이 모두 연산된 행렬의 역행렬을 구한다
	3. 구한 역행렬을 아까 구한 정점들과 연산하여 실제 월드에서의 좌표들을 구한다.
	이 좌표들이 실제 절두체의 각 꼭지점이다.
	*/

	int i;
	Matrix matinv;	//뷰, 프로젝션 행렬에 역행렬

	//투영행렬까지 거치고나면 모든 3차원 월드좌표의 점은 (-1, -1, 0) ~ (1, 1, 1)사이의 값으로 바뀐다.
	//vtx에 이 동차공간의 경계값을 넣어둔다
	vtx[0] = Vector3(-1.0f, -1.0f, 0.0f);
	vtx[1] = Vector3(1.0f, -1.0f, 0.0f);
	vtx[2] = Vector3(1.0f, -1.0f, 1.0f);
	vtx[3] = Vector3(-1.0f, -1.0f, 1.0f);
	vtx[4] = Vector3(-1.0f, 1.0f, 0.0f);
	vtx[5] = Vector3(1.0f, 1.0f, 0.0f);
	vtx[6] = Vector3(1.0f, 1.0f, 1.0f);
	vtx[7] = Vector3(-1.0f, 1.0f, 1.0f);

	//역행렬을 구한다
	//행렬끼리 곱한 경우 전치행렬 = 역행렬 공식이 먹히지 않음
	//D3DXMatrixTranspose(&matinv, mat);
	D3DXMatrixInverse(&matinv, nullptr, mat);

	//vertex_최종 = vertex_local * matrix_world * matrix_view * matrix_proj인데
	//vertex_world = vertex_local * matrix_world이므로
	//vertex_최종 = vertex_world * matrix_view * matrix_proj이다
	//vertex_최종 = vertex_world * (matrix_view * matrix_proj)에서
	//view, projection에 역행렬을 양변에 곱하면
	//vertex_최종 * 역행렬 = vertex_world
	//그러므로, vtx * matinv = vertex_world가 되어 월드좌표계의 프러스텀 좌표를 얻을 수 있다.
	for (i = 0; i < 8; i++)
		D3DXVec3TransformCoord(&vtx[i], &vtx[i], &matinv);

	//0번과 5번은 프러스텀중 near평면의 좌측상단과 우측하단이므로, 둘의 좌표를 더해서 2로나누면
	//카메라의 좌표를 얻을 수 있다.(정확히 일치하진 않음)
	D3DXPlaneFromPoints(&plane[0], vtx+4, vtx+7, vtx+6);	//상(top)
	D3DXPlaneFromPoints(&plane[1], vtx,   vtx+1, vtx+2);	//하(bottom)
	D3DXPlaneFromPoints(&plane[2], vtx,   vtx+4, vtx+5);	//근(near)
	D3DXPlaneFromPoints(&plane[3], vtx+2, vtx+6, vtx+7);	//원(far)
	D3DXPlaneFromPoints(&plane[4], vtx,   vtx+3, vtx+7);	//좌(left)
	D3DXPlaneFromPoints(&plane[5], vtx+1, vtx+5, vtx+6);	//우(right)

	return true;
}

bool FrustumCulling::PointCulling(Vector3* point)
{
	float fDist;

	for (D3DXPLANE iter : plane)
	{
		fDist = D3DXPlaneDotCoord(&iter, point);

		if (fDist > cullingSize) return false;
	}

	return true;
}

bool FrustumCulling::Draw()
{
	return true;
}
