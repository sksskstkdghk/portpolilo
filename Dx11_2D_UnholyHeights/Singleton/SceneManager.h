#pragma once
#include "SingletonBase.h"

class GameNode;

class SceneManager : public SingletonBase<SceneManager>
{
	//씬 == 게임노드(게임노드를 상속받는 전부가 씬이 될수가 있다.)
private:
	typedef map<string, GameNode*> m_sceneList;
	typedef map<string, GameNode*>::iterator m_sceneListIter;

	GameNode*			currentScene = nullptr;		//현재 씬
	m_sceneList			sceneList;					//씬 목록

public:
	SceneManager() {};
	~SceneManager() {};

	//전체 씬 삭제
	void ReleaseAll();
	//현재 씬 호출
	void CurrentSceneUpdate();
	void CurrentSceneRender();


	//씬 추가
	GameNode* AddScene(string name, GameNode* scene);
	//씬 변경
	HRESULT ChangeScene(string name);

	//현재씬 받아오기
	MAKEGET(GameNode*, currentScene);


};

