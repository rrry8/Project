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
			std::string name;	//���O
			Vector3 pos;	//���W
			Vector3 rot;	//��]
			char nameSize;

			//�n���h���A���O�A���W�A��]���ǂݎ��
			FileRead_read(&nameSize, sizeof(nameSize), file);
			name.resize(static_cast<int>(nameSize));
			FileRead_read(&name[0], nameSize, file);
			FileRead_read(&pos, sizeof(pos), file);
			FileRead_read(&rot, sizeof(rot), file);

			//�n���h���A���O�A���W�A��]�����i�[
			iList.emplace_back(NULL, name, pos, rot);
		}
	}
	//�t�@�C�������
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
		//�o�C�i���f�[�^�̃I�u�W�F�N�g����wstring�ɕϊ����đ��
		wstr = ConvertToWstring(items.objName_);
		//�w�肵���I�u�W�F�N�g���Ɠ����Ȃ�		
		if (wstr == objName)
		{
			//�w�肵�����f�������[�h����
			items.path_ = MV1LoadModel(mPath);
		}
	}
}


