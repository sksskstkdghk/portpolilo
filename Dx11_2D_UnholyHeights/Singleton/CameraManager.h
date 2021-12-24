#pragma once
#include "SingletonBase.h"
class CameraManager : public SingletonBase<CameraManager>
{
private:
	struct VpBufferDesc
	{
		D3DXMATRIX View;
		D3DXMATRIX Projection;
	};
	VpBufferDesc VpBufferData;
	ID3D11Buffer* VpBuffer;
public:
	CameraManager();
	~CameraManager();
	void SetBuffers();
};

