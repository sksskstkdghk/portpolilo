#include "Framework.h"

wstring Utility::ToWString(string value)
{
	wstring temp = L"";
	temp.assign(value.begin(), value.end());

	return temp;
}

string Utility::ToString(wstring value)
{
	string temp = "";
	temp.assign(value.begin(), value.end());

	return temp;
}

vector<string> Utility::SplitString(string origin, string tok)
{
	vector<string> result;

	int cutAt = 0;
	while ((cutAt = (int)origin.find_first_of(tok)) != origin.npos)
	{
		if (cutAt > 0)
			result.push_back(origin.substr(0, cutAt));

		origin = origin.substr(cutAt + 1);
	}

	if (origin.length() > 0)
		result.push_back(origin.substr(0, cutAt));

	return result;
}

void Utility::Replace(OUT string* str, IN string comp, IN string rep)
{
	string temp = *str;

	size_t startPos = 0;

	while ((startPos = temp.find(comp, startPos)) != string::npos)
	{
		temp.replace(startPos, comp.length(), rep);
		startPos += rep.length();
	}

	*str = temp;
}

bool Utility::IsExistDirectory(string path)
{
	DWORD attribute = GetFileAttributesA(path.c_str());

	BOOL temp = (attribute != INVALID_FILE_ATTRIBUTES && (attribute & FILE_ATTRIBUTE_DIRECTORY));

	return temp == TRUE;
}

bool Utility::IsExistFile(string path)
{
	DWORD attribute = GetFileAttributesA(path.c_str());

	return attribute < 0xffffffff;
}

void Utility::CreateFolders(string path, bool isFileName)
{
	vector<string> folders = SplitString(path, "/");

	if (isFileName)
		folders.pop_back();

	string temp = "";
	for (string folder : folders)
	{
		temp += folder + "/";

		if (!IsExistDirectory(temp))
			CreateDirectoryA(temp.c_str(), 0);
	}
}

string Utility::GetFileName(string path)
{
	Replace(&path, "\\", "/");
	size_t index = path.find_last_of('/');

	string fileName = path.substr(index + 1, path.length());

	string extention = GetExtention(fileName);

	if (extention != "png")
	{
		size_t index = fileName.find_last_of('.');

		fileName.replace(index + 1, 3, "png");
	}

	return fileName;
}

string Utility::GetExtention(string fileName)
{
	size_t index = fileName.find_last_of('.');

	return fileName.substr(index + 1, fileName.length());
}

string Utility::GetDirectoryName(string path)
{
	size_t index = path.find_last_of('/');

	return path.substr(0, index + 1);
}
