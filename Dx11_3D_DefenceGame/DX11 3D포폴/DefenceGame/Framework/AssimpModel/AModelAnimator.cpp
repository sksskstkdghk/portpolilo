#include "Framework.h"

AModelAnimator::AModelAnimator(Shader* shader)
	: shader(shader), texture(nullptr), srv(nullptr)
{
	model = new AModel();
	frameBuffer = new FrameBuffer();

	for (UINT i = 0; i < MAX_MODEL_INSTANCE; i++)
		D3DXMatrixIdentity(&worlds[i]);
	
	instanceBuffer = new VertexBuffer(worlds, sizeof(Matrix), MAX_MODEL_INSTANCE);
}

AModelAnimator::~AModelAnimator()
{
	delete model;
	delete frameBuffer;
	delete[] clipTransforms;
	
	texture->Release();
	srv->Release();

	for (int i = 0; i < transforms.size(); i++)
	{
		delete transforms[i];
		delete frameDatas[i];
	}

	delete instanceBuffer;

	frameDatas.clear();
	frameDatas.shrink_to_fit();

	transforms.clear();
	transforms.shrink_to_fit();
}

void AModelAnimator::Update()
{

	//UpdateTransforms();

	/*for (AModelMesh* mesh : *model->GetMeshes())
		mesh->Update();*/
}

void AModelAnimator::Render()
{
	if (texture == nullptr)
		CreateTexture();

	if (count <= 0)
		return;

	instanceBuffer->Set(1);
	frameBuffer->SetVSBuffer(10);
	SetVS();

	for (AModelMesh* mesh : *model->GetMeshes())
		mesh->Render(count);
}

void AModelAnimator::ReadMaterial(string file, string folder)
{
	model->ReadMaterial(file, folder);
}

void AModelAnimator::ReadMesh(string file, string folder)
{
	model->ReadMesh(file, folder);

	for (AModelMesh* mesh : *model->GetMeshes())
		mesh->SetShader(shader);
}

void AModelAnimator::ReadClip(string file, string folder)
{
	model->ReadClip(file, folder);
}

void AModelAnimator::PlayClip(UINT instance, UINT clip, float speed, float takeTime)
{
	if (frameDatas[instance]->cur.clip == clip)
		return;

	frameDatas[instance]->takeTime = takeTime;
	frameDatas[instance]->next.clip = clip;
	frameDatas[instance]->next.speed = speed;
}

Transform* AModelAnimator::AddTransform()
{
	Transform* transform = new Transform();
	transforms.push_back(transform);

	frameDatas.emplace_back(new TweenDesc());

	return transform;
}

void AModelAnimator::UpdateTransforms()
{
	for (UINT i = 0; i < transforms.size(); i++)
		transforms[i]->UpdateWorld();
}

void AModelAnimator::CreateTexture()
{
	clipTransforms = new ClipTransform[model->GetClips()->size()];
	for (UINT i = 0; i < model->GetClips()->size(); i++)
		CreateClipTransform(i);

	{//CreateTexture
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = MAX_MODEL_BONE * 4;
		desc.Height = MAX_MODEL_KEY;
		desc.ArraySize = model->GetClips()->size();
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;

		UINT pageSize = MAX_MODEL_BONE * 4 * 16 * MAX_MODEL_KEY;

		void* p = VirtualAlloc(nullptr, pageSize * model->GetClips()->size(),
			MEM_RESERVE, PAGE_READWRITE);

		for (UINT c = 0; c < model->GetClips()->size(); c++)
		{
			UINT start = c * pageSize;

			for (UINT y = 0; y < MAX_MODEL_KEY; y++)
			{
				void* temp = (BYTE*)p + MAX_MODEL_BONE * y * sizeof(Matrix) + start;

				VirtualAlloc(temp, MAX_MODEL_BONE * sizeof(Matrix),
					MEM_COMMIT, PAGE_READWRITE);
				memcpy(temp, clipTransforms[c].transform[y], MAX_MODEL_BONE * sizeof(Matrix));
			}
		}

		D3D11_SUBRESOURCE_DATA* subResource = new D3D11_SUBRESOURCE_DATA[model->GetClips()->size()];
		for (UINT c = 0; c < model->GetClips()->size(); c++)
		{
			void* temp = (BYTE*)p + c * pageSize;

			subResource[c].pSysMem = temp;
			subResource[c].SysMemPitch = MAX_MODEL_BONE * sizeof(Matrix);
			subResource[c].SysMemSlicePitch = pageSize;
		}
		DEVICE->CreateTexture2D(&desc, subResource, &texture);

		delete[] subResource;
		VirtualFree(p, 0, MEM_RELEASE);
	}

	{//CreateSRV
		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.MipLevels = 1;
		desc.Texture2DArray.ArraySize = model->GetClips()->size();

		DEVICE->CreateShaderResourceView(texture, &desc, &srv);
	}
	
	for (AModelMesh* mesh : *model->GetMeshes())
		mesh->SetTransformsSRV(srv);
}

void AModelAnimator::CreateClipTransform(UINT index)
{
	Matrix* bones = new Matrix[MAX_MODEL_BONE];

	AModelClip* clip = model->ClipByIndex(index);
	for (UINT f = 0; f < clip->FrameCount(); f++)
	{
		for (UINT b = 0; b < model->GetBones()->size(); b++)
		{
			AModelBone* bone = model->GetBones()->at(b);

			Matrix parent;
			Matrix invGlobal = bone->transform;
			D3DXMatrixInverse(&invGlobal, nullptr, &invGlobal);

			int parentIndex = bone->parentIndex;
			if (parentIndex < 0)
				D3DXMatrixIdentity(&parent);
			else
				parent = bones[parentIndex];

			Matrix animation;
			AssimpKeyFrame* frame = clip->KeyFrame(bone->name);
			if (frame != nullptr)
			{
				AssimpKeyFrameData& data = frame->transforms[f];

				Matrix S, R, T;
				D3DXMatrixScaling(&S, data.scale.x, data.scale.y, data.scale.z);
				D3DXMatrixRotationQuaternion(&R, &data.rotation);
				D3DXMatrixTranslation(&T, data.position.x, data.position.y, data.position.z);

				animation = S * R * T;
			}
			else
			{
				D3DXMatrixIdentity(&animation);
			}

			bones[b] = animation * parent;
			clipTransforms[index].transform[f][b] = invGlobal * bones[b];
		}
	}
}

void AModelAnimator::Animation(UINT index)
{
		TweenDesc* tweenDesc = frameDatas[index];

		{//Cur Animation
			KeyFrameDesc& cur = tweenDesc->cur;
			AModelClip* clip = model->ClipByIndex(cur.clip);
			cur.runningTime += DELTA;

			float time = 1.0f / clip->FrameRate() / cur.speed;
			if (cur.time >= 1.0f)
			{
				cur.runningTime = 0.0f;

				cur.curFrame = (cur.curFrame + (UINT)cur.time) % clip->FrameCount();
				cur.nextFrame = (cur.curFrame + 1) % clip->FrameCount();
			}
			cur.time = cur.runningTime / time;
		}

		{//Next Animation

			KeyFrameDesc& next = tweenDesc->next;

			if (next.clip > -1)
			{
				AModelClip* clip = model->ClipByIndex(next.clip);

				tweenDesc->runningTime += DELTA;
				tweenDesc->tweenTime = tweenDesc->runningTime / tweenDesc->takeTime;

				if (tweenDesc->tweenTime >= 1.0f)
				{
					tweenDesc->cur = next;

					next.clip = -1;
					next.curFrame = 0;
					next.nextFrame = 0;
					next.time = 0.0f;
					next.runningTime = 0.0f;

					tweenDesc->runningTime = 0.0f;
					tweenDesc->tweenTime = 0.0f;
				}
				else
				{
					next.runningTime += DELTA;

					float time = 1.0f / clip->FrameRate() / next.speed;
					if (next.time >= 1.0f)
					{
						next.runningTime = 0.0f;

						next.curFrame = (next.curFrame + 1) % clip->FrameCount();
						next.nextFrame = (next.curFrame + 1) % clip->FrameCount();
					}
					next.time = next.runningTime / time;
				}
			}
		}
}
