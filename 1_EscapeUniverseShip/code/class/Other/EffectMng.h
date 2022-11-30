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

	//サイズを取得
	const VecInt& GetSizeID(std::string key,float size);
	//再生取得
	const VecInt& GetPlayID(std::string key, float size);

	//再生セット
	const void SetPlayID(std::string key);
	//サイズセット
	const void SetSizeID(std::string key,float size);

private:
	EffectMng();
	~EffectMng();
	std::map<std::string, VecInt> effSizeMap_;
	std::map<std::string, VecInt> effPlayMap_;
};

