#include "Item.h"
using namespace std;

Item::Item()
{
}

Item::~Item()
{
}

void Item::LoadBinData(std::vector<ItemObj>& iList, const char* bData)
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
			iList.emplace_back(NULL, name, pos, rot);
		}
	}
	//ファイルを閉じる
	FileRead_close(file);
}

std::wstring Item::ConvertToWstring(std::string& str)
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

void Item::LoadModel(std::vector<ItemObj>& iList, std::wstring objName, const char* mPath)
{
	wstring wstr;
	for (auto& items : iList)
	{
		//バイナリデータのオブジェクト名をwstringに変換して代入
		wstr = ConvertToWstring(items.objName_);
		//指定したオブジェクト名と同じなら		
		if (wstr == objName)
		{
			//指定したモデルをロードする
			items.path_ = MV1LoadModel(mPath);
		}
	}
}


