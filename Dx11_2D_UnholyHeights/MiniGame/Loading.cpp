#include "stdafx.h"
#include "Loading.h"

//정적멤버 초기화
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
	////스레드 객체 생성시에 함수포인터로 매개변수 그리고
	////함수에 매개변수가 필요한 경우엔 순서대로 입력
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
	//cout << "init호출" << endl;
	//
	////join을 통하여 종료시점 명시
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
	//메인게임 이닛 후 스레드 돌리기

	thread t[1];

	t[0] = thread(func1);
	t[0].join();

	
	//업데이트에서 씬전환시 타일맵은 업데이트 호출이 안되있어서
	//수동으로 호출
	SCENEMANAGER->GetcurrentScene()->update();
}

void Loading::render()
{
}
