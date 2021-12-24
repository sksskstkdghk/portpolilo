#include "Framework.h"

TerrainEditor* TerrainEditor::instance = nullptr;

TerrainEditor::TerrainEditor(UINT width, UINT height)
	: width(width), height(height), selectType((int)TYPE_NONE), pickingRect(PickingRect(width, height))
{
	material = new Material(L"TerrainBrush");
	material->SetDiffuseMap(L"Textures/Landscape/Stones.png");
	material->SetNormalMap(L"Textures/Landscape/Stones_normal.png");

	CreateData();

	Vector2 pos = Vector2(WIN_WIDTH * 0.5f, WIN_HEIGHT * 0.95f);
	Vector2 scale = Vector2(WIN_WIDTH * 0.1f, WIN_HEIGHT * 0.1f);

	TypeUI.push_back(USER->PushUI(Vector2(pos.x - scale.x * 0.5f, pos.y), scale, nullptr, L"road"));
	TypeUI.push_back(USER->PushUI(Vector2(pos.x + scale.x * 0.5f, pos.y), scale, nullptr, L"FriendlyBase"));

	count = 0;

	UpdateTransforms();

	wall = new BoxWall();
}

TerrainEditor::~TerrainEditor()
{
	delete material;

	delete vertexBuffer;
	delete indexBuffer;
	delete instanceBuffer;

	delete wall;

	rectDatas.clear();
	rectDatas.shrink_to_fit();
}

void TerrainEditor::Update()
{
	for (UINT i = 0; i < TypeUI.size(); i++)
	{
		if (selectType - 1 == i)
			TypeUI[i]->SetBackColor(Color(0.0f, 1.0f, 0.0f, 1.0f));
		else
			TypeUI[i]->SetBackColor(Color(0.0f, 0.0f, 0.0f, 1.0f));

		if(TypeUI[i]->IsMouseCollision())
			if (KEYDOWN(VK_LBUTTON))
			{
				selectType = i + 1;
				break;
			}
	}

	SelectMapToType();

	if (KEYDOWN(VK_F1))
		Save();

	if (KEYDOWN(VK_F2))
		Load();

	if (KEYDOWN(VK_RBUTTON))
	{
		InitColor();
		selectType = 0;
	}


	//UpdateWorld();
}

void TerrainEditor::Render()
{
	count = 0;

	for (UINT i = 0; i < height * width; i++)
	{
		if (CAMERA->GetCulling()->PointCulling(&transforms[i]->position))
		{
			memcpy_s(&instanceRenderDatas[count], sizeof(InstanceData), &instanceDatas[i], sizeof(InstanceData));

			instanceRenderDatasIndex[count] = i;

			count++;
		}
	}

	instanceBuffer->UpdateBuffer(instanceRenderDatas, count);

	instanceBuffer->Set(1);
	vertexBuffer->Set();
	indexBuffer->Set();

	IASetPT();
	material->Set();

	DC->DrawIndexedInstanced(indices.size(), count, 0, 0, 0);

	int temp = 0;

	for (UINT i = 0; i < count; i++)
	{
		if ((int)ASTAR->GetNode(instanceRenderDatasIndex[i])->GetState() == 4)
		{
			memcpy_s(&instanceRenderDatas[temp], sizeof(InstanceData), &instanceDatas[instanceRenderDatasIndex[i]], sizeof(InstanceData));

			temp++;
		}
	}

	instanceBuffer->UpdateBuffer(instanceRenderDatas, temp);

	wall->Render(instanceBuffer, temp);
}

void TerrainEditor::PostRender()
{
	ImGui::SliderFloat3("PickPos", pickingPos, -1000, 1000);
	ImGui::Text("BrushMode");

	ImGui::SetWindowFontScale(IMGUI_FONT_SCALE_SIZE);

	ImGui::SliderInt("SelectType", &selectType, 0, 3);
	ImGui::Text("RandDrawCount: %d", count);
}

void TerrainEditor::SetCastle()
{
	//맵 중앙 2x2를 성으로 만듬
	for (UINT z = 0; z < 2; z++)
		for (UINT x = 0; x < 2; x++)
		{
			UINT index = width / 2 + (height / 2 - z) * width - x;

			instanceDatas[index].color = rectDatas.at(index)->StateByColor();
			ASTAR->GetNodes().at(index)->SetState(CASTLE);
		}
}

//길을 만드는 함수
void TerrainEditor::MakeRoad()
{
	if (pickingRect.Picking(&pickingPos))
	{
		InitColor();

		ASTAR->Reset();

		/*for (UINT i = 0; i < rectDatas.size(); i++)
			wall->SetColor(rectDatas.at(i)->StateByColor());*/

		UINT index = (UINT)pickingPos.x + (UINT)pickingPos.z * width;

		if (ASTAR->GetNodes().at(index)->GetState() == MONSTER_ROAD)
		{
			instanceDatas[index].color = Color(1.0f, 0.0f, 0.0f, 1.0f);
			//wall->SetColor(Color(1.0f, 0.0f, 0.0f, 1.0f));
			return;
		}

		instanceDatas[index].color = Color(0.0f, 0.0f, 1.0f, 1.0f);
		//wall->SetColor(Color(0.0f, 0.0f, 1.0f, 1.0f));

		if (KEYPRESS(VK_LBUTTON))
		{
			rectDatas.at(index)->SetState((UINT)RAND_ROAD);
			//wall->SetColor(rectDatas.at(index)->StateByColor());

			ASTAR->GetNodes().at(index)->SetState(NONE);
		}
	}

	instanceBuffer->UpdateBuffer(instanceDatas.data(), instanceDatas.size());
}

//3x3 크기에 기지 만들기
void TerrainEditor::SelectRect(RectRandState changeState, int which)
{
	if (pickingRect.Picking(&pickingPos))
	{
		static vector<int> pickGridIndex;
		static bool isMake;

		//설치전 가능여부확인 및 범위지정
		if (pickX != pickingPos.x || pickZ != pickingPos.z)
		{
			isMake = true;

			pickGridIndex.clear();

			InitColor();

			pickX = (int)pickingPos.x;
			pickZ = (int)pickingPos.z;

			for (int z = -1; z <= 1; z++)
			{
				for (int x = -1; x <= 1; x++)
				{
					int CheckX, CheckZ;
					CheckX = pickX + x;
					CheckZ = pickZ + z;

					if (CheckX < 0 || CheckX >= width ||
						CheckZ < 0 || CheckZ >= height)
						continue;

					pickGridIndex.emplace_back(CheckZ * width + CheckX);
				}
			}

			//3x3 설치전 설치 가능한지 불가능한지 확인
			//5x5 사이즈
			for (int z = -2; z <= 2; z++)
			{
				for (int x = -2; x <= 2; x++)
				{
					int CheckX, CheckZ;
					CheckX = pickX + x;
					CheckZ = pickZ + z;

					if (CheckX < 0 || CheckX >= width ||
						CheckZ < 0 || CheckZ >= height)
						continue;

					RectRandState temp = (RectRandState)rectDatas[CheckZ * width + CheckX]->GetRandState();

					if (temp == RectRandState::RAND_ENEMY_BASE ||
						temp == RectRandState::RAND_MAIN_OFFICE)
					{
						isMake = false;

						break;
					}
				}

				if (!isMake)
					break;
			}

			for (int iter : pickGridIndex)
			{
				if (isMake)
					instanceDatas[iter].color = Color(0.0f, 0.0f, 1.0f, 1.0f);
				else
					instanceDatas[iter].color = Color(1.0f, 0.0f, 0.0f, 1.0f);
			}
		}

		if (isMake && KEYDOWN(VK_LBUTTON))
		{
			for (int iter : pickGridIndex)
			{
				rectDatas[iter]->SetState((UINT)changeState);

				if(which == (UINT)TYPE_FRIENDLY)
					ASTAR->GetNodes().at(iter)->SetState(FRIENDLY);
				else if (which == (UINT)TYPE_DUNGEON)
					ASTAR->GetNodes().at(iter)->SetState(MONSTER_ROAD);
			}

			SPAWN->AddWhichBase(pickingPos, which);
		}
	}
	
	instanceBuffer->UpdateBuffer(instanceDatas.data(), instanceDatas.size());
}

void TerrainEditor::SelectMapToType()
{
	static bool isInit = false;

	if (USER->GetMouseCol() ||
		!pickingRect.Picking(&pickingPos))
	{
		pickingPos.x = -10.0f;

		if (isInit)
		{
			//InitColor();
			isInit = false;
		}
	}
	else
	{
		isInit = true;

		if (selectType == (int)TYPE_ROAD)
			MakeRoad();
		else if (selectType == (int)TYPE_FRIENDLY)
			SelectRect(RAND_FRIENDLY_BASE, (int)TYPE_FRIENDLY);
		else if (selectType == (int)TYPE_DUNGEON)
			SelectRect(RAND_ENEMY_BASE, (int)TYPE_DUNGEON);
	}
}

void TerrainEditor::CreateNormal()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		VertexType v0 = vertices[index0];
		VertexType v1 = vertices[index1];
		VertexType v2 = vertices[index2];

		Vector3 A = v1.position - v0.position;
		Vector3 B = v2.position - v0.position;

		Vector3 normal;
		D3DXVec3Cross(&normal, &A, &B);

		vertices[index0].normal += normal;
		vertices[index1].normal += normal;
		vertices[index2].normal += normal;
	}

	for (VertexType& vertex : vertices)
		D3DXVec3Normalize(&vertex.normal, &vertex.normal);
}

void TerrainEditor::CreateTangent()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		VertexType vertex0 = vertices[index0];
		VertexType vertex1 = vertices[index1];
		VertexType vertex2 = vertices[index2];

		Vector3 p0 = vertex0.position;
		Vector3 p1 = vertex1.position;
		Vector3 p2 = vertex2.position;

		Vector2 uv0 = vertex0.uv;
		Vector2 uv1 = vertex1.uv;
		Vector2 uv2 = vertex2.uv;

		Vector3 e0 = p1 - p0;
		Vector3 e1 = p2 - p0;

		float u0 = uv1.x - uv0.x;
		float u1 = uv2.x - uv0.x;
		float v0 = uv1.y - uv0.y;
		float v1 = uv2.y - uv0.y;

		float d = 1.0f / (u0 * v1 - v0 * u1);

		Vector3 tangent;
		tangent = (v1 * e0 - v0 * e1) * d;

		vertices[index0].tangent += tangent;
		vertices[index1].tangent += tangent;
		vertices[index2].tangent += tangent;
	}

	for (VertexType& vertex : vertices)
	{
		Vector3 t = vertex.tangent;
		Vector3 n = vertex.normal;

		Vector3 temp = (t - n * D3DXVec3Dot(&n, &t));
		D3DXVec3Normalize(&temp, &temp);

		vertex.tangent = temp;
	}
}

//맵상에 타일 깔기
void TerrainEditor::CreateData()
{
	vertices.resize(4);
	//wall = new BoxWall();

	{//Terrain edit

		int i = 0;

		{//Vertices
			for (UINT z = 0; z < 2; z++)
			{
				for (UINT x = 0; x < 2; x++)
				{
					//정점에 위치 인덱스
					UINT vertexAdressIndex = i++;

					VertexType vertex;
					vertex.position = Vector3((x), 0, (z));
					vertex.uv = Vector2(x, z);

					vertices.at(vertexAdressIndex) = vertex;
				}
			}
		}

		{//Indices
			indices.push_back(0);//0
			indices.push_back(3);//1
			indices.push_back(1);//2

			indices.push_back(0);//0				
			indices.push_back(2);//2
			indices.push_back(3);//3
		}

		{//Instance
			instanceDatas.resize(height * width);

			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					//위치 그리드 형식으로 판 깔기
					AddTransform()->position = Vector3(j, 0.0f, i);

					rectDatas.push_back(new RectRoad());
					
					/*if(j % 4 == 0)
						instanceDatas[i * width + j].color = Color(0.0f, 0.0f, 0.0f, 1.0f);
					else if (j % 4 == 1)
						instanceDatas[i * width + j].color = Color(1.0f, 0.0f, 0.0f, 1.0f);
					else if (j % 4 == 2)
						instanceDatas[i * width + j].color = Color(0.0f, 1.0f, 0.0f, 1.0f);
					else if (j % 4 == 3)
						instanceDatas[i * width + j].color = Color(0.0f, 0.0f, 1.0f, 1.0f);*/
				}
			}
		}
	}

	CreateNormal();
	CreateTangent();

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexType), vertices.size());
	indexBuffer = new IndexBuffer(indices.data(), indices.size());
	instanceBuffer = new VertexBuffer(instanceRenderDatas, sizeof(InstanceData), MAX_RAND_RENDER_SIZE);
}

Transform* TerrainEditor::AddTransform()
{
	Transform* transform = new Transform();
	transforms.push_back(transform);

	return transform;
}

void TerrainEditor::UpdateTransforms()
{
	for (UINT i = 0; i < transforms.size(); i++)
	{
		transforms[i]->UpdateWorld();
		memcpy(&instanceDatas[i].world, &transforms[i]->GetTransposeWorld(), sizeof(Matrix));
	}

	instanceBuffer->UpdateBuffer(instanceDatas.data(), height * width);
}

void TerrainEditor::InitColor()
{
	for (int i = 0; i < instanceDatas.size(); i++)
	{
		instanceDatas[i].color = Color(0.0f, 0.0f, 0.0f, 1.0f);
	}

	instanceBuffer->UpdateBuffer(instanceDatas.data(), height * width);
}

void TerrainEditor::Save()
{
	/*BinaryWriter* writer = new BinaryWriter(L"TextData/Height.map");

	UINT size = (width + 1) * (height + 1);
	heights.clear();
	heights.resize(size);
	writer->UInt(size);
	for (UINT i = 0; i < size; i++)
		heights[i] = vertices[i].position.y;
	writer->Byte(heights.data(), sizeof(float) * size);

	delete writer;*/
}

void TerrainEditor::Load()
{
	/*BinaryReader* reader = new BinaryReader(L"TextData/Height.map");

	UINT size = reader->UInt();
	if (size != 0)
	{
		heights.resize(size);
		void* data = heights.data();

		reader->Byte(&data, size * sizeof(float));

		for (UINT i = 0; i < size; i++)
			vertices[i].position.y = heights[i];
	}

	delete reader;

	CreateNormal();
	vertexBuffer->UpdateBuffer(vertices.data(), vertices.size());*/
}
