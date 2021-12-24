#pragma once

class UI
{
private:
	vector<Render2D*> renderUI;
	bool isMouseCollision;

	static UI* instance;

	UI();
	~UI();
public:
	static void Create() { instance = new UI(); }
	static void Delete() { delete instance; }
	static UI* Get() { return instance; }

	void Clear() { renderUI.clear(); }
	Render2D* PushUI(Vector2 pos, Vector2 scale, ID3D11ShaderResourceView* srv = nullptr, wstring text = L"");
	bool GetMouseCol() { return isMouseCollision; }

	void Update();
	void PostRender();
};