#pragma once

class ParticleExp : public Particle
{
private:
	struct VertexParticle
	{
		Vector3 position;
		Vector2 size;
		Vector3 velocity;

		VertexParticle()
			: position(0, 0, 0), size(0, 0), velocity(0, 0, 0)
		{
		}
	};

	class VSBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			float duration;
			float age;
			float gravity;

			float padding;
		}data;

		VSBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			data.duration = 0.0f;
			data.age = 0.0f;
			data.gravity = 0.0f;
		}
	};

	class PSBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			Color startColor;
			Color endColor;
		}data;

		PSBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			data.startColor = Color(1, 1, 1, 1);
			data.endColor = Color(1, 1, 1, 1);
		}
	};

	vector<VertexParticle> vertices;
	VSBuffer* vsBuffer;
	PSBuffer* psBuffer;
public:
	ParticleExp();
	~ParticleExp();

	void Update() override;
	void Render() override;

	void Play(Vector3 position, Vector3 rot, Vector3 scale) override;

	void Reset() override;

	void SetVelocity();
};