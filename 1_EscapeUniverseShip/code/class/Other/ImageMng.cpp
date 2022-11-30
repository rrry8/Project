#include<DxLib.h>
#include<algorithm>
#include "ImageMng.h"
#include"../../_debug/_DebugConOut.h"

const VecInt& ImageMng::GetID(std::string key)
{
	//�摜��ǂݍ���ł��邩�̃`�F�b�N
	if (!imageMap_.count(key))
	{
		//�ǂݍ���ł��Ȃ��ꍇ���T�C�Y���ĉ摜�����[�h����
		imageMap_[key].resize(1);
		imageMap_[key][0] = LoadGraph(key.c_str());
	}
	return imageMap_[key];
}

const VecInt& ImageMng::GetID(std::string f_name, std::string key, Vector2 divSize, Vector2 divCnt)
{
	if (!imageMap_.count(key))
	{
		imageMap_[key].resize(static_cast<UINT>(divCnt.x * divCnt.y));	//static_cast<UINT>

		if (LoadDivGraph(f_name.c_str(),
			divCnt.x * divCnt.y,
			divCnt.x, divCnt.y,
			divSize.x, divSize.y,
			&imageMap_[key][0],
			true) == -1)
		{
			TRACE("���s");
		}
	}
	return imageMap_[key];

}

const VecInt& ImageMng::GetID(std::string f_name, std::string key)
{
	if (!imageMap_.count(key))
	{
		//�ǂݍ���ł��Ȃ��ꍇ���T�C�Y���ĉ摜�����[�h����
		imageMap_[key].resize(1);
		imageMap_[key][0] = LoadGraph(f_name.c_str());
	}
	return imageMap_[key];
}

const VecInt& ImageMng::GetmapID(std::string key)
{
	//�摜��ǂݍ���ł��邩�̃`�F�b�N
	if (!imageMap2_.count(key))
	{
		//�ǂݍ���ł��Ȃ��ꍇ���T�C�Y���ĉ摜�����[�h����
		imageMap2_[key].resize(1);
		imageMap2_[key][0] = LoadGraph(key.c_str());
	}
	return imageMap2_[key];
}

const VecInt& ImageMng::GetmapID(std::string f_name, std::string key, Vector2 divSize, Vector2 divCnt)
{
	if (!imageMap2_.count(key))
	{
		imageMap2_[key].resize(static_cast<UINT>(divCnt.x * divCnt.y));	//static_cast<UINT>

		if (LoadDivGraph(f_name.c_str(),
			divCnt.x * divCnt.y,
			divCnt.x, divCnt.y,
			divSize.x, divSize.y,
			&imageMap2_[key][0],
			true) == -1)
		{
			TRACE("���s");
		}
	}
	return imageMap2_[key];
}

const VecInt& ImageMng::GetmapID(std::string f_name, std::string key)
{
	if (!imageMap2_.count(key))
	{
		//�ǂݍ���ł��Ȃ��ꍇ���T�C�Y���ĉ摜�����[�h����
		imageMap2_[key].resize(1);
		imageMap2_[key][0] = LoadGraph(f_name.c_str());
	}
	return imageMap2_[key];
}

ImageMng::ImageMng()
{
}

ImageMng::~ImageMng()
{
}
