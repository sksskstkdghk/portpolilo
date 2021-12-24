#pragma once
#include "SingletonBase.h"

class ImageManager :public SingletonBase<ImageManager>
{
private:
	typedef map<string, Image*>				ImgMap;//이미지 목록
	typedef map<string, Image*>::iterator	ImgIter;//이미지 목록의 반복자

	ImgMap m_ImgMap;
public:
	
	Image* AddImage(string strKey, _tstring file, string ps = "PS", string vs = "VS");
	Image* AddImage(string strKey, _tstring file, D3DXVECTOR2 leftDown, D3DXVECTOR2 rightUp, string ps = "PS", string vs = "VS");
	Image* AddImage(string strKey, _tstring file, UINT MaxFrameX, UINT MaxFrameY, string ps = "PS");

	//이미지 키값으로 찾기
	Image* GetImage(string strkey);
	//이미지 키값으로 삭제
	bool DeleteImage(string strKey);
	//전부 삭제
	void DeleteAll();
	//일반렌더
	void Render(string strKey);
	//루프 렌더
	void LoofRender(string strKey,float x = 0.0f,float y=0.0f);

	//애니메이션
	void PlayAnimation(string strKey,PlayState ps, bool isFrameX, float nexttime);
};

