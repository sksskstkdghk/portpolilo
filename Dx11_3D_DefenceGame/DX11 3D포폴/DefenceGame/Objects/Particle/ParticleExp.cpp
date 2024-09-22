#include "Framework.h"

ParticleExp::ParticleExp()
	: Particle(L"Textures/star.png", 500)
{
	shader = Shader::Add(L"ParticleExp");
	shader->CreateGeometryShader();

	vsBuffer = new VSBuffer();
	psBuffer = new PSBuffer();

	Reset();
}

ParticleExp::~ParticleExp()
{
	delete vsBuffer;
	delete psBuffer;
}

void ParticleExp::Update()
{
	if (!isPlay)
		return;

	vsBuffer->data.age += DELTA;

	if (vsBuffer->data.age > vsBuffer->data.duration)
		Stop();

	UpdateWorld();
}

void ParticleExp::Render()
{
	if (!isPlay)
		return;	

	vsBuffer->SetVSBuffer(10);
	psBuffer->SetPSBuffer(10);
	Particle::Render();
}

void ParticleExp::Play(Vector3 position, Vector3 rot, Vector3 scale)
{
	vsBuffer->data.age = 0.0f;
	Particle::Play(position, rot, scale);
	SetVelocity();
}

void ParticleExp::Reset()
{
	vertices.resize(particleCount);

	for (UINT i = 0; i < particleCount; i++)
	{
		vertices[i].position = position;
		float size = Random(0.1f, 0.5f);
		vertices[i].size = Vector2(size, size);

		float radius = Random(5.0f, 10.0f);

		Vector3 velocity = Vector3(0, 0, radius);

		Vector3 rot;
		rot.x = Random(0.0f, D3DX_PI * 2.0f);
		rot.y = Random(0.0f, D3DX_PI * 2.0f);
		rot.z = Random(0.0f, D3DX_PI * 2.0f);

		Matrix matRot;
		D3DXMatrixRotationYawPitchRoll(&matRot, rot.y, rot.x, rot.z);

		D3DXVec3TransformCoord(&velocity, &velocity, &matRot);
		vertices[i].velocity = velocity;
	}

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexParticle), particleCount);

	vsBuffer->data.duration = 2.0f;
	vsBuffer->data.gravity = 5.0f;
	psBuffer->data.startColor = { 1.0f, 1.0f, 0.0f, 1.0f };
	psBuffer->data.endColor = { 1.0f, 0.0f, 0.0f, 0.0f };
}

void ParticleExp::SetVelocity()
{
	for (UINT i = 0; i < particleCount; i++)
	{
		vertices[i].position = position;
		float size = Random(0.1f, 0.5f);
		vertices[i].size = Vector2(size, size);

		float radius = Random(5.0f, 10.0f);

		Vector3 velocity = Vector3(0, 0, radius);

		Vector3 rot;
		rot.x = Random(0.0f, D3DX_PI * 2.0f);
		rot.y = Random(0.0f, D3DX_PI * 2.0f);
		rot.z = Random(0.0f, D3DX_PI * 2.0f);

		Matrix matRot;
		D3DXMatrixRotationYawPitchRoll(&matRot, rot.y, rot.x, rot.z);

		D3DXVec3TransformCoord(&velocity, &velocity, &matRot);
		vertices[i].velocity = velocity;
	}

	vertexBuffer->UpdateBuffer(vertices.data(), particleCount);
}
