#pragma once

class ParticleRain : public Particle
{
private:
	class VSBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			Vector3 velocity;
			float distance;

			Color color;
			Vector3 origin;
			float time;

			Vector3 size;

			float padding;
		}data;

		VSBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			data.velocity = Vector3(0, -1, 0);
			data.distance = 1000.0f;

			data.color = Color(1, 1, 1, 1);

			data.origin = Vector3(0, 0, 0);
			data.size = Vector3(0, 0, 0);

			data.time = 0.0f;
		}
	};

	VSBuffer* vsBuffer;
	vector<VertexSize> vertices;

public:
	ParticleRain();
	~ParticleRain();

	void Update() override;
	void Render() override;

	void Reset() override;
};