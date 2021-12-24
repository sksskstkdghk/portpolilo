#pragma once



//디버그,문자집합 매크로
#if defined UNICODE && defined _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#elif defined _MBCS && defined _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif 

//매크로 함수
#define SAFE_DELETE(p) {if(p){delete(p); (p)=nullptr;}}
#define SAFE_DELETE_ARRAY(p) {if(p) {delete[](p); (p) = nullptr;}}
#define SAFE_RELEASE(p){ if(p){ (p)->Release(); (p) = nullptr; } }

#define MAKEGET(Type, Value)	\
	Type Get##Value(){return Value;}
#define MAKESET(Type, Value)	\
	void Set##Value(Type val){Value = val;}
#define MAKEGETSET(Type, Value) \
	MAKEGET(Type, Value) MAKESET(Type, Value)
#define MAKEREFGET(Type, Value)	\
	Type& GetRef##Value(){return Value;}

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <vector>
#include <map>
#include <string>
#include <assert.h>

using namespace std;

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dX10.h>
#include <d3dx10math.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

//ImGui
#include "imgui/imgui.h"
#include "imgui/imguiDx11.h"
#pragma comment(lib,"imgui/imgui.lib")

//Direct Write
#include <d2d1_1.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")


//문자집합 매크로
#ifdef _UNICODE
#define _tstring wstring
#define _tcout wcout
#else
#define _tstring string
#define _tcout cout
#endif


#include "MyFunc.h"
#include "./Object/Rect.h"
#include "./Object/Circle.h"
#include "./Object/Shader.h"
#include "./Object/Image.h"
#include "./Object/Button.h"
#include "./Object/Bullet.h"
#include "./Character/Character.h"
#include "./MensionObject/Quest.h"
#include "./MensionObject/KingRoom.h"

//싱글톤 추가
#include "./Singleton/KeyManager.h"
#define KEYMANAGER KeyManager::GetInstance()
#include "./Singleton/TimeManager.h"
#define TIMEMANAGER TimeManager::GetInstance()
#define g_ETime TimeManager::GetInstance()->GetElapsedTime()
#include "./Singleton/SoundManager.h"
#define SOUNDMANAGER SoundManager::GetInstance()
#include"./Singleton/CameraManager.h"
#define CAMMANAGER CameraManager::GetInstance()
#include"./Singleton/ImageManager.h"
#define IMGMANAGER ImageManager::GetInstance()
#include"./Singleton/WriteManager.h"
#define WRITEMANAGER WriteManager::GetInstance()
#include"./Singleton/CharacterManager.h"
#define CHARACTERMANAGER CharacterManager::GetInstance()
#include"./Singleton/SceneManager.h"
#define SCENEMANAGER SceneManager::GetInstance()
#include"./Singleton/QuestManager.h"
#define QUESTMANAGER QuestManager::GetInstance()

#define WINNAME _T("DX11")
#define WINSTARTX 0
#define WINSTARTY 0
#define WINSIZEX 1280
#define WINSIZEY 720
#define FIXFRAME 600



//전역변수
extern HINSTANCE	g_hInst;
extern HWND			g_hwnd;
extern IDXGISwapChain*				SwapChain;
extern ID3D11Device*				Device;
extern ID3D11DeviceContext*			DeviceContext;
extern ID3D11RenderTargetView*		RTV;
extern D3DXVECTOR2		g_ptMouse;
extern D3DXVECTOR2		g_ptCam;

extern vector<D3DXVECTOR2> upFloor;
extern vector<D3DXVECTOR2> downFloor;
extern short wheel;

extern KingRoom* kingDoor;
extern int playerGold;
extern D3DXCOLOR backColor;
extern float gameTime;		//게임 내 자체 시간

enum ApartStep
{
	APART01 = 0,
	APART02, APART03, APART04
};
