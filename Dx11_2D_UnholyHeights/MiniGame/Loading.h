#pragma once
#include "GameNode.h"

//멀티 스레딩시 필요한 stl
#include <thread>
#include <mutex>

static mutex mtx;

class Loading : public GameNode
{
public:
	Loading();
	~Loading()override;
	HRESULT init() override;
	void release() override;
	void update() override;
	void render() override;

	static int LoadingNum;


	static void func1()
	{
		mtx.lock();
		D3DXCOLOR background = D3DXCOLOR(1, 1, 1, 1);
		CAMMANAGER->SetBuffers();
		DeviceContext->ClearRenderTargetView(RTV, (float*)background);
		//////////////////
		//Sleep(100);
		/////////////
		WRITEMANAGER->GetDc()->BeginDraw();
		Image* img;
		img = IMGMANAGER->GetImage("title_back");
		img->Scale(WINSIZEX, WINSIZEY);
		img->Position(WINSIZEX / 2.0f, WINSIZEY / 2.0f);
		img->Render();

		img = IMGMANAGER->GetImage("title");
		img->Scale(WINSIZEX, WINSIZEY);
		img->Position(WINSIZEX / 2.0f, WINSIZEY / 2.0f);
		img->Render();

		RECT frc = { WINSIZEX / 2 - 30 , WINSIZEY / 2, WINSIZEX,  WINSIZEY };
		WRITEMANAGER->RenderText(L"Loading", frc, 30.0f, L"돋움체", D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), DWRITE_FONT_WEIGHT_NORMAL);

		WRITEMANAGER->GetDc()->EndDraw();
		SwapChain->Present(0, 0);

		mtx.unlock();
		//모든 스레드 종료되면
		SCENEMANAGER->ChangeScene("Game");
	}

	static void func2(int a)
	{
		for (int i = 0; i < a; i++)
		{
			cout << "스레드 2 작동중" << endl;
		}
	}
	static void func3(int a,int b)
	{
		for (int i = 0; i < a; i++)
		{
			Rect* rc = new Rect();
			cout << b<<"스레드에서 생성" << endl;
			SAFE_DELETE(rc);
			LoadingNum++;
			cout << "로딩:"<< LoadingNum << endl;
			//생성후 해제가 아닌 UNITMANAGER->add(rc)
		}
	}
	static void func4(int a, int b)
	{
		
	}
};

