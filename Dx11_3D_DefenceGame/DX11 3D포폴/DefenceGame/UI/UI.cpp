#include "Framework.h"

UI* UI::instance = nullptr;

UI::UI()
{
	renderUI.clear();
}

UI::~UI()
{
	renderUI.clear();
	renderUI.shrink_to_fit();
}

Render2D* UI::PushUI(Vector2 pos, Vector2 scale, ID3D11ShaderResourceView* srv, wstring text)
{
	Render2D* returnData = new Render2D();

	returnData->scale = scale;
	returnData->position = pos;
	returnData->SetSRV(srv);

	returnData->SetText(pos, scale, text);
	
	renderUI.emplace_back(returnData);

	return returnData;
}

void UI::Update()
{
	isMouseCollision = false;

	for (Render2D* iter : renderUI)
	{
		if (iter->IsMouseCollision())
			isMouseCollision = true;

		iter->Update();
	}
}

void UI::PostRender()
{
	WRITE->GEtDC()->BeginDraw();

	for (Render2D* iter : renderUI)
		iter->Render();

	WRITE->GEtDC()->EndDraw();

	//VP->SetProjection(Environment::Get()->GetPerspective()->GetMatrix());
}
