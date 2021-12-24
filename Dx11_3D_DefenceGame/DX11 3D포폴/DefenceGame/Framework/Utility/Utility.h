#pragma once

class Utility
{
public:
	static wstring ToWString(string value);
	static string ToString(wstring value);

	static vector<string> SplitString(string origin, string tok);
	static void Replace(OUT string* str, IN string comp, IN string rep);

	static bool IsExistDirectory(string path);
	static bool IsExistFile(string path);

	static void CreateFolders(string path, bool isFileName = false);	

	static string GetFileName(string path);
	static string GetExtention(string fileName);
	static string GetDirectoryName(string path);
};