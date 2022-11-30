#include <DxLib.h>
#include <algorithm>
#include "SoundMng.h"
#include <cassert>
#include "../../../_debug/_DebugConOut.h"


const VecInt& SoundMng::GetID(std::string key)
{
	if (soundMap_.count(key) == 0)//����ǂݍ���ł��邩�ǂ���
	{
		//�ǂݍ���łȂ��ꍇ�͓ǂݍ���
		soundMap_[key].resize(1);
		soundMap_[key][0] = LoadSoundMem(key.c_str());
		assert(soundMap_[key][0] >= 0);
	}
	return soundMap_[key];//���̃n���h����n��
}

void SoundMng::SetDefaultVolume(void)
{
	for (auto& sound : soundMap_)
	{
		soundVolumeMap_[sound.first] = GetVolumeSoundMem2(sound.second[0]);
	}
}

void SoundMng::PlayingSound(std::string name)
{
	if (CheckSoundMem(lpSoundMng.GetID(name)[0]) != 0)
	{
		StopSoundMem(lpSoundMng.GetID(name)[0]);
	}
	PlaySoundMem(lpSoundMng.GetID(name)[0], DX_PLAYTYPE_BACK);
}

SoundMng::SoundMng()
{
}

SoundMng::~SoundMng()
{
}

void SoundMng::ChangeVolume(int Vol, std::string name)
{
	ChangeVolumeSoundMem(Vol, GetID(name)[0]);
}

void SoundMng::AllChangeVolume(int Vol)
{
	for (auto& sound : soundVolumeMap_)
	{
		auto s = sound.second;
		Vol = std::clamp(Vol, -100 + sound.second, 100 + sound.second);
		Vol = std::clamp(Vol,0,255);
		ChangeVolume(Vol, sound.first);
	}
}

void SoundMng::SetAllVolume(int Vol)
{
	allVolume_ = Vol;
}

int SoundMng::GetAllVolume()
{
	return allVolume_;
}

void SoundMng::Play3DSound(std::string path,Vector3 pos,float distance)
{
	//���̍Đ�
	/*lpSoundMng.PlayingSound("./resource/sound/test.wav");*/
	PlaySoundMem(lpSoundMng.GetID(path)[0], DX_PLAYTYPE_LOOP);
	//���̍��W�ݒ�
	Set3DPositionSoundMem(pos.toVECTOR(), lpSoundMng.GetID(path)[0]);
	// �T�E���h���͂�( ������������ )�͈͂�ݒ�
	Set3DRadiusSoundMem(distance, lpSoundMng.GetID(path)[0]);
}