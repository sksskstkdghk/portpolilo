#pragma once

class Particle : public Transform
{
protected:
	
	Shader* shader;	

	VertexBuffer* vertexBuffer;

	Texture* texture;
	UINT particleCount;
	bool isPlay;

	BlendState* blendState[2];

public:
	Particle(wstring diffuseFile, UINT particleCount);
	virtual ~Particle();

	virtual void Update();
	virtual void Render();

	virtual void Reset();

	virtual void Play(Vector3 position, Vector3 rot, Vector3 scale);
	virtual void Stop();

	bool IsPlay() { return isPlay; }
};