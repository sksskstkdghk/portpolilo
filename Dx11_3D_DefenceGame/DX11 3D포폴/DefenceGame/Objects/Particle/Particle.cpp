#include "Framework.h"

Particle::Particle(wstring diffuseFile, UINT particleCount)
	: particleCount(particleCount), isPlay(false)
{
	texture = Texture::Add(diffuseFile);	

	blendState[0] = new BlendState();
	blendState[1] = new BlendState();
	blendState[1]->Additive();
}

Particle::~Particle()
{
	delete vertexBuffer;	
	delete blendState[0];
	delete blendState[1];
}

void Particle::Update()
{
}

void Particle::Render()
{
	SetVS();
	
	vertexBuffer->Set();
	IASetPT(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	texture->Set(10);

	shader->Set();
	VP->SetGSBuffer(0);

	blendState[1]->SetState();
	DC->Draw(particleCount, 0);
	blendState[0]->SetState();

	DC->GSSetShader(nullptr, nullptr, 0);
}

void Particle::Reset()
{
}

void Particle::Play(Vector3 position, Vector3 rot, Vector3 scale)
{
	isPlay = true;
	this->position = position;
	this->rotation = rot;
	this->scale = scale;	
}

void Particle::Stop()
{
	isPlay = false;
}
