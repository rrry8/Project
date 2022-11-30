#include<DxLib.h>
#include "SoundMng.h"

const VecInt& SoundMng::GetID(std::string key)
{
	//�摜��ǂݍ���ł��邩�̃`�F�b�N
	if (!soundMap_.count(key))
	{
		//�ǂݍ���ł��Ȃ��ꍇ���T�C�Y���ĉ摜�����[�h����
		soundMap_[key].resize(1);
		soundMap_[key][0] = LoadSoundMem(key.c_str());
	}
	return soundMap_[key];
}

SoundMng::SoundMng()
{
}

SoundMng::~SoundMng()
{
}
