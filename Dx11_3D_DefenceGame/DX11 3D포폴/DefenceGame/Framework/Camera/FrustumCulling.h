#pragma once

class FrustumCulling
{
private:
	Vector3 vtx[8];	//���������� ������ ���� 8��
	//Vector3 vPos;	//���� ī�޶��� ������ǥ
	D3DXPLANE plane[6];	//���������� �����ϴ� 6���� ���

public:
	FrustumCulling();
	~FrustumCulling();

	//ī�޶�(view) * ��������(projection)����� �Է¹޾� 6���� ����� �����
	//�ᱹ viewProjection����� �Է¹޾ƾ� �Ѵٴ� ��
	bool Make(Matrix* mat);

	//������ �������Ҿȿ� ������ true�ƴϸ� false�� ��ȯ�Ѵ�
	bool PointCulling(Vector3* point);

	bool Draw();
};

