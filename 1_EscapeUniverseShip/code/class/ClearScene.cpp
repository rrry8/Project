#include<DxLib.h>
#include "ClearScene.h"
#include"SelectScene.h"
#include"SceneManager.h"
#include"Transition/FadeInOut.h"
#include"Input/Controller.h"
#include"Input/KeyInput.h"
#include"Input/PadInput.h"
#include"Other/ImageMng.h"
#include"Other/SoundMng.h"

constexpr int ClearSizeX = 777;

constexpr int PressZsizeX = 389;
constexpr int PressZsizeY = 58;

constexpr int PressAsizeX = 504;
constexpr int PressAsizeY = 48;
constexpr double fluffyLimit = 0.25;

ClearScene::ClearScene()
{
    Init();
    DrawScreen();
}

ClearScene::~ClearScene()
{
}

void ClearScene::Init(void)
{
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
    con_ = 1.0;
    fluffyTime_ = 0.0;
    plus_ = 0.004;
    PlaySoundMem(lpSoundMng.GetID("./sound/clearscene.mp3")[0], DX_PLAYTYPE_LOOP);
}

UniqueScene ClearScene::Update(UniqueScene scene)
{
    controller_->Update();
    auto UsedController = controller_->GetControllerData();

    if (UsedController[KeyID::Decide][static_cast<int>(ControllerAge::Now)] && !UsedController[KeyID::Decide][static_cast<int>(ControllerAge::Old)])
    {
        PlaySoundMem(lpSoundMng.GetID("./sound/decide.mp3")[0], DX_PLAYTYPE_BACK);
        StopSoundMem(lpSoundMng.GetID("./sound/clearscene.mp3")[0]);
        return std::make_unique<FadeInOut>(1.0, std::move(scene), std::make_unique<SelectScene>());
    }
    con_ += plus_;
    fluffyTime_ += lpSceneMng.GetDeltaTime();
    if (fluffyLimit <= fluffyTime_)
    {
        plus_ = -plus_;
        fluffyTime_ = 0.0;
    }

    DrawScreen();
    return scene;
}

void ClearScene::DrawScreen(void)
{
    auto viewsize = lpSceneMng.GetViewSize();

    SetDrawScreen(screenID_);
    ClsDrawScreen();

    //背景
    DrawGraph(0, 0, lpImageMng.GetID("./Image/clearBG.png")[0], true);

    //ステージクリア
    DrawGraph((viewsize.x - ClearSizeX) / 2, 50, lpImageMng.GetID("./Image/font/clear.png")[0], true);

    //指示
    if (GetJoypadNum() == 0)
    {
        DrawRotaGraph((viewsize.x) / 2, (viewsize.y - 200 + PressZsizeY), con_, 0.0, lpImageMng.GetID("./Image/font/presskey.png")[0], true);
    }
    else
    {
        DrawRotaGraph((viewsize.x) / 2, (viewsize.y - 200 + PressAsizeY), con_, 0.0, lpImageMng.GetID("./Image/font/presspad.png")[0], true);
    }
}

void ClearScene::Release(void)
{
}
