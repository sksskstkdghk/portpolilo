#include "stdafx.h"
#include "SceneManager.h"
#include "GameNode.h"

void SceneManager::ReleaseAll()
{
	//반복자를 통하여 맵을 돌면서 하나씩 삭제
	m_sceneListIter iter = sceneList.begin();
	for (iter; iter != sceneList.end();)
	{
		if (iter->second != nullptr)
		{
			//현재씬이 삭제될때는 릴리즈 호출뒤 삭제
			if (iter->second == currentScene) iter->second->release();
			SAFE_DELETE(iter->second);
			iter = sceneList.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	sceneList.clear();
}

void SceneManager::CurrentSceneUpdate()
{
	if (currentScene)currentScene->update();
}

void SceneManager::CurrentSceneRender()
{
	if (currentScene)currentScene->render();
}

GameNode * SceneManager::AddScene(string name, GameNode * scene)
{
	if(!scene) return nullptr;

	sceneList.insert(make_pair(name, scene));

	return scene;
}

HRESULT SceneManager::ChangeScene(string name)
{
	m_sceneListIter iter = sceneList.find(name);

	//못찾을 경우
	if (iter == sceneList.end()) return E_FAIL;

	//바꾸려는 씬이 현재 씬과 같다면
	if (iter->second == currentScene) return E_FAIL;

	//현재씬과 같지도 않고 찾으려는값이 존재한다면
	if (SUCCEEDED(iter->second->init()))
	{
		//기존 바꾸기전 씬이 존재한다면 릴리즈 호출
		if (currentScene) currentScene->release();
		currentScene = iter->second;
		return S_OK;

	}
	else
	{
		return E_FAIL;
	}

	return E_FAIL;
}
