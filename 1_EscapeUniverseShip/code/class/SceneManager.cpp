#include <DxLib.h>
#include<EffekseerForDXLib.h>
#include "SceneManager.h"
#include"Input/KeyInput.h"
#include"TitleScene.h"
#include"ClearScene.h"
#include"SelectScene.h"
#include"../Image/icon/icon.h"
#include"../_debug/_DebugConOut.h"
#include"../_debug/_DebugDispOut.h"

constexpr float ViewSizeX = 1280.0f;
constexpr float ViewSizeY = 720.0f;

bool SceneManager::SysInit(void)
{
	//SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_MAXIMUM);
	SetGraphMode(static_cast<int>(ViewSizeX), static_cast<int>(ViewSizeY), 32);
	ChangeWindowMode(true);


	// DirectX11���g�p����悤�ɂ���B(DirectX9���A�ꕔ�@�\�s��)
	// Effekseer���g�p����ɂ͕K���ݒ肷��B
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	SetWindowText("EscapeUniverseShip");
	SetWindowIconID(ICON_1);
	if (DxLib_Init() == -1)
	{
		return false;
	}
	if (Effekseer_Init(8000) == -1)
	{
		return false;
	}
	// �t���X�N���[���E�C���h�E�̐؂�ւ��Ń��\�[�X��������̂�h���B
	// Effekseer���g�p����ꍇ�͕K���ݒ肷��B
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DX���C�u�����̃f�o�C�X���X�g�������̃R�[���o�b�N��ݒ肷��B
	// �E�C���h�E�ƃt���X�N���[���̐؂�ւ�����������ꍇ�͕K�����s����B
	// �������ADirectX11���g�p����ꍇ�͎��s����K�v�͂Ȃ��B
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// Effekseer��2D�`��̐ݒ������B
	Effekseer_Set2DSetting(static_cast<int>(ViewSizeX), static_cast<int>(ViewSizeY));

	//_dbgSetup(static_cast<int>(ViewSizeX), static_cast<int>(ViewSizeY), 255);
	return true;
}

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Init(void)
{
	tickCount_ = std::chrono::system_clock::now();
}

void SceneManager::Run(void)
{

	if (!SysInit())
	{
		return;
	}
	Init();
	scene_ = std::make_unique<TitleScene>();
	// Z�o�b�t�@��L���ɂ���B
	// Effekseer���g�p����ꍇ�A2D�Q�[���ł�Z�o�b�t�@���g�p����B
	SetUseZBuffer3D(TRUE);

	// Z�o�b�t�@�ւ̏������݂�L���ɂ���B
	// Effekseer���g�p����ꍇ�A2D�Q�[���ł�Z�o�b�t�@���g�p����B
	SetWriteZBuffer3D(TRUE);

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		//_dbgStartDraw();

		auto tick = std::chrono::system_clock::now();
		deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(tick - tickCount_).count() / 1000000.0f;
		tickCount_ = tick;

		scene_ = scene_->Update(std::move(scene_));

		SetDrawScreen(DX_SCREEN_BACK);
		ClsDrawScreen();

		scene_->Draw();

		//_dbgDraw();
		ScreenFlip();
	}
	Effkseer_End();
	DxLib_End();
}

Vector2 SceneManager::GetViewSize(void)
{
	Vector2 ViewSize = { ViewSizeX,ViewSizeY };
	return ViewSize;
}

double SceneManager::GetDeltaTime(void)
{
	return deltaTime;
}
