#pragma once

struct FontBrushDesc
{
	Color color;
	ID2D1SolidColorBrush* brush;

	bool operator==(const FontBrushDesc& val)
	{
		return color == val.color;
	}
};

struct FontTextDesc
{
	wstring font;
	float fontSize;

	DWRITE_FONT_WEIGHT weight;
	DWRITE_FONT_STYLE style;
	DWRITE_FONT_STRETCH stretch;

	IDWriteTextFormat* format;

	bool operator==(const FontTextDesc& val)
	{
		bool b = true;
		b &= font == val.font;
		b &= fontSize == val.fontSize;
		b &= weight == val.weight;
		b &= style == val.style;
		b &= stretch == val.stretch;

		return b;
	}
};

class DirectWrite
{
private:
	ID2D1Factory1* factory;
	IDWriteFactory* writeFactory;

	ID2D1Device* device;
	ID2D1DeviceContext* context;

	ID2D1Bitmap1* targetBitmap;

	vector<FontBrushDesc> fontBrush;
	vector<FontTextDesc> fontText;

	static DirectWrite* instance;

	DirectWrite();
	~DirectWrite();

public:
	static DirectWrite* Get() { return instance; }

	static void Create() { instance = new DirectWrite(); }
	static void Delete() { delete instance; }

	void RenderText(wstring text, RECT rect,
		float size = 20.0f, wstring font = L"µ¸¿òÃ¼", Color color = Color(1, 1, 1, 1),
		DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH stretch = DWRITE_FONT_STRETCH_NORMAL);

	ID2D1DeviceContext* GEtDC() { return context; }
};