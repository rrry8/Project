#include"DxLib.h"
#include "RetryCount.h"
#include"../SceneManager.h"
#include"../Other/ImageMng.h"
#include "../Input/KeyInput.h"
#include "../Input/PadInput.h"

constexpr int PressZsizeX = 389;
constexpr int PressZsizeY = 58;
constexpr int PressAsizeX = 504;
constexpr int PressAsizeY = 48;
constexpr double fluffyLimit = 0.25;

RetryCount::RetryCount(double LimitTime, UniqueScene beforeScene, UniqueScene afterScene):
	TransitionScene(std::move(beforeScene), std::move(afterScene))
{
	LimitTime_ = LimitTime;
    if (GetJoypadNum() == 0)
    {
        //キーボードの場合
        controller_ = std::make_unique<KeyInput>();
    }
    else
    {
        //パッドの場合
        controller_ = std::make_unique<PadInput>();
    }
    controller_->Update();
    con_ = 1.0;
    fluffyTime_ = 0.0;
    plus_ = 0.004;
	DrawScreen();
}

RetryCount::~RetryCount()
{
}

bool RetryCount::UpdateTransition(double delta)
{
    controller_->Update();
    auto UsedController = controller_->GetControllerData();
    //決定でゲームシーンへ
    if (UsedController[KeyID::Decide][static_cast<int>(ControllerAge::Now)] && !UsedController[KeyID::Decide][static_cast<int>(ControllerAge::Old)])
    {
        return true;
    }

    con_ += plus_;
    fluffyTime_ += lpSceneMng.GetDeltaTime();
    if (fluffyLimit <= fluffyTime_)
    {
        plus_ = -plus_;
        fluffyTime_ = 0.0;
    }

    return false;
}

void RetryCount::DrawScreen()
{
    SetDrawScreen(screenID_);
    ClsDrawScreen();

    afterScene_->Draw();

    auto viewsize = lpSceneMng.GetViewSize();

    if (GetJoypadNum() == 0)
    {
        DrawRotaGraph((viewsize.x) / 2, (viewsize.y / 2 + PressZsizeY), con_, 0.0, lpImageMng.GetID("./Image/font/presskey.png")[0], true);
    }
    else
    {
        DrawRotaGraph((viewsize.x) / 2, (viewsize.y / 2 + PressAsizeY), con_, 0.0, lpImageMng.GetID("./Image/font/presspad.png")[0], true);
    }
}
