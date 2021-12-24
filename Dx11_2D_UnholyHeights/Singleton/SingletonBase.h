#pragma once

/*
싱글톤 패턴은 static member로 하나의 인스턴스만 생성되며
프로그램 안에서 전역적으로 접근이 가능하다.
보통의 클래스의 인스턴스를 하나만 유지 하고자 할때 사용한다

전역 변수를 이용해도 하나의 인스턴스 유지할 수 있지만
싱글톤 패턴을 사용하면 클래스 잔시의 자기의 유일한 인스턴스로
접근하는 방법을 캡슐화 하여 관리할수 있고 상속 객체 생성
객체 소멸 인스턴스 개수 제어 등등 활용에 유리하다.
*/

template <typename T>
class SingletonBase {
protected:
	static T* Instance;

	SingletonBase() {};
	~SingletonBase() {};
public:
	static T* GetInstance();
	void ReleaseSingleton();
};

//정적 멤버 변수는 클래스 밖에서 초기화 가능
template <typename T>
T* SingletonBase<T>::Instance = nullptr;

template<typename T>
T * SingletonBase<T>::GetInstance()
{
	if (!Instance) Instance = new T;
	return Instance;
}

template<typename T>
void SingletonBase<T>::ReleaseSingleton()
{
	if (Instance)
	{
		delete Instance;
		Instance = nullptr;
	}
}
