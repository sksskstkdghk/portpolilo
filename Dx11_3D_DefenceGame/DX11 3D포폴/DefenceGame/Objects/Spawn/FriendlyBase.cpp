#include "Framework.h"

FriendlyBase::FriendlyBase()
	:SpawnBase()
{
	tex = Texture::Add(L"Textures/MagicCircle/red.png");
	shader = Shader::Add(L"MagicCircle");

	time = 0.0f;

	summonData.name = "pikachu";
	summonData.summonTime = 10.0f;
}

FriendlyBase::~FriendlyBase()
{

}

void FriendlyBase::Update()
{
	UpdateWorld();
	CreateCharacter();
	SetBattle();

	EraseSummonData();
}

void FriendlyBase::Render()
{
	SetDraw();
}

void FriendlyBase::CreateCharacter()
{
	if (time > 0.0f)
	{
		time -= DELTA;
		return;
	}

	if (summonCharacters >= 3)
		return;

	CreateFriendly();
}

void FriendlyBase::CreateFriendly()
{
	if (summonData.name == "pikachu")
	{
		vector<Vector3> path;
		useChar.push_back(CHARACTER->AddFriendly("pikachu", baseLVL, position, path));
		summonCharacters++;

		time = summonData.summonTime;
	}
}

void FriendlyBase::SetBattle()
{
	bool check = false;

	for (auto iter = CHARACTER->GetEnemys()->begin(); iter != CHARACTER->GetEnemys()->end(); iter++)
	{
		for (UINT i = 0; i < iter->second->GetDataSize(); i++)
		{
			if (!iter->second->IsAlive(i))
				continue;

			if (IncludeOtherDiscrimToRect(iter->second->GetPos(i)))
			{
				if (summonCharacters == 0)
				{
					iter->second->SetIDLE(i);
					check = false;
				}
				else
				{
					for (int j = 0; j < useChar.size(); j++)
						CHARACTER->FindNameFriendly(useChar[j].name)->SetBattle(useChar[j].instanceID);

					iter->second->SetBattle(i);

					check = true;
				}
			}
		}
	}

	if (!check)
	{
		for (int j = 0; j < useChar.size(); j++)
		{
			CHARACTER->FindNameFriendly(useChar[j].name)->SetIDLE(useChar[j].instanceID);
		}
	}
}

void FriendlyBase::EraseSummonData()
{
	if (useChar.size() < 0)
		return;

	for (UINT i = 0; i < useChar.size(); i++)
	{
		Character* cursor = nullptr;

		if (CHARACTER->FindNameFriendly(useChar[i].name) != nullptr)
		{
			cursor = CHARACTER->FindNameFriendly(useChar[i].name);

			if (!cursor->IsAlive(useChar[i].instanceID))
			{
				useChar.erase(useChar.begin() + i);
				summonCharacters--;

				continue;
			}
		}
	}
}
