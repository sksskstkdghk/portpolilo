#pragma once

class Program
{
private:
	vector<Scene*> scenes;

	short isRender;
public:
	Program();
	~Program();

	void Update();

	void PreRender();
	void Render();
	void PostRender();
};