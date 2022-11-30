#include <DxLib.h>
#include<EffekseerForDXLib.h>
#include <memory>
#include <chrono>
#include "../../_debug/_DebugConOut.h"
#include "../../_debug/_DebugDispOut.h"
#include "TitleScene.h"
#include "SelectScene.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "../Common/Fps.h"
#include "../Common/ImageMng.h"
#include "../UI/SceneUIManager.h"
#include "ResultScene.h"
#include "LoadScene.h"
#include "../Input/KeyInput.h"
#include "../Input/PadInput.h"
#include "../Input/ControllerMng.h"
#include"../../source/image/icon/icon.h"

// ��ʃT�C�Y
constexpr int ScreenSizeX = 1920;
constexpr int ScreenSizeY = 1080;

//�c�܂������͈�
constexpr float DISTORTION_SIZE_X = 500; //x
constexpr float DISTORTION_SIZE_Y = 500; //y

//�X�N���[���T�C�Y
constexpr float SCREEN_SIZE_X = 1920.0f; //x
constexpr float SCREEN_SIZE_Y = 1080.0f; //y


void SceneManager::Run(void)
{
	if (!initFlg_)
	{
		if (!SysInit())
		{
			return;
		}
	}
	gameEndFlg = false;		//�Q�[�����[�v�I���t���O

	if (GetJoypadNum())
	{
		lpCntMng.SetUseController(CntType::Pad);
	}
	else
	{
		lpCntMng.SetUseController(CntType::Key);
	}
	lpCntMng.GetController()->KeyConfig_SetDefault();;//�Q�[���J�n���O�ɃL�[�R���t�B�O�����Z�b�g�iclick�A�łŐ��l���Ԃ����ŃG���[�ɂȂ������p�j
	lpCntMng.GetController()->KeyConfig_Save();//���Z�b�g�I����.txt�ɃZ�[�u����B����ɂ��A����Q�[���J�n���ɃL�[�R���t�B�O�̓��Z�b�g�����B

	soundVol = 100;
	lpSoundMng.ChangeVolume(soundVol - 30, "./source/Sound/walk_trim.mp3");
	lpSoundMng.ChangeVolume(soundVol, "./source/Sound/coinget.mp3");
	lpSoundMng.ChangeVolume(soundVol, "./source/Sound/��02.mp3");
	lpSoundMng.ChangeVolume(soundVol, "./source/Sound/���{�b�g����������3.mp3");
	lpSoundMng.ChangeVolume(soundVol, "./source/Sound/EnemyDeath.mp3");
	lpSoundMng.ChangeVolume(soundVol + 30, "./source/Sound/UI�ړ���.mp3");
	lpSoundMng.ChangeVolume(soundVol, "./source/Sound/UI�I����.mp3");
	lpSoundMng.ChangeVolume(soundVol - 30, "./source/Sound/machikouba.mp3");
	lpSoundMng.ChangeVolume(soundVol - 10, "./source/Sound/streetofrage.mp3");
	lpSoundMng.ChangeVolume(soundVol - 10, "./source/Sound/stageclear.mp3");
	lpSoundMng.ChangeVolume(soundVol - 20, "./source/Sound/selectscene.mp3");
	lpSoundMng.ChangeVolume(soundVol - 20, "./source/Sound/explosion.mp3");

	// �����V�[���̃C���X�^���X
	//scene_ = lpSceneUIMng.Init((std::move(std::make_unique<TitleScene>())));
	scene_ = lpSceneUIMng.Init((std::move(std::make_unique<LoadScene>(0))));
	std::chrono::system_clock::time_point now, old;
	now = std::chrono::system_clock::now();

	Fps fps;

	//FontHandle_ = CreateFontToHandle("HG����E", 50, 5, DX_FONTTYPE_NORMAL);
	FontHandle_ = CreateFontToHandle("UD�f�W�^�����ȏ���", 50, 5, DX_FONTTYPE_NORMAL);

	lpSceneUIMng.Data_SetDef();

	while (ProcessMessage() == 0 && !gameEndFlg)
	{
		//�X�V
		_dbgStartDraw();

		// �o�ߎ��Ԃ̌v�Z
		old = now;
		now = std::chrono::system_clock::now();
		auto elTime = now - old;
		auto msec = std::chrono::duration_cast<std::chrono::microseconds>(elTime).count();
		double delta = msec / 1000000.0;
		deltaTime_ = delta;

		fps.Update();

		lpCntMng.GetController()->Update();
		scene_ = scene_->Update(delta, std::move(scene_));
		lpSceneUIMng.Update(delta);

		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();

		fps.Draw();

		Draw(delta);
		lpSceneUIMng.Draw(delta);
		cnt_++;
		ScreenFlip();
		fps.Wait();//�ҋ@
		//���ꂪ����ƃ��j�^�[��Hz��60�ȏ�ł�60�ɌŒ肵�Ă����I

		if (CheckHitKey(KEY_INPUT_ESCAPE)&& CheckHitKey(KEY_INPUT_F1))
		{
			gameEndFlg = true;
		}
	}
}

uniqueScene SceneManager::Update(void)
{
	return std::move(scene_);
}

void SceneManager::Draw(double delta)
{
	scene_->Draw(delta);

	//_dbgDrawFormatString(0, 1000, 0xffffff, "%d", GetDrawCallCount());
	_dbgDraw();
}

double SceneManager::GetDeltaTime(void)
{
	return deltaTime_;
}

int SceneManager::GetFontHandle(void)
{
	return FontHandle_;
}

void SceneManager::SetMultiFlg(bool flg)
{
	multiFlg_ = flg;
}

bool SceneManager::GetMultiFlg(void)
{
	return multiFlg_;
}

Vector2Int SceneManager::GetScreenSize(void)
{
	Vector2Int ScreenSize = { ScreenSizeX,ScreenSizeY };
	return ScreenSize;
}

void SceneManager::SetEndFlg(bool flg)
{
	gameEndFlg = flg;
}

void SceneManager::SetSoundVol(int vol)
{
	soundVol = vol;
}

int SceneManager::GetSoundVol()
{
	return soundVol;
}

bool SceneManager::SysInit(void)
{
	SetWindowIconID(ICON_1);
	// �V�X�e������
	SetWindowText("NotScrap");
	SetGraphMode(ScreenSizeX, ScreenSizeY, 16);

	SetUseDirect3DVersion(DX_DIRECT3D_11);
	ChangeWindowMode(true);
	//ChangeWindowMode(true);

	if (DxLib_Init() == -1)
	{
		return false;
	}
	if (Effekseer_Init(8000)==-1)
	{
		DxLib_End();
		return false;
	}
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	//SetMouseDispFlag(false);
	//�L�[��񏉊���
	_dbgSetup(ScreenSizeX, ScreenSizeY, 255);
	// �}�E�X��\����Ԃɂ���
	SetMouseDispFlag(false);
	//�}�E�X����ʊO�ɍs���Ȃ��悤�ɂ���
	SetValidMousePointerWindowOutClientAreaMoveFlag(true);
	return true;
}

Scene SceneManager::GetSceneID(void)
{
	return scene_->GetSceneID();
}

SceneManager::SceneManager()
{
	initFlg_ = SysInit();
}

SceneManager::~SceneManager()
{
}
