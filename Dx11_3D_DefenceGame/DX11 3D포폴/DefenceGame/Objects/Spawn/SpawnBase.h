#pragma once

class SpawnBase : public Transform
{
private:
	typedef VertexUVColor VertexType;

	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	vector<VertexType> vertices;
	vector<UINT> indices;

	void CreateData();
protected:
	Texture* tex;
	Shader* shader;

	struct SummonData
	{
		string name;
		float summonTime;
	}summonData;

	//실제 사용하는 캐릭터들 집합
	vector<C_ManagerData> useChar;

	GameModel* magicCircle;

	UINT baseLVL;
	UINT summonCharacters;

	void SetDraw();
public:
	SpawnBase();
	~SpawnBase();

	UINT GetLVL() { return baseLVL; }

	virtual void Update() = 0;
	virtual void Render() = 0;

	virtual void CreateCharacter() = 0;

	bool IncludeOtherDiscrimToRect(Vector3 point);	//아군진지내에 적이 들어왔나?

public:	//set, get용
	Vector3 GetPos() { return position; }
	UINT GetCharacterCount() { return summonCharacters; }

};
