#pragma once
#include "GameNode.h"

//��Ƽ �������� �ʿ��� stl
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
		WRITEMANAGER->RenderText(L"Loading", frc, 30.0f, L"����ü", D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), DWRITE_FONT_WEIGHT_NORMAL);

		WRITEMANAGER->GetDc()->EndDraw();
		SwapChain->Present(0, 0);

		mtx.unlock();
		//��� ������ ����Ǹ�
		SCENEMANAGER->ChangeScene("Game");
	}

	static void func2(int a)
	{
		for (int i = 0; i < a; i++)
		{
			cout << "������ 2 �۵���" << endl;
		}
	}
	static void func3(int a,int b)
	{
		for (int i = 0; i < a; i++)
		{
			Rect* rc = new Rect();
			cout << b<<"�����忡�� ����" << endl;
			SAFE_DELETE(rc);
			LoadingNum++;
			cout << "�ε�:"<< LoadingNum << endl;
			//������ ������ �ƴ� UNITMANAGER->add(rc)
		}
	}
	static void func4(int a, int b)
	{
		
	}
};

