#include "stdafx.h"
#include "Loading.h"

//������� �ʱ�ȭ
int Loading::LoadingNum = 0;

Loading::Loading()
{
}


Loading::~Loading()
{
}

HRESULT Loading::init()
{
	////std::thread t[5];
	//thread t[5];
	////������ ��ü �����ÿ� �Լ������ͷ� �Ű����� �׸���
	////�Լ��� �Ű������� �ʿ��� ��쿣 ������� �Է�
	////t[0] = thread(func1);
	////t[1] = thread(func2,20);
	//
	////t[0] = thread(func2, 100);
	////t[1] = thread(func2, 20);
	//
	//
	//for (int i = 0; i < 5; i++)
	//{
	//	t[i] = thread(func3, 100, i);
	//}
	//
	//
	//
	//cout << "initȣ��" << endl;
	//
	////join�� ���Ͽ� ������� ���
	////t[0].join();
	////t[1].join();
	//for (int i = 0; i < 5; i++)
	//{
	//	t[i].join();
	//}
	return S_OK;
}

void Loading::release()
{
}

void Loading::update()
{
	//���ΰ��� �̴� �� ������ ������

	thread t[1];

	t[0] = thread(func1);
	t[0].join();

	
	//������Ʈ���� ����ȯ�� Ÿ�ϸ��� ������Ʈ ȣ���� �ȵ��־
	//�������� ȣ��
	SCENEMANAGER->GetcurrentScene()->update();
}

void Loading::render()
{
}
