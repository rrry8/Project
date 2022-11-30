#include <DxLib.h>
#include "SceneManager.h"
#include"TitleScene.h"
#include"GameScene.h"
#include"LoadScene.h"

#include "InventoryScene.h"
#include "../common/Fps.h"
#include "../common/SocketServer.h"
#include"../common/MouseIcon.h"

#include "../../_debug/_DebugDispOut.h"
//��ʃT�C�Y
//constexpr int SCREEN_SIZE_X = 1280;	//��
//constexpr int SCREEN_SIZE_Y = 720;	//�c
constexpr int SCREEN_SIZE_X = 1920;	//��
constexpr int SCREEN_SIZE_Y = 1080;	//�c
bool SceneManager::SysInit(void)
{
	SetGraphMode(static_cast<int>(SCREEN_SIZE_X), static_cast<int>(SCREEN_SIZE_Y), 32);
	ChangeWindowMode(true);

	SetEnableXAudioFlag(true);
	// 64.0f �łP���[�g���Ƃ���
	Set3DSoundOneMetre(1500.0f);

	SetWindowText("AR�z���[");
	if (DxLib_Init() == -1)
	{
		return false;
	}
	SetMouseDispFlag(false);

	_dbgSetup(static_cast<int>(SCREEN_SIZE_X), static_cast<int>(SCREEN_SIZE_Y), 255);
	return true;
}

SceneManager::SceneManager()
{
	constantBuffer_ = -1;
	writer_ = nullptr;
	brightnessScrren_ = 255;
	
}

SceneManager::~SceneManager()
{
	lpSocketServer.CommunicationCutoff();
}

void SceneManager::UpdateMouse()
{
	Vector2Int Mpos;
	GetMousePoint(&Mpos.x, &Mpos.y);
	if (scene_->GetSceneID() == SceneID::Game)
	{
		Mpos.x += lpSocketServer.GetCameraValue().first;
		Mpos.y -= lpSocketServer.GetCameraValue().second;
	}
	else
	{
		Mpos.x += lpSocketServer.GetCameraValue().first / 10;
		Mpos.y -= lpSocketServer.GetCameraValue().second / 10;
	}
	SetMousePoint(Mpos.x, Mpos.y);
}

void SceneManager::Init(void)
{
	tickCount_ = std::chrono::system_clock::now();
	//mouseIcon_ = std::make_unique<MouseIcon>();
	Init3D();
}

void SceneManager::Init3D(void)
{
	// Z�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(true);

	// Z�o�b�t�@�ւ̏������݂�L���ɂ���
	SetWriteZBuffer3D(true);

	// �o�b�N�J�����O��L���ɂ���
	SetUseBackCulling(true);

	// �N���b�v������ݒ肷��
	//SetCameraNearFar(CAMERA_RANGE_NEAR, CAMERA_RANGE_FAR);

	// ���C�g�̐ݒ�
	ChangeLightTypeDir({ 0.3f, -0.7f, 0.8f });

	// �w�i�F�ݒ�
	SetBackgroundColor(0, 0, 0);

	// �t�H�O��L���ɂ���
	SetFogEnable(TRUE);

	// �t�H�O�̐F�����F�ɂ���
	SetFogColor(0, 0, 0);

	// �t�H�O�̊J�n�������O�A�I���������P�T�O�O�ɂ���
	SetFogStartEnd(0.0f, 12000.0f);
}

void SceneManager::Run(void)
{
	if (!SysInit())
	{
		return;
	}

	Init();
	Fps fps;
	//�V�[��������
	scene_ = std::make_unique<TitleScene>();
	constantBuffer_ = CreateShaderConstantBuffer(sizeof(float) * 8);
	writer_ = static_cast<float*>(GetBufferShaderConstantBuffer(constantBuffer_));

	int screenSizeX, screenSizeY;
	GetDrawScreenSize(&screenSizeX, &screenSizeY);
	sceneGraphFilter_ = MakeScreen(screenSizeX, screenSizeY);

	for (int n = 0; n < 8; n++)
	{
		writer_[n] = 0.0f;
	}
	lpSocketServer.InternetConnection();

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_F1) == 0)
	{
		
		_dbgStartDraw();

		lpSocketServer.Receive();
		UpdateMouse();

		auto tick = std::chrono::system_clock::now();
		deltaTime_ = std::chrono::duration_cast<std::chrono::microseconds>(tick - tickCount_).count() / 1000000.0f;
		tickCount_ = tick;

		fps.Update();

		//�萔�o�b�t�@�͂R����(0��1��Dxlib���g�p)
		SetShaderConstantBuffer(constantBuffer_, DX_SHADERTYPE_PIXEL, 3);
		//writer_[0] += 0.01f;
		UpdateShaderConstantBuffer(constantBuffer_);


		//�X�V
		scene_ = scene_->Update(std::move(scene_));
		lpMouseIcon.Update(scene_->GetSceneID());

		SetDrawScreen(sceneGraphFilter_);
		ClsDrawScreen();
		scene_->Draw();
		SetDrawScreen(DX_SCREEN_BACK);
		ClsDrawScreen();
		GraphFilter(sceneGraphFilter_, DX_GRAPH_FILTER_LEVEL, 0, 255, brightnessScrren_-155,0,255);
		DrawGraph(0, 0, sceneGraphFilter_, true);
		//fps.Draw();
		//SetDrawBlendMode(DX_BLENDMODE_ADD, brightnessScrren_);
		////�`��
		//scene_->Draw();
		//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		lpMouseIcon.Draw();
		//mouseIcon_->Draw();
		


		_dbgDraw();
		ScreenFlip();
		fps.Wait();//�ҋ@
	}
	SetWindowVisibleFlag(false);
	DxLib_End();
}

Vector2Int SceneManager::GetScreenSize(void)
{
	return Vector2Int(SCREEN_SIZE_X, SCREEN_SIZE_Y);
}

double SceneManager::GetDeltaTime(void)
{
	return deltaTime_;
}

void SceneManager::SetWriterData(int num, float setData)
{
	writer_[num] = setData;
}

void SceneManager::SetBrightnessScrren(int brightnessScrren)
{
	brightnessScrren_ = brightnessScrren;
}

int SceneManager::GetBrightnessScrren()
{
	return brightnessScrren_;
}

void SceneManager::AddInventoryCnt()
{
	inventoryCnt_++;
}

int SceneManager::GetInventoryCnt()
{
	return inventoryCnt_;
}
