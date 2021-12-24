// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720
#define WIN_START_X 0
#define WIN_START_Y 0

#define MAX_MODEL_BONE 256
#define MAX_MODEL_KEY 500
#define MAX_MODEL_INSTANCE 500

#define MAX_RAND_RENDER_SIZE 700
#define IMGUI_FONT_SCALE_SIZE 1.5f

#define DEBUG(hr) assert(SUCCEEDED(hr))

#define DEVICE Device::Get()->GetDevice()
#define DC Device::Get()->GetContext()
#define SOUND FmodSound::Get()

#define VP Environment::Get()->GetVP()
#define LIGHT Environment::Get()->GetLight()
#define CAMERA Environment::Get()->GetCamera()
#define PERSPECTIVE Environment::Get()->GetPerspective()
#define ASTAR AStar::Get()
#define MAZE MazeAlgorithm::Get()
#define TERRAIN TerrainEditor::Get()
#define SPAWN SpawnManager::Get()
#define CHARACTER CharacterManager::Get()

#define KEYDOWN(k) Keyboard::Get()->Down(k)
#define KEYUP(k) Keyboard::Get()->Up(k)
#define KEYPRESS(k) Keyboard::Get()->Press(k)
#define USER UI::Get()
#define WRITE DirectWrite::Get()

#define DELTA Timer::Get()->Delta()

#define LERP(s, e, t) (s + (e - s)*t)

#include <windows.h>

#include <cassert>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <cmath>
#include <functional>
#include <algorithm>
#include <time.h>

using namespace std;

#include <d3d11.h>
#include <D3DX11.h>
#include <d3dcompiler.h>
#include <D3DX10math.h>

#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

//ImGui
#include <imgui.h>
#include <imguiDx11.h>
#pragma comment(lib, "imgui.lib")

//FBX SDK
//#define FBXSDK_SHARED
//#include <fbxsdk.h>
//#pragma comment(lib, "libfbxsdk.lib")

//using namespace fbxsdk;

//Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#pragma comment(lib, "assimp/assimp-vc142-mtd.lib")

#include <d2d1_2.h>
#include <dwrite.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

typedef D3DXVECTOR4 Vector4;
typedef D3DXVECTOR3 Vector3;
typedef D3DXVECTOR2 Vector2;
typedef D3DXMATRIX Matrix;
typedef D3DXCOLOR Color;
typedef D3DXQUATERNION Quaternion;

extern UINT mapWidth, mapHeight;

//고용 구조체
struct C_ManagerData
{
	string name;
	UINT instanceID;
};

//Framework Header
#include "Framework/Core/Device.h"

#include "Framework/Utility/Keyboard.h"
#include "Framework/Utility/Timer.h"
#include "Framework/Utility/Binary.h"
#include "Framework/Utility/Xml.h"
#include "Framework/Utility/Utility.h"
#include "Framework/Utility/Math.h"
#include "Framework/Utility/DirectWrite.h"

using namespace GameMath;

#include "Framework/Render/Shader.h"
#include "Framework/Render/Buffer.h"
#include "Framework/Render/ConstBuffer.h"
#include "Framework/Render/GlobalBuffer.h"
#include "Framework/Render/VertexLayouts.h"
#include "Framework/Render/Texture.h"
#include "Framework/Render/Material.h"
#include "Framework/Render/RenderTarget.h"

#include "Framework/Game/Transform.h"

#include "Framework/Camera/Camera.h"
#include "Framework/Camera/Freedom.h"

#include "Framework/Environment/Viewport.h"
#include "Framework/Environment/Perspective.h"
#include "Framework/Environment/Environment.h"
#include "Framework/Camera/FrustumCulling.h"

#include "Framework/FBX/FbxType.h"
#include "Framework/FBX/AssimpReader.h"

#include "Framework/Model/ModelMeshPart.h"
#include "Framework/Model/ModelMesh.h"
#include "Framework/Model/ModelKeyFrame.h"
#include "Framework/Model/ModelClip.h"
#include "Framework/Model/ModelTweener.h"
#include "Framework/Model/Model.h"
#include "Framework/Model/GameModel.h"
#include "Framework/Model/GameAnimModel.h"

#include "Framework/AssimpModel/AModelMesh.h"
#include "Framework/AssimpModel/AModelClip.h"
#include "Framework/AssimpModel/AModel.h"
#include "Framework/AssimpModel/AModelRender.h"
#include "Framework/AssimpModel/AModelAnimator.h"

#include "Framework/Collision/Collision.h"
#include "Framework/Collision/BoxCollision.h"
#include "Framework/Collision/SphereCollision.h"
#include "Framework/Collision/RectCollision.h"

#include "Framework/States/BlendState.h"
#include "Framework/States/DepthStencilState.h"
#include "Framework/States/RasterizerState.h"

#include "Framework/FmodSound/FmodSound.h"

#include "AStar/Node.h"
#include "AStar/Heap.h"
#include "AStar/AStar.h"
#include "AStar/MazeAlgorithm.h"

//Object Header
#include "Objects/Road/BoxWall.h"
#include "Objects/Road/RectRoad.h"
#include "Objects/Road/PickingRect.h"
#include "Objects/TerrainEditor.h"

#include "Objects/PostEffect/Render2D.h"

//User Interface
#include "UI/UI.h"

//성(지켜야 함)
#include "Objects/Castle.h"

//캐릭터
#include "Objects/Character/Character.h"

//아군
#include "Objects/Character/Friendly/Pikachu.h"

//적군
#include "Objects/Character/Monster/Monster01.h"

//스폰 지역
#include "Objects/Spawn/SpawnBase.h"
#include "Objects/Spawn/EnemyBase.h"
#include "Objects/Spawn/FriendlyBase.h"

//매니저
#include "Manager/CharacterManager.h"
#include "Manager/SpawnManager.h"

#include "Objects/Particle/Particle.h"
#include "Objects/Particle/ParticleExp.h"
#include "Objects/Particle/ParticleRain.h"
#include "Objects/Particle/ParticleManager.h"

//Scene Header
#include "Scenes/Scene.h"
#include "Scenes/Portfolio/GameScene.h"

//Program Header
#include "Program/Program.h"

extern HWND hWnd;
extern Vector3 mousePos;
extern float wheelValue;
extern float cullingSize;