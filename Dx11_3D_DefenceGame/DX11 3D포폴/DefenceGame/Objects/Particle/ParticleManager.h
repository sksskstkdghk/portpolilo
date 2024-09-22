#pragma once

class ParticleManager
{
private:
	map<string, vector<Particle*>> totalParticle;

	static ParticleManager* instance;

	ParticleManager();
	~ParticleManager();
public:
	static ParticleManager* Get() { return instance; }
	static void Create() { instance = new ParticleManager(); }
	static void Delete() { delete instance; }

	void Update();
	void Render();

	void Play(string key, Vector3 position, Vector3 rot = Vector3(0, 0, 0),
		Vector3 scale = Vector3(1, 1, 1));

	void CreateExp();
};