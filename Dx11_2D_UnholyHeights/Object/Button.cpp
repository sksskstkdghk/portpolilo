#include "stdafx.h"
#include "Button.h"


Button::Button(string name, D3DXVECTOR2 pos, D3DXVECTOR2 scale)
{
	imgScale = scale;
	colBoxScale = scale;
	imgPos = pos;
	colBoxPos = pos;
	imgName = name;

	Init();
}

Button::Button(string name, D3DXVECTOR2 pos, D3DXVECTOR2 imgScale, D3DXVECTOR2 colBoxScale)
{
	this->imgScale = imgScale;
	this->colBoxScale = colBoxScale;
	imgPos = pos;
	colBoxPos = pos;
	imgName = name;

	Init();
}


Button::~Button()
{
}

void Button::Init()
{
	img = IMGMANAGER->GetImage(imgName);
	img->Scale(imgScale);
	img->Position(imgPos);

	rc = new Rect();
	rc->Position(colBoxPos);
	rc->Scale(colBoxScale);
	rc->SetisFill(false);
}

void Button::Release()
{
	SAFE_DELETE(rc);
}

void Button::Update()
{
	rc->Position(colBoxPos);
	rc->Scale(colBoxScale);
	rc->Update();
}

void Button::Render()
{
	img = IMGMANAGER->GetImage(imgName);
	img->Scale(imgScale);
	img->Position(imgPos);

	img->Render();
	//rc->Render();
}

void Button::RenderFrame(int x, int y)
{
	img = IMGMANAGER->GetImage(imgName);
	img->SetCurrentFrameX(x);
	img->SetCurrentFrameY(y);
	img->Scale(imgScale);
	img->Position(imgPos);

	img->Render();
	//rc->Render();
}

bool Button::IsKeyDown(int key)
{
	if (MouseInColBox())
	{
		return KEYMANAGER->isKeyDown(key);
	}

	return false;
}

bool Button::StayKeyDown(int key)
{
	if (MouseInColBox())
	{
		return KEYMANAGER->StayKeyDown(key);
	}

	return false;
}

bool Button::MouseInColBox()
{
	if (PtInRect(rc->GetRect(), g_ptMouse))
		return true;

	return false;
}
