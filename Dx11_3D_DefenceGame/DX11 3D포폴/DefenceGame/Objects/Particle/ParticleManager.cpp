#include "Framework.h"

ParticleManager* ParticleManager::instance = nullptr;

ParticleManager::ParticleManager()
{
	CreateExp();
}

ParticleManager::~ParticleManager()
{
	for (auto particles : totalParticle)
	{
		for (Particle* particle : particles.second)
		{
			delete particle;
		}
		particles.second.clear();
	}
	totalParticle.clear();
}

void ParticleManager::Update()
{
	for (auto particles : totalParticle)
	{
		for (Particle* particle : particles.second)
		{
			particle->Update();
		}
	}
}

void ParticleManager::Render()
{
	for (auto particles : totalParticle)
	{
		for (Particle* particle : particles.second)
		{
			particle->Render();
		}
	}
}

void ParticleManager::Play(string key, Vector3 position, Vector3 rot, Vector3 scale)
{
	if (totalParticle.count(key) == 0)
		return;

	for (Particle* particle : totalParticle[key])
	{
		if (!particle->IsPlay())
		{
			particle->Play(position, rot, scale);
			return;
		}
	}
}

void ParticleManager::CreateExp()
{
	UINT poolCount = 50;

	vector<Particle*> particles;

	for (UINT i = 0; i < poolCount; i++)
	{
		Particle* particle = new ParticleExp();
		particles.push_back(particle);
	}

	totalParticle.insert({ "Exp", particles });
}
