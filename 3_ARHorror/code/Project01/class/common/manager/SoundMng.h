#pragma once
#include <vector>
#include<string>
#include <map>
#include "../Geometry.h"

#define lpSoundMng SoundMng::GetInstance()

using VecInt = std::vector<int>;

class SoundMng
{
public:
	static SoundMng& GetInstance()
	{
		static SoundMng s_Instance;
		return s_Instance;
	}

	const VecInt& GetID(std::string key);
	
	//音量の変更
	void ChangeVolume(int Vol, std::string name);
	void AllChangeVolume(int Vol);
	void SetAllVolume(int Vol);
	int  GetAllVolume();
	void SetDefaultVolume(void);
	//音を鳴らす
	void PlayingSound(std::string name);

	void Play3DSound(std::string path, Vector3 pos, float distance);
private:
	SoundMng();
	~SoundMng();

	//マップ
	std::map<std::string, VecInt> soundMap_;
	std::map<std::string, int> soundVolumeMap_;

	int allVolume_;
};

