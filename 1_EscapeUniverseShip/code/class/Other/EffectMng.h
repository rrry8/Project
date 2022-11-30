#pragma once
#include<vector>
#include<string>
#include<map>
#include"../../Geometry.h"

#define lpEffMng EffectMng::GetInstance()

using VecInt = std::vector<int>;

class EffectMng
{
public:
	static EffectMng& GetInstance()
	{
		static EffectMng s_instance;
		return s_instance;
	}

	//�T�C�Y���擾
	const VecInt& GetSizeID(std::string key,float size);
	//�Đ��擾
	const VecInt& GetPlayID(std::string key, float size);

	//�Đ��Z�b�g
	const void SetPlayID(std::string key);
	//�T�C�Y�Z�b�g
	const void SetSizeID(std::string key,float size);

private:
	EffectMng();
	~EffectMng();
	std::map<std::string, VecInt> effSizeMap_;
	std::map<std::string, VecInt> effPlayMap_;
};

