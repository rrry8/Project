#include "Gimmick.h"

using namespace std;


Gimmick::Gimmick()
{
}

Gimmick::~Gimmick()
{
}

void Gimmick::LoadBinData(std::vector<GimmickObj>& gList, const char* bData)
{
	int file = FileRead_open(bData);
	int version;
	FileRead_read(&version, sizeof(version), file);
	if (version == 6)
	{
		while (!FileRead_eof(file))
		{
			std::string name;	//名前
			Vector3 pos;	//座標
			Vector3 rot;	//回転
			char nameSize;
			//ハンドル、名前、座標、回転情報読み取り
			FileRead_read(&nameSize, sizeof(nameSize), file);
			name.resize(static_cast<int>(nameSize));
			FileRead_read(&name[0], nameSize, file);
			FileRead_read(&pos, sizeof(pos), file);
			FileRead_read(&rot, sizeof(rot), file);
			//ハンドル、名前、座標、回転情報を格納
			gList.emplace_back(name, pos, rot);
		}
	}
	//ファイルを閉じる
	FileRead_close(file);
}

void Gimmick::LoadBinData(std::vector<GimmickObj>& gList, const char* bData, std::string objName)
{
	int file = FileRead_open(bData);
	int version;
	FileRead_read(&version, sizeof(version), file);
	if (version == 6)
	{
		while (!FileRead_eof(file))
		{
			std::string name;	//名前
			Vector3 pos;	//座標
			Vector3 rot;	//回転
			char nameSize;
			//ハンドル、名前、座標、回転情報読み取り
			FileRead_read(&nameSize, sizeof(nameSize), file);
			name.resize(static_cast<int>(nameSize));
			FileRead_read(&name[0], nameSize, file);
			FileRead_read(&pos, sizeof(pos), file);
			FileRead_read(&rot, sizeof(rot), file);
			if (objName != name)
			{
				continue;
			}
			//名前、座標、回転情報を格納
			gList.emplace_back(name, pos, rot);
		}
	}
	//ファイルを閉じる
	FileRead_close(file);
}

std::wstring Gimmick::ConvertToWstring(std::string& str)
{
	auto cnt = MultiByteToWideChar(
		CP_UTF8,
		0,
		str.c_str(),
		str.length(),
		nullptr,
		0
	);

	wstring wstr;
	wstr.resize(cnt);

	MultiByteToWideChar(
		CP_UTF8,
		0,
		str.c_str(),
		str.length(),
		&wstr[0],
		wstr.length()
	);

	return wstr;
}

//void Gimmick::LoadModel(std::vector<GimmickObj>& gList,std::wstring objName, const char* mPath)
//{
//	wstring wstr;
//	for (auto& gimmick : gList)
//	{
//		//バイナリデータのオブジェクト名をwstringに変換して代入
//		wstr = ConvertToWstring(gimmick.objName_);
//		//指定したオブジェクト名と同じなら		
//		if (wstr == objName)
//		{
//			//指定したモデルをロードする
//			gimmick.path_ = MV1LoadModel(mPath);
//		}
//	}
//}

//bool Gimmick::CheckObjName(std::vector<GimmickObj>& gList, std::wstring objName)
//{
//	wstring wstr;
//	for (auto& gimmick : gList)
//	{
//		//バイナリデータのオブジェクト名をwstringに変換して代入
//		wstr = ConvertToWstring(gimmick.objName_);
//		//指定したオブジェクト名と同じなら		
//		if (wstr == objName)
//		{
//			//指定したモデルをロードする
//		}
//	}
//}
