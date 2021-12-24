#pragma once
#include "SingletonBase.h"

class GameNode;

class SceneManager : public SingletonBase<SceneManager>
{
	//�� == ���ӳ��(���ӳ�带 ��ӹ޴� ���ΰ� ���� �ɼ��� �ִ�.)
private:
	typedef map<string, GameNode*> m_sceneList;
	typedef map<string, GameNode*>::iterator m_sceneListIter;

	GameNode*			currentScene = nullptr;		//���� ��
	m_sceneList			sceneList;					//�� ���

public:
	SceneManager() {};
	~SceneManager() {};

	//��ü �� ����
	void ReleaseAll();
	//���� �� ȣ��
	void CurrentSceneUpdate();
	void CurrentSceneRender();


	//�� �߰�
	GameNode* AddScene(string name, GameNode* scene);
	//�� ����
	HRESULT ChangeScene(string name);

	//����� �޾ƿ���
	MAKEGET(GameNode*, currentScene);


};

