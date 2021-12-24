#include "Framework.h"

ParticleRain::ParticleRain()
	: Particle(L"Textures/Star.png", 500)
{
	shader = Shader::Add(L"ParticleRain");
	shader->CreateGeometryShader();

	Reset();
}

ParticleRain::~ParticleRain()
{
	delete vsBuffer;
}

void ParticleRain::Update()
{
	vsBuffer->data.time += DELTA;
}

void ParticleRain::Render()
{
	vsBuffer->SetVSBuffer(10);
	Particle::Render();

	ImGui::SliderFloat3("Velocity", (float*)&vsBuffer->data.velocity, -50, 50);
	ImGui::ColorEdit4("Color", vsBuffer->data.color);
	ImGui::SliderFloat("Distance", &vsBuffer->data.distance, 0, 2000);
	ImGui::SliderFloat3("Origin", (float*)&vsBuffer->data.origin, -50, 50);
}

void ParticleRain::Reset()
{
	vertices.resize(particleCount);

	Vector3 size = Vector3(10, 10, 10);

	for (UINT i = 0; i < particleCount; i++)
	{
		Vector2 scale;
		scale.x = Random(0.1f, 0.4f);
		scale.y = Random(4.0f, 8.0f);

		Vector3 position;
		position.x = Random(-size.x, size.x);
		position.y = Random(-size.y, size.y);
		position.z = Random(-size.z, size.z);

		vertices[i].position = position;
		vertices[i].size = scale;
	}

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexSize), vertices.size());

	vsBuffer = new VSBuffer();
	vsBuffer->data.size = size;
	vsBuffer->data.velocity = { 0, -10, 0 };
}
