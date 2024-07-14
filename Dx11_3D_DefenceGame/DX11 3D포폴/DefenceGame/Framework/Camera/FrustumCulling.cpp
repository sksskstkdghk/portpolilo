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
	���������ø��� �ϱ����� ��� 6���� ����� ���
	1. �������� ��ı��� ��ģ ������ǥ���� ������ �̷�� �������� �˾ƿ´�. �� �������� ���� (-1, -1, 0) ~ (1, 1, 1)������ ��
	2. ��, ���������� ��� ����� ����� ������� ���Ѵ�
	3. ���� ������� �Ʊ� ���� ������� �����Ͽ� ���� ���忡���� ��ǥ���� ���Ѵ�.
	�� ��ǥ���� ���� ����ü�� �� �������̴�.
	*/

	int i;
	Matrix matinv;	//��, �������� ��Ŀ� �����

	//������ı��� ��ġ���� ��� 3���� ������ǥ�� ���� (-1, -1, 0) ~ (1, 1, 1)������ ������ �ٲ��.
	//vtx�� �� ���������� ��谪�� �־�д�
	vtx[0] = Vector3(-1.0f, -1.0f, 0.0f);
	vtx[1] = Vector3(1.0f, -1.0f, 0.0f);
	vtx[2] = Vector3(1.0f, -1.0f, 1.0f);
	vtx[3] = Vector3(-1.0f, -1.0f, 1.0f);
	vtx[4] = Vector3(-1.0f, 1.0f, 0.0f);
	vtx[5] = Vector3(1.0f, 1.0f, 0.0f);
	vtx[6] = Vector3(1.0f, 1.0f, 1.0f);
	vtx[7] = Vector3(-1.0f, 1.0f, 1.0f);

	//������� ���Ѵ�
	//��ĳ��� ���� ��� ��ġ��� = ����� ������ ������ ����
	//D3DXMatrixTranspose(&matinv, mat);
	D3DXMatrixInverse(&matinv, nullptr, mat);

	//vertex_���� = vertex_local * matrix_world * matrix_view * matrix_proj�ε�
	//vertex_world = vertex_local * matrix_world�̹Ƿ�
	//vertex_���� = vertex_world * matrix_view * matrix_proj�̴�
	//vertex_���� = vertex_world * (matrix_view * matrix_proj)����
	//view, projection�� ������� �纯�� ���ϸ�
	//vertex_���� * ����� = vertex_world
	//�׷��Ƿ�, vtx * matinv = vertex_world�� �Ǿ� ������ǥ���� �������� ��ǥ�� ���� �� �ִ�.
	for (i = 0; i < 8; i++)
		D3DXVec3TransformCoord(&vtx[i], &vtx[i], &matinv);

	//0���� 5���� ���������� near����� ������ܰ� �����ϴ��̹Ƿ�, ���� ��ǥ�� ���ؼ� 2�γ�����
	//ī�޶��� ��ǥ�� ���� �� �ִ�.(��Ȯ�� ��ġ���� ����)
	D3DXPlaneFromPoints(&plane[0], vtx+4, vtx+7, vtx+6);	//��(top)
	D3DXPlaneFromPoints(&plane[1], vtx,   vtx+1, vtx+2);	//��(bottom)
	D3DXPlaneFromPoints(&plane[2], vtx,   vtx+4, vtx+5);	//��(near)
	D3DXPlaneFromPoints(&plane[3], vtx+2, vtx+6, vtx+7);	//��(far)
	D3DXPlaneFromPoints(&plane[4], vtx,   vtx+3, vtx+7);	//��(left)
	D3DXPlaneFromPoints(&plane[5], vtx+1, vtx+5, vtx+6);	//��(right)

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
