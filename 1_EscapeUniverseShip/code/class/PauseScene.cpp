#include<DxLib.h>
#include "PauseScene.h"
#include"SceneManager.h"
#include"SelectScene.h"
#include"Transition/FadeInOut.h"
#include"Other/ImageMng.h"
#include "Input/KeyInput.h"
#include "Input/PadInput.h"
#include "Other/SoundMng.h"

constexpr float PauseOffsetX = 435.0f;
constexpr float PauseOffsetY = 100.0f;

constexpr double fluffyLimit = 0.35;

PauseScene::PauseScene(UniqueScene scene, int id)
{
    ownerScene_ = std::move(scene);
    gameBG_ = id;
    Init();
    DrawScreen();
}

PauseScene::~PauseScene()
{
    Release();
}

void PauseScene::Init(void)
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
    controller_->Update();

    con_ = 1.0;
    fluffyTime_ = 0.0;
    plus_ = 0.004;
    nowSelect_ = SELECT::Resume;

}

UniqueScene PauseScene::Update(UniqueScene scene)
{
    controller_->Update();

    auto UsedController = controller_->GetControllerData();

    bool flag = false;
    if (UsedController[KeyID::Up][static_cast<int>(ControllerAge::Now)] && !UsedController[KeyID::Up][static_cast<int>(ControllerAge::Old)])
    {
        flag = true;
    }

    if (UsedController[KeyID::Down][static_cast<int>(ControllerAge::Now)] && !UsedController[KeyID::Down][static_cast<int>(ControllerAge::Old)])
    {
        flag = true;
    }

    if (flag)
    {
        if (nowSelect_ == SELECT::Resume)
        {
            nowSelect_ = SELECT::Select;
        }
        else if (nowSelect_ == SELECT::Select)
        {
            nowSelect_ = SELECT::Resume;
        }
        PlaySoundMem(lpSoundMng.GetID("./sound/select.mp3")[0], DX_PLAYTYPE_BACK);
        con_ = 1.0;
        plus_ = abs(plus_);
        fluffyTime_ = 0.0;
    }

    con_ += plus_;
    fluffyTime_ += lpSceneMng.GetDeltaTime();
    if (fluffyLimit <= fluffyTime_)
    {
        plus_ = -plus_;
        fluffyTime_ = 0.0;
    }

    if (UsedController[KeyID::Decide][static_cast<int>(ControllerAge::Now)] && !UsedController[KeyID::Decide][static_cast<int>(ControllerAge::Old)])
    {
        PlaySoundMem(lpSoundMng.GetID("./sound/decide.mp3")[0], DX_PLAYTYPE_BACK);
        if (nowSelect_ == SELECT::Resume)
        {
            ChangeSoundVolume(150);
            return std::move(ownerScene_);
        }
        else if (nowSelect_ == SELECT::Select)
        {
            ChangeSoundVolume(150);
            StopSound();
            return std::make_unique<FadeInOut>(1.0, std::move(scene), std::make_unique<SelectScene>());
        }
    }

    DrawScreen();

    return scene;
}

void PauseScene::DrawScreen(void)
{
    SetDrawScreen(screenID_);
    ClsDrawScreen();

    //背景
    DrawGraph(0, 0, gameBG_, true);
    DrawGraphF(PauseOffsetX, PauseOffsetY, lpImageMng.GetID("././Image/font/Pause.png")[0], true);

    Vector2 viewSize = lpSceneMng.GetViewSize();
    //Resume Game の位置
    Vector2 resumePos = { viewSize.x / 2, PauseOffsetY * 3.75f };
    //Return Game の位置
    Vector2 selectPos = { viewSize.x / 2, PauseOffsetY * 5.0f };

    if (nowSelect_ == SELECT::Resume)
    {
        DrawRotaGraph(resumePos.x,
            resumePos.y,
            con_, 0.0,
            lpImageMng.GetID("./Image/font/ResumeGame.png")[0],
            true);

        DrawRotaGraph(selectPos.x,
            selectPos.y,
            1.0, 0.0,
            lpImageMng.GetID("./Image/font/nonreturnselect.png")[0],
            true);
    }
    else if(nowSelect_ == SELECT::Select)
    {
        DrawRotaGraph(resumePos.x,
            resumePos.y,
            1.0, 0.0,
            lpImageMng.GetID("./Image/font/nonResumeGame.png")[0],
            true);

        DrawRotaGraph(selectPos.x,
            selectPos.y,
            con_, 0.0,
            lpImageMng.GetID("./Image/font/returnselect.png")[0],
            true);
    }
}

void PauseScene::Release(void)
{
    //DeleteGraph(gameBG_);
}
