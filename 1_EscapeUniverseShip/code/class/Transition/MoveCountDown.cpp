#include<DxLib.h>
#include<cmath>
#include "MoveCountDown.h"
#include"../Camera/Camera.h"
#include"../Obj/Player.h"
#include "../Other/ImageMng.h"
#include"../Other/SoundMng.h"
#include"../SceneManager.h"
#include"../Input/KeyInput.h"
#include"../Input/PadInput.h"


constexpr float GoalPosX = 530.0f;
constexpr float GoalPosY = 150.0f;

constexpr double AlphaCoeff = 0.199218875;

static constexpr double BLINK_LIMIT = 0.25;

MoveCountDown::MoveCountDown(double LimitTime, UniqueScene beforeScene, UniqueScene afterScene) :
	TransitionScene(std::move(beforeScene), std::move(afterScene))
{
	LimitTime_ = LimitTime;
	camera_ = std::make_unique<Camera>(3000.0f, 720.0f);
	player_ = std::make_unique<Player>();
	if (GetJoypadNum() == 0)
	{
		//�L�[�{�[�h�̏ꍇ
		controller_ = std::make_unique<KeyInput>();
	}
	else
	{
		//�p�b�h�̏ꍇ
		controller_ = std::make_unique<PadInput>();
	}
	DrawScreen();
}

MoveCountDown::MoveCountDown(double LimitTime, std::string stage, std::string nonstage,Vector2 pos, UniqueScene beforeScene, UniqueScene afterScene):
	TransitionScene(std::move(beforeScene), std::move(afterScene))
{
	LimitTime_ = LimitTime;
	camera_ = std::make_unique<Camera>(3000.0f, 720.0f);
	player_ = std::make_unique<Player>();
	if (GetJoypadNum() == 0)
	{
		//�L�[�{�[�h�̏ꍇ
		controller_ = std::make_unique<KeyInput>();
	}
	else
	{
		//�p�b�h�̏ꍇ
		controller_ = std::make_unique<PadInput>();
	}

	stage_ = stage;
	nonstage_ = nonstage;
	pos_ = pos;
	blink_ = 0.0;

	Vector2 goalPos = { GoalPosX,GoalPosY };
	vel = (goalPos - pos_).Normalized() * 2.0f;

	DrawScreen();
}

MoveCountDown::~MoveCountDown()
{
}

bool MoveCountDown::UpdateTransition(double delta)
{
	controller_->Update();
	auto UsedController = controller_->GetControllerData();

	auto carPos = player_->GetPos();
	auto offset = camera_->GetPos();
	count++;
	camera_->Update(player_->GetPos());
	if (carPos.x != lpSceneMng.GetViewSize().x + 256.0f)
	{
		float con = 1.0f;
		if (UsedController[KeyID::Decide][static_cast<int>(ControllerAge::Now)])
		{
			con = 4.0f;
		}
		player_->MoveRight(camera_->GetPos(), con);
		carPos = player_->GetPos();
		angle_ += 1.0;
	}
	else
	{
		count2 += delta;
		if (count2 >= 1.5)
		{
			return true;
		}
	}
	if (carPos.x >= lpSceneMng.GetViewSize().x + 256.0f)
	{
		player_->SetPos({ lpSceneMng.GetViewSize().x + 256.0f ,665.0f });
	}

	MoveFont(vel.x > 0);

	//�t�H���g�_��
	blink_ += lpSceneMng.GetDeltaTime();
	if (blink_ > BLINK_LIMIT * 2)
	{
		blink_ = 0.0;
	}

	return false;
}

void MoveCountDown::MoveFont(bool flag)
{
	pos_ += vel;
	//�t�H���g�ړ�
	if (flag)
	{
		if (pos_.x > GoalPosX)
		{
			pos_.x = GoalPosX;
		}
		if (pos_.y < GoalPosY)
		{
			pos_.y = GoalPosY;
		}

	}
	else
	{
		if (pos_.x < GoalPosX)
		{
			pos_.x = GoalPosX;
		}
		if (pos_.y < GoalPosY)
		{
			pos_.y = GoalPosY;
		}

	}

}

void MoveCountDown::DrawScreen()
{
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	auto delta = lpSceneMng.GetDeltaTime();
	auto offset = camera_->GetPos();
	DrawGraph(-offset.x, -offset.y, lpImageMng.GetID("./Image/BGtransition.png")[0], true);

	auto carPos = player_->GetPos();
	auto alpha = static_cast<int>(carPos.x * AlphaCoeff * 2.0);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - alpha);
	beforeScene_->DrawplusOffset(-offset);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, alpha);

	afterScene_->DrawplusOffset({ lpSceneMng.GetViewSize().x - offset.x,-offset.y });
	PlayerDraw();

	if (blink_ < BLINK_LIMIT)
	{
		DrawGraph(pos_.x, pos_.y, lpImageMng.GetID(nonstage_)[0], true);
	}
	else if (blink_ < BLINK_LIMIT * 2)
	{
		DrawGraph(pos_.x, pos_.y, lpImageMng.GetID(stage_)[0], true);
	}

}

void MoveCountDown::PlayerDraw(void)
{
	auto offset = camera_->GetPos();
	auto carPos = player_->GetPos();
	//�E�^�C���̍��v���Z�l�w
	auto AddWheelRX = NormalAddWheelRX;
	//���^�C���̍��v���Z�l�w
	auto AddWheelLX = NormalAddWheelLX;
	//�^�C���̍��v���Z�l�x
	auto AddWheelY = NormalAddWheelY;

	//���@
	//�E�̃^�C��
	DrawRotaGraph(static_cast<int>(carPos.x + AddWheelRX - offset.x), static_cast<int>(carPos.y + AddWheelY - offset.y), 1.0, angle_, lpImageMng.GetID("././Image/Wheel.png")[0], true);
	//���̃^�C��
	DrawRotaGraph(static_cast<int>(carPos.x + AddWheelLX - offset.x), static_cast<int>(carPos.y + AddWheelY - offset.y), 1.0, angle_, lpImageMng.GetID("././Image/Wheel.png")[0], true);
	//�ԑ�
	DrawRotaGraph(static_cast<int>(carPos.x - offset.x), static_cast<int>(carPos.y - offset.y), 1.0, 0.0, lpImageMng.GetID("././Image/Car.png")[0], true);
}
