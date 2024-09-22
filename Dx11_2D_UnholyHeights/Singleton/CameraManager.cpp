#include "stdafx.h"
#include "CameraManager.h"


CameraManager::CameraManager()
{
	//Create VpBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VpBufferDesc);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		HRESULT hr = Device->CreateBuffer(&desc, NULL, &VpBuffer);
		assert(SUCCEEDED(hr));

		D3DXMatrixLookAtLH(&VpBufferData.View, &D3DXVECTOR3(g_ptCam.x, g_ptCam.y, -1), &D3DXVECTOR3(g_ptCam.x, g_ptCam.y, 0), &D3DXVECTOR3(0, 1, 0));
		D3DXMatrixTranspose(&VpBufferData.View, &VpBufferData.View);

		D3DXMatrixOrthoOffCenterLH(&VpBufferData.Projection, 0, (float)WINSIZEX, 0, (float)WINSIZEY, -1, 1);
		D3DXMatrixTranspose(&VpBufferData.Projection, &VpBufferData.Projection);
	}
}


CameraManager::~CameraManager()
{
	SAFE_RELEASE(VpBuffer);
}

void CameraManager::SetBuffers()
{
	D3DXMatrixLookAtLH(&VpBufferData.View, &D3DXVECTOR3(g_ptCam.x, g_ptCam.y, -1), 
		&D3DXVECTOR3(g_ptCam.x, g_ptCam.y, 0), &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixTranspose(&VpBufferData.View, &VpBufferData.View);

	DeviceContext->UpdateSubresource(VpBuffer, 0, NULL, &VpBufferData, 0, 0);
	DeviceContext->VSSetConstantBuffers(0, 1, &VpBuffer);
}
