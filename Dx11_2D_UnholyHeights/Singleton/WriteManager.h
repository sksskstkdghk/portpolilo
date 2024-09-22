#pragma once
#include "SingletonBase.h"
//클래스(구조체) 전방선언
struct FontBrushDesc;
struct FontTextDesc;

class WriteManager : public SingletonBase<WriteManager>
{
private:
	//dx11에선 텍스트를 출력할수 없기 때문에 d2d와 dxwrite를 불러와서 출력해야 한다

	//d2d와 dxwrite 의 컴객체들
	ID2D1Factory1 * factory;//Direct2D 리소스를 만드는 역할
	IDWriteFactory* writeFactory = NULL;//DirectWrite 개체를 만드는 데 사용

	ID2D1Device* device;//d2d 디바이스
	ID2D1DeviceContext* deviceContext = NULL;//d2d 디바이스컨텍스트

	ID2D1Bitmap1* targetBitmap; //그려줄 비트맵 ID3D11Texture2D 같은역할이라 보면 됨
	vector<FontBrushDesc> fontBrush; //폰트 브러시 모음
	vector<FontTextDesc> fontText; //폰트 텍스트 모음
public:
	void RenderText
	(
		wstring text
		, RECT rect
		, float size = 20.0f
		, wstring font = L"돋움체"
		, D3DXCOLOR color = D3DXCOLOR(1, 1, 1, 1)
		, DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_NORMAL
		, DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL
		, DWRITE_FONT_STRETCH stretch = DWRITE_FONT_STRETCH_NORMAL
	);

public:
	WriteManager();
	~WriteManager();
	ID2D1DeviceContext* GetDc() { return deviceContext; }
};


struct FontBrushDesc
{
	D3DXCOLOR Color;
	ID2D1SolidColorBrush* Brush = NULL;//HBRUSH 같은놈
	//같은놈이 있는지 확인하기 위해 연산자 오버로딩
	bool operator==(const FontBrushDesc& val)
	{
		return Color.r == val.Color.r &&
			Color.g == val.Color.g &&
			Color.b == val.Color.b &&
			Color.a == val.Color.a;
	}
};

struct FontTextDesc
{
	wstring Font;
	float FontSize;

	DWRITE_FONT_WEIGHT Weight;
	DWRITE_FONT_STYLE Style;
	DWRITE_FONT_STRETCH Stretch;

	IDWriteTextFormat* Format = NULL;//HFONT 같은놈
	//같은놈이 있는지 확인하기 위해 연산자 오버로딩
	bool operator==(const FontTextDesc& val)
	{
		bool b = true;
		b &= Font == val.Font;
		b &= FontSize == val.FontSize;
		b &= Weight == val.Weight;
		b &= Style == val.Style;
		b &= Stretch == val.Stretch;

		return b;
	}
};