#pragma once

class AModelAnimator : public Transform
{
protected:
	struct ClipTransform
	{
		Matrix** transform;

		ClipTransform()
		{
			transform = new Matrix * [MAX_MODEL_KEY];

			for (UINT i = 0; i < MAX_MODEL_KEY; i++)
				transform[i] = new Matrix[MAX_MODEL_BONE];
		}
		
		~ClipTransform()
		{
			for (UINT i = 0; i < MAX_MODEL_KEY; i++)
			{
				delete[] transform[i];
			}
			delete[] transform;
		}
	};

	struct KeyFrameDesc
	{
		int clip;

		UINT curFrame, nextFrame;

		float time, runningTime;

		float speed;

		float padding[2];

		KeyFrameDesc()
		{
			clip = 0;
			curFrame = 0;
			nextFrame = 0;

			time = 0.0f;
			runningTime = 0.0f;

			speed = 1.0f;
		}
	};

	struct TweenDesc
	{
		float takeTime;
		float tweenTime;
		float runningTime;
		float padding;

		KeyFrameDesc cur;
		KeyFrameDesc next;

		TweenDesc()
		{
			takeTime = 1.0f;
			tweenTime = 0.0f;
			runningTime = 0.0f;

			cur.clip = 0;
			next.clip = -1;
		}
	};

	class FrameBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			TweenDesc tweenDesc[MAX_MODEL_INSTANCE];
		}data;

		FrameBuffer() : ConstBuffer(&data, sizeof(Data))
		{
		}
	};


	ClipTransform* clipTransforms;
	
	Matrix worlds[MAX_MODEL_INSTANCE];

	VertexBuffer* instanceBuffer;

	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;

	FrameBuffer* frameBuffer;

	Shader* shader;

	int count;

protected:
	AModel* model;
	vector<Transform*> transforms;
	vector<TweenDesc*> frameDatas;

public:
	AModelAnimator(Shader* shader);
	~AModelAnimator();

	void Update();
	void Render();

	void ReadMaterial(string file, string folder);
	void ReadMesh(string file, string folder);
	void ReadClip(string file, string folder);

	void PlayClip(UINT instance, UINT clip, float speed = 1.0f, float takeTime = 1.0f);

	Transform* AddTransform();

	void UpdateTransforms();

	AModel* GetModel() { return model; }

	UINT GetCurFrame(UINT index){ return frameBuffer->data.tweenDesc[index].cur.curFrame; }

	void Animation(UINT index);

private:
	void CreateTexture();
	void CreateClipTransform(UINT index);
	
};