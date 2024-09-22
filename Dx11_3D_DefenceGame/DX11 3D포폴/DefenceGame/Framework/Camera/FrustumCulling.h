#pragma once

class FrustumCulling
{
private:
	Vector3 vtx[8];	//프러스텀을 구성할 정점 8개
	//Vector3 vPos;	//현재 카메라의 월드좌표
	D3DXPLANE plane[6];	//프러스텀을 구성하는 6개의 평면

public:
	FrustumCulling();
	~FrustumCulling();

	//카메라(view) * 프로젝션(projection)행렬을 입력받아 6개의 평면을 만든다
	//결국 viewProjection행렬을 입력받아야 한다는 말
	bool Make(Matrix* mat);

	//한점이 프러스텀안에 있으면 true아니면 false를 반환한다
	bool PointCulling(Vector3* point);

	bool Draw();
};

