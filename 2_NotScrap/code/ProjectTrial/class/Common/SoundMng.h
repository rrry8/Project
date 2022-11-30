#pragma once
#include <vector>
#include<string>
#include <map>

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
	
	//���ʂ̕ύX
	void ChangeVolume(int Vol, std::string name);

	//����炷
	void PlayingSound(std::string name);
private:
	SoundMng();
	~SoundMng();

	//�}�b�v
	std::map<std::string, VecInt> soundMap_;
};

