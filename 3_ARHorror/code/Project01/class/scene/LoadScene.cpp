#include "LoadScene.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "../common/manager/ImageMng.h"
#include "../common/manager/ModelMng.h"
#include "../common/manager/MovieMng.h"
#include "../common/manager/SoundMng.h"

LoadScene::LoadScene()
{
	Init();
	DrawScreen();
}

LoadScene::~LoadScene()
{
}

void LoadScene::Init(void)
{
	LoadVol = 0;
	IsLoadingEnd = false;
	Load();
}

UniqueScene LoadScene::Update(UniqueScene scene)
{
	//�i�݋�̍X�V
	UpdateProgress();
	
	//���[�h���I�������
	if (IsLoadingEnd)
	{
		return std::move(std::make_unique<GameScene>());
	}
	DrawScreen();
	return scene;
}

void LoadScene::DrawScreen(void)
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	DrawFormatString(0, 0, 0xffffff, "LoadScene");
	//DrawFormatString(0, 1000, 0xffffff, "vol %d", LoadVol);
	//DrawFormatString(0, 1050, 0xffffff, "num %d", GetASyncLoadNum());

	//���`��
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	DrawBox(0,0, 1920, 1080,
		0x333333,
		true);

	DrawBox(lpSceneMng.GetScreenSize().x / 2 - 500,
		lpSceneMng.GetScreenSize().y / 2 - 50,
		lpSceneMng.GetScreenSize().x / 2 + 500,
		lpSceneMng.GetScreenSize().y / 2 + 50,
		0x555555,
		false);
	DrawBox(lpSceneMng.GetScreenSize().x / 2 - 500,
		lpSceneMng.GetScreenSize().y / 2 - 50,
		lpSceneMng.GetScreenSize().x / 2 - 500 + LoadVol,
		lpSceneMng.GetScreenSize().y / 2 + 50,
		0x881100,
		true);
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
}

void LoadScene::Release(void)
{
}

void LoadScene::Load()
{
	SetUseASyncLoadFlag(true);
	lpImageMng.GetID("resource/image/minCamera.png");
	lpImageMng.GetID("resource/image/Font/gameover.png");
	lpModelMng.GetID("./resource/model/Item/old_key.mv1",5);
	lpModelMng.GetID("resource/model/stage/map2.mv1");
	lpModelMng.GetID("resource/model/stage/Coll3.mv1");

	std::string gimmickPath = "./resource/model/gimmick/";
	//��-------------------
	//��
	lpModelMng.GetID(gimmickPath + "Door_White1.mv1", 25);
	lpModelMng.GetID(gimmickPath + "Door_White1_Double.mv1",25);
	lpModelMng.GetID(gimmickPath + "Door_White2.mv1",25);
	lpModelMng.GetID(gimmickPath + "Door_White2_Double.mv1",25);
	lpModelMng.GetID(gimmickPath + "Door_White3.mv1",25);
	lpModelMng.GetID(gimmickPath + "Door_White3_Double.mv1",25);
	//��
	lpModelMng.GetID(gimmickPath + "Door_Red1.mv1",25);
	lpModelMng.GetID(gimmickPath + "Door_Red2.mv1",25);
	//��
	lpModelMng.GetID(gimmickPath + "Door_Dark1.mv1",25);
	lpModelMng.GetID(gimmickPath + "Door_Dark1_Double.mv1",25);
	lpModelMng.GetID(gimmickPath + "Door_Dark2.mv1",25);
	lpModelMng.GetID(gimmickPath + "Door_Dark2_Double.mv1",25);
	//�����o��
	lpModelMng.GetID(gimmickPath + "drawer_frame_L.mv1", 25);
	lpModelMng.GetID(gimmickPath + "drawer.mv1", 25);
	//�_�C�A��
	lpModelMng.GetID(gimmickPath + "Dial.mv1", 25);
	//�g
	lpModelMng.GetID(gimmickPath + "Dial_Frame.mv1", 25);

	//AR�}�[�J�[
	lpModelMng.GetID("./resource/model/marker/redMarker.mv1");
	lpModelMng.GetID("./resource/model/marker/blueMarker.mv1");
	lpModelMng.GetID("./resource/model/marker/greenMarker.mv1");
	lpModelMng.GetID("./resource/model/marker/orangeMarker.mv1");
	lpModelMng.GetID("./resource/model/marker/purpleMarker.mv1");


	SetUseASyncLoadFlag(false);

	// 3D�T�E���h�̓ǂݍ���
	SetCreate3DSoundFlag(true);
	lpSoundMng.GetID("./resource/sound/WalkVoice.mp3");
	SetCreate3DSoundFlag(false);
}

void LoadScene::UpdateProgress()
{
	//���Ŏ��Ԍo�߂ŏ���ɏオ���Ă���
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	//TRACE("�񓯊��ǂݍ��݂̐�:%d\n", GetASyncLoadNum());
	if (GetASyncLoadNum() == 0)//�񓯊��ǂݍ��݂̎c�������������ׂ�
	{
		//�O�Ȃ�
		//TRACE("�񓯊��ǂݍ��ݏI��\n");
		if (LoadVol >= 1000)
		{
			IsLoadingEnd = true;//���[�h�I��
		}
		else//�o�[���}�b�N�X�ɂ���
		{
			LoadVol += 30;
			if (LoadVol > 1000)
			{
				LoadVol = 1000;
			}
		}
	}
	else
	{
		IsLoadingEnd = false;
		//���[�h�̐i�����\�����Ă�
		if (LoadVol < 950)
		{
			LoadVol += 1;

		}
	}
	//�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
}
