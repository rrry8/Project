#include<EffekseerForDXLib.h>
#include "EffectMng.h"

const VecInt& EffectMng::GetSizeID(std::string key, float size)
{
	//�摜��ǂݍ���ł��邩�̃`�F�b�N
	if (!effSizeMap_.count(key))
	{
		//�ǂݍ���ł��Ȃ��ꍇ���T�C�Y���ĉ摜�����[�h����
		effSizeMap_[key].resize(1);
		effSizeMap_[key][0] = LoadEffekseerEffect(key.c_str(),size);
	}
	return effSizeMap_[key];
}

const VecInt& EffectMng::GetPlayID(std::string key,float size)
{
	if (!effPlayMap_.count(key))
	{
		effPlayMap_[key].resize(1);
		effPlayMap_[key][0] = PlayEffekseer2DEffect(GetSizeID(key.c_str(), size)[0]);
	}
	return effPlayMap_[key];
}

const void EffectMng::SetPlayID(std::string key)
{
	if (effPlayMap_.count(key))
	{
		effPlayMap_[key][0] = PlayEffekseer2DEffect(effSizeMap_[key][0]);
	}
}

const void EffectMng::SetSizeID(std::string key,float size)
{
	if (!effSizeMap_.count(key))
	{
		effSizeMap_[key].resize(1);
		effSizeMap_[key][0] = LoadEffekseerEffect(key.c_str(), size);
	}
}

EffectMng::EffectMng()
{
}

EffectMng::~EffectMng()
{
}
