#pragma once

class Button
{
private:
	Image* img;
	Rect* rc;

	D3DXVECTOR2 imgScale, colBoxScale, imgPos, colBoxPos;
	string imgName;

public:
	Button(string name, D3DXVECTOR2 pos, D3DXVECTOR2 scale);
	Button(string name, D3DXVECTOR2 pos, D3DXVECTOR2 imgScale, D3DXVECTOR2 colBoxScale);
	~Button();

	// GameNode을(를) 통해 상속됨
	void Init();
	void Release();
	void Update();
	void Render();
	void RenderFrame(int x, int y);

	void SetImgScale(D3DXVECTOR2 scale) { imgScale = scale; }
	void SetColBoxScale(D3DXVECTOR2 scale) { colBoxScale = scale; }
	void SetImgName(string name) { imgName = name; }
	void SetImgPosition(D3DXVECTOR2 pos) { imgPos = pos; }
	void SetColBoxPosition(D3DXVECTOR2 pos) { colBoxPos = pos; }
	void SetPosition(D3DXVECTOR2 pos)
	{
		imgPos = pos;
		colBoxPos = pos;
	}

	D3DXVECTOR2	GetImgScale() { return imgScale; }
	D3DXVECTOR2	GetColBoxScale() { return colBoxScale; }
	D3DXVECTOR2	GetImgPosition() { return imgPos; }
	D3DXVECTOR2	GetColBoxPosition() { return colBoxPos; }
	string GetImgName() { return imgName; }
	FRECT GetRect() { return rc->GetRect(); }
	D3DXMATRIX* GetRT() { return img->GetRT(); }
	

	bool IsKeyDown(int key);
	bool StayKeyDown(int key);
	bool MouseInColBox();
};

