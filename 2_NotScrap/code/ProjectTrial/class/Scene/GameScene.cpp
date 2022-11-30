#include <DxLib.h>
#include<EffekseerForDXLib.h>
#include "../../_debug/_DebugConOut.h"
#include "../../_debug/_DebugDispOut.h"
#include "../Transition/CrossOver.h"
#include "../Transition/FadeinOut.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "../Common/ImageMng.h"
#include "../Common/MovieMng.h"
#include"../Camera/Camera.h"
#include"../Obj/Unit/Player.h"
#include "../Obj/Unit/Enemy.h"
#include"../Obj/Unit/EnemyShoot.h"
#include"../Obj/Unit/Item/Coin.h"
#include"../Obj/Stage.h"
#include"../Obj/Gimmick/PressurePlate.h"
#include"../Obj/Gimmick/PressureDoor.h"
#include"../Obj/Gimmick/PressureSidewaysDoor.h"
#include"../Obj/Gimmick/ElectricPole.h"
#include"../Obj/Gimmick/MagBox.h"
#include"../Obj/Gimmick/Gear.h"
#include"../Obj/Gimmick/MidPoint.h"
#include "ResultScene.h"
#include "../UI/SceneUIManager.h"
#include "OptionScene.h"
#include "../Input/KeyInput.h"
#include "../Input/PadInput.h"
#include "../Obj/Goal.h"
#include "../Obj/Unit/Item/BulletManager.h"
#include "../Common/SoundMng.h"
#include "../Obj/Gimmick/Accessory.h"
#include "../Obj/Gimmick/Tutorial.h"
#include "../Obj/Gimmick/SmokeAcs.h"
#include "../Obj/Gimmick/WarningSign.h"
#include"../Obj/Gimmick/BeltConveyor.h"
#include "../Common/ModelMng.h"
#include "MenuScene.h"

constexpr int RANGE = 2000;
constexpr float WITHIN_RANGE = 3000.0f;

GameScene::GameScene()
{
	Init();
	DrawOwnScreen(0.0);
}

GameScene::~GameScene()
{
	// ���C�g�n���h���̍폜
	DeleteLightHandle(lightHandle_);
	DeleteShadowMap(ShadowMapHandle_);

	DeleteShader(psDistortionHandle_);
	DeleteShader(psCrackedScreen_);

	DeleteShaderConstantBuffer(cBuff_);
}

State GameScene::GetPlayerState(void)
{
	return playerList_[0]->GetState();
}

Vector3 GameScene::GetShakePlayer(void)
{
	return playerList_[0]->ShakeGetPos();
}

int GameScene::GetScreenID(void)
{
	return screenID_;
}

bool GameScene::Init(void)
{
	//3D������
	Init3D();

	//�^�C��������
	clearTime_ = 0;
	bestClearTime_ = lpSceneUIMng.Data_Load("./source/Data/Time/bestClearTime.txt", lpSceneUIMng.GetStageNum());
	//�J�����̍쐬
	camera_ = std::make_unique<Camera>();

	//�I�u�W�F�N�g�̍쐬
	CreateObject();

	dust_.resource = LoadEffekseerEffect("./source/effect/dust.efkefc", 30.0f);
	dust_.playhandle = -1;
	dust_.playhandle = PlayEffekseer3DEffect(dust_.resource);
	dust_.pos = Vector3{ 600,400 ,0 };
	SetPosPlayingEffekseer3DEffect(dust_.playhandle, dust_.pos.x, dust_.pos.y, dust_.pos.z);

	//�V�F�[�_
	//�V�F�[�_�n���h���ǂݍ���
	psDistortionHandle_ = LoadPixelShader("./source/shader/PS_MagTrail.pso");		//�c��
	psCrackedScreen_ = LoadPixelShader("./source/shader/PS_CrackedScreen.pso");		//��ʊ���
	//�����_�����O�^�[�Q�b�g�쐬
	renderTarget_ = MakeScreen(lpSceneMng.GetScreenSize().x,lpSceneMng.GetScreenSize().y);

	//�萔
	cBuff_ = CreateShaderConstantBuffer(sizeof(float) * 4);
	writer_ = static_cast<float*>(GetBufferShaderConstantBuffer(cBuff_));
	writer_[0] = 0.0f;

	return true;
}

void GameScene::Init3D(void)
{
	//Z�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(true);

	//Z�o�b�t�@�ւ̏������݂�L���ɂ���
	SetWriteZBuffer3D(true);

	//�o�b�N�������O��L���ɂ���
	SetUseBackCulling(true);

	//�J�����̃N���b�v�����̐ݒ�
	SetCameraNearFar(0.0f, 1500.0f);

	//���C�g�̐ݒ�
	ChangeLightTypeDir({ 0.3f,-0.6f,0.8f });

	lightHandle_ = CreateDirLightHandle(VGet(-0.5f, 0.2f, 0.3f));

	// �V���h�E�}�b�v�n���h���̍쐬
	ShadowMapHandle_ = MakeShadowMap(2048, 2048);

	SetShadowMapLightDirection(ShadowMapHandle_, VGet( 0.1f,-0.3f,0.8f ));
	
	// �V���h�E�}�b�v�ɕ`�悷��͈͂�ݒ�
	SetShadowMapDrawArea(ShadowMapHandle_, VGet(-3000, -3000, -3000), VGet(3000, 3000, 3000));
	SetShadowMapAdjustDepth(ShadowMapHandle_, 0.013); 
	//3D�̔w�i�F
	SetBackgroundColor(0, 0, 0);
	Adjust = 0.002f;
}

uniqueScene GameScene::Update(double delta, uniqueScene ownScene)
{
	fadeFlag_ = false;
	if (CheckSoundMem(lpSoundMng.GetID("./source/Sound/streetofrage.mp3")[0]) == 0)
	{
		PlaySoundMem(lpSoundMng.GetID("./source/Sound/streetofrage.mp3")[0], DX_PLAYTYPE_BACK);
	}
	if (CheckSoundMem(lpSoundMng.GetID("./source/Sound/machikouba.mp3")[0]) == 0)
	{
		PlaySoundMem(lpSoundMng.GetID("./source/Sound/machikouba.mp3")[0], DX_PLAYTYPE_BACK);
	}
	camera_->Update();
	lpStage.g_dynamicsworld->stepSimulation(1 / 60.f, 10);

	cntData_ = lpCntMng.GetController()->GetCntData();
	ownScene = UpdateObject(ownScene);
	if (abs(dust_.pos.x - camera_->GetPos().x) > RANGE ||
		abs(dust_.pos.y - camera_->GetPos().y) > RANGE)
	{
		SetShadowMapDrawArea(ShadowMapHandle_, VGet(-3000- camera_->GetPos().x, -3000 - camera_->GetPos().y, -1000 - camera_->GetPos().z), 
			VGet(3000 + camera_->GetPos().x, 3000 + camera_->GetPos().y, 1000 + camera_->GetPos().z));

		StopEffekseer3DEffect(dust_.playhandle);
		dust_.playhandle = PlayEffekseer3DEffect(dust_.resource);
		dust_.pos = camera_->GetPos();
		SetPosPlayingEffekseer3DEffect(dust_.playhandle, dust_.pos.x, dust_.pos.y, dust_.pos.z);
	}

	if (KeyTrgDown(InputID::Menu))
	{
		lpSoundMng.PlayingSound("./source/Sound/UI�I����.mp3");
		return lpSceneUIMng.Init((std::move(std::make_unique<MenuScene>(ownScene)))); // ����
	}

	//R�������ƃS�[������i�f�o�b�N�p�j
	for (auto& player : playerList_)
	{
		if (player->GetState() == State::Goal)
		{
			if (CheckSoundMem(lpSoundMng.GetID("./source/Sound/streetofrage.mp3")[0]) != 0)
			{
				StopSoundMem(lpSoundMng.GetID("./source/Sound/streetofrage.mp3")[0]);
			}
			if (CheckSoundMem(lpSoundMng.GetID("./source/Sound/machikouba.mp3")[0]) != 0)
			{
				StopSoundMem(lpSoundMng.GetID("./source/Sound/machikouba.mp3")[0]);
			}
			//�N���A�^�C���̏����t�@�C���ɏ����o��
			lpSceneUIMng.Data_Save("./source/Data/Time/clearTime.txt", static_cast<int>(clearTime_),lpSceneUIMng.GetStageNum());
			if (bestClearTime_ != 0)//���ڈȍ~�ŁA���ȃx�X�g�X�V���ɂ������ɓ���
			{
				if (bestClearTime_ > clearTime_)
				{
					lpSceneUIMng.Data_Save("./source/Data/Time/bestClearTime.txt",static_cast<int>(clearTime_), lpSceneUIMng.GetStageNum());
				}
			}
			else//�ŏ��͂������ɓ���
			{
				lpSceneUIMng.Data_Save("./source/Data/Time/bestClearTime.txt", static_cast<int>(clearTime_), lpSceneUIMng.GetStageNum());
			}
			int a = 0;
			int b = 0;
			int c = 0;

			int i = 0;
			for (const auto& coin : coinList_)
			{
				if (i == 0)
				{
					a = coin->GetCoinFlg();
				}
				else if (i == 1)
				{
					b = coin->GetCoinFlg();
				}
				else if (i == 2)
				{
					c = coin->GetCoinFlg();
				}
				i++;
			}
			lpSceneUIMng.CoinData_Save("./source/Data/Coin/coinState.txt", a, b, c, lpSceneUIMng.GetStageNum());
			lpStage.Remove();
			return lpSceneUIMng.Init(
				std::move(std::make_unique<CrossOver>(
					2.0,
					std::move(ownScene),
					lpSceneUIMng.Init(std::move(std::make_unique<ResultScene>(coinList_, clearTime_)))
					))); // ����
		}
	}

	//�V�F�[�_�֌W
	SetShaderConstantBuffer(cBuff_, DX_SHADERTYPE_PIXEL, 5);
	writer_[0] += 0.01f;
	UpdateShaderConstantBuffer(cBuff_);

	DrawOwnScreen(delta);
	clearTime_ += lpSceneMng.GetDeltaTime();
	return ownScene;
}

void GameScene::DrawOwnScreen(double delta)
{
	SetDrawScreen(renderTarget_);
	ClearDrawScreen();

	//1�v���C��2�v���C�̐ؑ�
	if (!lpSceneMng.GetMultiFlg())
	{
		camera_->SetBeforeDraw(playerList_[0]->ShakeGetPos());
	}
	else
	{
		auto Multi = playerList_[0]->ShakeGetPos() - playerList_[1]->ShakeGetPos();
		auto multi = playerList_[0]->ShakeGetPos() + Vector3{ -Multi.x / 2, -Multi.y / 2, -abs(Multi.x / 4 + Multi.y / 4) };
		camera_->SetBeforeDraw(multi);
	}

	// �V���h�E�}�b�v�ւ̕`��̏���
	ShadowMap_DrawSetup(ShadowMapHandle_);

	//�I�u�W�F�N�g�̕`��
	//�M�~�b�N�̕`��
	for (auto& gimmick : gimmickList_)
	{
		gimmick->Draw();
	}
	//�R�C���̕`��
	for (auto& coin : coinList_)
	{
		coin->Draw();
	}
	//�G�̕`��
	for (auto& list : enemyList_)
	{
		list->Draw();
	}
	//���΂̕`��
	for (auto& box : magBoxList_)
	{
		box->Draw();
	}
	//�v���C���[�̕`��
	for (auto& player : playerList_)
	{
		player->Draw();
	}

	//�e�̕`��
	lpBulletMng.Draw();

	// �V���h�E�}�b�v�ւ̕`����I��
	ShadowMap_DrawEnd();

	// �`��Ɏg�p����V���h�E�}�b�v��ݒ�
	SetUseShadowMap(0, ShadowMapHandle_);

	//�X�e�[�W
	lpStage.Draw();

	//�I�u�W�F�N�g�̕`��
	DrawObject();

	// �`��Ɏg�p����V���h�E�}�b�v�̐ݒ������
	SetUseShadowMap(0, -1);

	SetDrawScreen(screenID_);
	ClearDrawScreen();

	camera_->SetBeforeDraw(playerList_[0]->ShakeGetPos());

	DrawGraph(0, 0, renderTarget_, false);

	if (playerList_[0]->GetState() == State::DismemberDeath)
	{
		DrawPostEffect(psCrackedScreen_, renderTarget_, lpImageMng.GetID("./source/shader/texture/CrackNormalMap.png")[0]);
	}
	else
	{
		//�c��
		//�O�Ղ̕`��
		DrawDistortion(psDistortionHandle_, renderTarget_,
			lpImageMng.GetID("./source/shader/texture/distortion2.png")[0], GetTrailData());
	}

	DrawEffekseer3D();
}

std::shared_ptr<Player> GameScene::DismemberPlayer(std::shared_ptr<Player>& player)
{
	if (player->GetState() == State::DismemberDeath)
	{
		if (!player->GetisAlive())
		{
			lpStage.CleanBulletObj(player->GetBody());
			player->ReCreateBullet();
			player->SetisAlive(true);
		}
	}
	return player;
}

std::shared_ptr<Player> GameScene::DeathPlayer(std::shared_ptr<Player>& player)
{
	if (player->GetState() == State::Death)
	{
		for (auto gg : player->GetBreakmodel())
		{
			lpStage.CleanBulletObj(gg);
		}
		lpStage.CleanBulletObj(player->GetBody());
		for (auto& gimm : gimmickList_)
		{
			if (gimm->GetObjType() == objType::NMagBox)
			{
				lpStage.CleanBulletObj(gimm->GetRigidBody());
				gimm->SetisAlive(false);
			}
			if (gimm->GetObjType() == objType::SMagBox)
			{
				lpStage.CleanBulletObj(gimm->GetRigidBody());
				gimm->SetisAlive(false);
			}
		}
		player->SetisAlive(false);

		if (player->GetPlayerNum() == PlayerNum::Player1)
		{
			fadeFlag_ = true;
		}
	}
	return player;
}

uniqueScene GameScene::UpdatePlayer(uniqueScene& ownScene)
{
	for (auto& player : playerList_)
	{
		//�v���C�������񂾎��̃o���o���I�u�W�F�N�g�̒ǉ�
		player = DismemberPlayer(player);

		//�v���C���[�̃A�b�v�f�[�g
		player->Update(gimmickList_);

		//�v���C���[�̍폜
		player = DeathPlayer(player);

		//���񂾂Ƃ��̕����t�F�[�h�C���A�E�g
		if (fadeFlag_)
		{
			return std::move(std::make_unique<FadeinOut>(
				1.0,
				Transition::FadeInOut,
				std::move(ownScene)
				));
		}
	}
	return std::move(ownScene);
}

void GameScene::CreateObject(void)
{
	//�G�̍쐬
	CreateEneymyObject();
	//�R�C���̍쐬
	CreateCoinObject();
	//�v���C���[�̍쐬		��������Ȃ��ƓG�ƃR�C���ɔ��肪�Ȃ��Ȃ�
	CreatePlayerObject();
	//�M�~�b�N�̍쐬
	CreateGimmickObject();
	//�S�[���̍쐬
	CreateGoalObject();
}

void GameScene::CreateGimmickObject(void)
{
	int i = 0;
	//�h�A
	for (auto& Door : lpStage.GetDoorList())
	{
		if (i == 0)
		{
			i++;
			continue;
		}
		gimmickList_.emplace_back(std::make_unique<PressurePlate>(Door.pos_, Door.pos2_, DoorType::Door, i - 1));
		i++;
	}
	i = 0;
	for (auto& floor : lpStage.GetFloorList())
	{
		if (i == 0)
		{
			i++;
			continue;
		}
		gimmickList_.emplace_back(std::make_unique<PressurePlate>(floor.pos_, floor.pos2_, DoorType::SidewaysDoor, i-1));
		i++;
	}
	//gimmickList_.emplace_back(std::make_unique<PressurePlate>(Vector3Zero, Vector3{500,500,0}, DoorType::SidewaysDoor, 1));


	i = 0;
	//���d���u
	for (auto& pole : lpStage.GetPoleList())
	{
		if (i == 0)
		{
			i++;
			continue;
		}
		gimmickList_.emplace_back(std::make_unique<ElectricPole>(pole.pos_, i - 1));
		i++;
	}
	i = 0;
	//����
	for (auto& accessory : lpStage.GetAccessoryList())
	{
		if (i == 0)
		{
			i++;
			continue;
		}
		gimmickList_.emplace_back(std::make_unique<Accessory>(accessory.pos_, i - 1));
		i++;
	}
	i = 0;
	//����
	for (auto& gear : lpStage.GetGearList())
	{
		if (i == 0)
		{
			i++;
			continue;
		}
		gimmickList_.emplace_back(std::make_unique<Gear>(gear.pos_, i - 1,gear.objectName));
		i++;
	}
	i = 0;
	//����
	for (auto& mid : lpStage.GetMidPointList())
	{
		if (i == 0)
		{
			i++;
			continue;
		}
		gimmickList_.emplace_back(std::make_unique<MidPoint>(mid.pos_, i - 1));
		i++;
	}

	//�`���[�g���A��
	if (lpStage.GetStage() == STAGE_NAME::STAGE_1)
	{
		gimmickList_.emplace_back(std::make_unique<Tutorial>(
			Vector3{ 3600, 600,300 }, Vector3{ 5400,600 ,0 }, 0,
			enemyList_, TutorialType::TutorialS));

		gimmickList_.emplace_back(std::make_unique<Tutorial>(
			Vector3{ 6200, 600,300 }, Vector3{ 7800, 600,0 }, 1,
			enemyList_, TutorialType::TutorialN));

		gimmickList_.emplace_back(std::make_unique<Tutorial>(
			Vector3{ 600, 600,300 }, Vector3{ 2390, 600,0 }, 2,
			enemyList_, TutorialType::TutorialKey));

		gimmickList_.emplace_back(std::make_unique<Tutorial>(
			Vector3{ 9000, 1000,300 }, Vector3{ 10180, 600,0 }, 3,
			enemyList_, TutorialType::TutorialShift));

		gimmickList_.emplace_back(std::make_unique<Tutorial>(
			Vector3{ 17600, 1000,300 }, Vector3{ 19200, 600,0 }, 4,
			enemyList_, TutorialType::TutorialBullet));

		gimmickList_.emplace_back(std::make_unique<Tutorial>(
			Vector3{ 12100, 1000,300 }, Vector3{ 13800, 600,0 }, 5,
			enemyList_ ,TutorialType::TutorialDeath));

		gimmickList_.emplace_back(std::make_unique<Tutorial>(
			Vector3{ 15100, 1000,300 }, Vector3{ 16200, 600,0 }, 6,
			enemyList_,TutorialType::TutorialStick));

		gimmickList_.emplace_back(std::make_unique<Tutorial>(
			Vector3{ 20600, 1200,300 }, Vector3{ 0, 600,0 }, 7,
			enemyList_, TutorialType::TutorialMagBox));

	}

	i = 0;
	//���G�t�F�N�g
	for (auto& smoke : lpStage.GetSmokeList())
	{
		if (i == 0)
		{
			i++;
			continue;
		}
		gimmickList_.emplace_back(std::make_unique<SmokeAcs>(smoke.pos_));
	}

	int s = 0;
	for (auto& box : lpStage.GetNMagBoxCollList())
	{
		//�n���h���ƍ��W
		gimmickList_.emplace_back(std::make_unique<MagBox>(box.pos, objType::NMagBox, s));
		s++;
		//MagBoxlist_.emplace_back(std::make_unique<MagBox>(MV1LoadModel("source/Model/Magnet/N_Box.mv1"), Vector3{ 600,400,0 }));
	}
	s = 0;
	for (auto& box : lpStage.GetSMagBoxCollList())
	{
		//�n���h���ƍ��W
		gimmickList_.emplace_back(std::make_unique<MagBox>(box.pos, objType::SMagBox, s));
		s++;
		//MagBoxlist_.emplace_back(std::make_unique<MagBox>(MV1LoadModel("source/Model/Magnet/N_Box.mv1"), Vector3{ 600,400,0 }));
	}
	i = 0;
	//�x���g�R���x�A
	for (auto& belt : lpStage.GetBeltConveyorList())
	{
		if (i == 0)
		{
			i++;
			continue;
		}
		gimmickList_.emplace_back(std::make_unique<BeltConveyor>(belt.pos_, i - 1));
		i++;
	}
	i = 0;
	//�x���Ŕ�
	for (auto& sign : lpStage.GetWarningSignList())
	{
		if (i == 0)
		{
			i++;
			continue;
		}
		gimmickList_.emplace_back(std::make_unique<WarningSign>(sign.pos_, i - 1));
		i++;
	}

}

void GameScene::CreateEneymyObject(void)
{
	int i = 0;
	//�G
	for (auto& enemy : lpStage.GetEnemyList())
	{
		if (i == 0)
		{
			i++;
			continue;
		}
		enemyList_.emplace_back(std::make_shared<Enemy>(EnemyType::WALK, enemy.pos_, i - 1));
		i++;
	}
	i = 0;
	//�G�F�ˌ�
	for (auto& enemyShoot : lpStage.GetShootEnemyList())
	{
		if (i == 0)
		{
			i++;
			continue;
		}
		enemyList_.emplace_back(std::make_shared<EnemyShoot>(EnemyType::SHOOT, enemyShoot.pos_, i - 1));
		i++;
	}
}

void GameScene::CreateCoinObject(void)
{
	int i = 0;
	//�R�C��
	for (auto& coin : lpStage.GetCoinList())
	{
		if (i == 0)
		{
			i++;
			continue;
		}
		coinList_.emplace_back(std::make_unique<Coin>(Scene::Game, coin.pos_, i - 1));
		i++;
	}
}

void GameScene::CreateGoalObject(void)
{
	for (auto& goal : lpStage.GetGoalCollList())
	{
		goalList_.emplace_back(std::make_unique<Goal>(goal.first, goal.second));
	}
}

void GameScene::CreatePlayerObject(void)
{
	Vector3 defPos;
	switch (lpStage.GetStage())
	{
	case STAGE_NAME::STAGE_1:
	case STAGE_NAME::STAGE_2:
	case STAGE_NAME::STAGE_3:
	case STAGE_NAME::STAGE_4:
	case STAGE_NAME::STAGE_5:
	case STAGE_NAME::STAGE_6:
		defPos = { 400,250 ,0 };
		break;
	case STAGE_NAME::STAGE_7:
		defPos = { 600,2300 ,0 };
		break;
	case STAGE_NAME::MAX:
		defPos = { 400,250 ,0 };
		break;
	}
	//playerList_.emplace_back(std::make_shared<Player>(std::make_unique<KeyInput>(), defPos, 0, coinList_, enemyList_));

	if (lpSceneMng.GetMultiFlg())
	{
		playerList_.emplace_back(std::make_shared<Player>(std::make_unique<KeyInput>(), defPos, 0, coinList_, enemyList_));
		playerList_.emplace_back(std::make_shared<Player>(std::make_unique<PadInput>(), Vector3{ 500,400 ,0 }, 1, coinList_, enemyList_));
	}
	else
	{
		if (GetJoypadNum() > 0)
		{
			playerList_.emplace_back(std::make_shared<Player>(std::make_unique<PadInput>(), defPos, 0, coinList_, enemyList_));
		}
		else
		{
			playerList_.emplace_back(std::make_shared<Player>(std::make_unique<KeyInput>(), defPos, 0, coinList_, enemyList_));
		}
	}
}

bool GameScene::CheckWithinRange(Vector3 pos, Vector3 objpos)
{
	if (pos.x - WITHIN_RANGE < objpos.x && objpos.x < pos.x + WITHIN_RANGE &&
		pos.y - WITHIN_RANGE < objpos.y && objpos.y < pos.y + WITHIN_RANGE)
	{
		return true;
	}
	return false;
}

uniqueScene GameScene::UpdateObject(uniqueScene& ownScene)
{
	//�v���C���[�̍X�V
	ownScene = UpdatePlayer(ownScene);
	//�G�̍X�V
	for (auto& enemy : enemyList_)
	{
		enemy->Update(playerList_);
	}

	//�e�̍X�V
	lpBulletMng.Move();

	//�M�~�b�N�̍X�V
	for (auto& gimmick : gimmickList_)
	{
		if (!CheckWithinRange(gimmick->GetPos(), playerList_[0]->GetPos()))
		{
			if (gimmick->GetObjType() != objType::PressurePlate||
				gimmick->GetObjType() != objType::PresureDoor)
			{
				continue;

			}
		}
		gimmick->Update(playerList_, gimmickList_);
	}

	//�R�C���̍X�V
	for (auto& coin : coinList_)
	{
		if (!CheckWithinRange(coin->GetCoinPos(), playerList_[0]->GetPos()))
		{
			continue;
		}
		coin->Update();
	}

	UpdateEffekseer3D();
	return std::move(ownScene);
}

void GameScene::DrawObject(void)
{
	//�M�~�b�N�̕`��
	for (auto& gimmick : gimmickList_)
	{
		if (!CheckWithinRange(gimmick->GetPos(), playerList_[0]->GetPos()))
		{
			continue;
		}
		gimmick->Draw();
	}
	//�R�C���̕`��
	for (auto& coin : coinList_)
	{
		if (!CheckWithinRange(coin->GetCoinPos(), playerList_[0]->GetPos()))
		{
			continue;
		}
		coin->Draw();
	}
	//���΂̕`��
	int i = 0;
	for (auto& magnet : lpStage.GetMagnetList())
	{
		if (!CheckWithinRange(magnet.pos_, playerList_[0]->GetPos()))
		{
			continue;
		}
		MV1DrawModel(magnet.path_);
		i++;
	}
	//�G�̕`��
	for (auto& enemy : enemyList_)
	{
		enemy->Draw();
	}
	//���΂̕`��
	for (auto& box : magBoxList_)
	{
		if (!CheckWithinRange(box->GetPos(), playerList_[0]->GetPos()))
		{
			continue;
		}
		box->Draw();
	}
	//�e�̕`��
	lpBulletMng.Draw();
	//�v���C���[�̕`��
	for (auto& player : playerList_)
	{
		player->Draw();
	}
}

void GameScene::DrawDistortion(int ps, int screen, int tex1, const std::vector<VERTEX2DSHADER>& vertex)
{
	SetUsePixelShader(ps);
	SetUseTextureToShader(0, screen);
	SetUseTextureToShader(1, tex1);

	//�`��(����)
	SetUseBackCulling(false);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	DrawPrimitive2DToShader(vertex.data(), vertex.size(), DX_PRIMTYPE_TRIANGLESTRIP);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	SetUseBackCulling(true);

	SetUseTextureToShader(0, -1);
	SetUseTextureToShader(1, -1);
}

std::vector<VERTEX2DSHADER>& GameScene::GetTrailData(void)
{
	return playerList_[0]->GetTrailData();
}

Vector3 GameScene::GetCameraPos(void)
{
	return camera_->GetPos();
}

Vector3 GameScene::GetCameraTargetPos(void)
{
	return camera_->GetTargetPos();
}
