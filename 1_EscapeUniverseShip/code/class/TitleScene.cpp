#include<DxLib.h>
#include "TitleScene.h"
#include"SelectScene.h"
#include"SceneManager.h"
#include"Other/ImageMng.h"
#include"Other/SoundMng.h"
#include"Transition/FadeInOut.h"
#include"Input/KeyInput.h"
#include"Input/PadInput.h"

constexpr int PressZsizeX = 389;
constexpr int PressZsizeY = 58;

constexpr int PressAsizeX = 504;
constexpr int PressAsizeY = 48;
constexpr double fluffyLimit = 0.25;

constexpr int TITLESIZEY = 384;

TitleScene::TitleScene()
{
    Init();
    DrawScreen();
}

TitleScene::~TitleScene()
{
    Release();
}

void TitleScene::Init()
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
    InitSound();

    PlaySoundMem(lpSoundMng.GetID("./sound/titlescene.mp3")[0], DX_PLAYTYPE_LOOP);
    con_ = 1.0;
    fluffyTime_ = 0.0;
    plus_ = 0.004;
    posY_ = -TITLESIZEY;
}

UniqueScene TitleScene::Update(UniqueScene scene)
{
    controller_->Update();
    auto UsedController = controller_->GetControllerData();
    if (posY_ != 50)
    {
        posY_ += 2;
        if (UsedController[KeyID::Decide][static_cast<int>(ControllerAge::Now)] && !UsedController[KeyID::Decide][static_cast<int>(ControllerAge::Old)])
        {
            posY_ = 50;
        }
    }
    else
    {
        if (UsedController[KeyID::Decide][static_cast<int>(ControllerAge::Now)] && !UsedController[KeyID::Decide][static_cast<int>(ControllerAge::Old)])
        {
            PlaySoundMem(lpSoundMng.GetID("./sound/decide.mp3")[0], DX_PLAYTYPE_BACK);
            StopSoundMem(lpSoundMng.GetID("./sound/titlescene.mp3")[0]);
            return std::make_unique<FadeInOut>(0.5,std::move(scene),std::make_unique<SelectScene>());
        }
        con_ += plus_;
        fluffyTime_ += lpSceneMng.GetDeltaTime();
        if (fluffyLimit <= fluffyTime_)
        {
            plus_ = -plus_;
            fluffyTime_ = 0.0;
        }
    }
    DrawScreen();

    return scene;
}

void TitleScene::DrawScreen(void)
{
    SetDrawScreen(screenID_);
    ClsDrawScreen();

    //背景
    DrawGraph(0, 0, lpImageMng.GetID("./Image/BG.png")[0], true);
    DrawGraph(157, posY_, lpImageMng.GetID("./Image/font/title2.png")[0], true);

    auto viewsize = lpSceneMng.GetViewSize();

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

void TitleScene::Release(void)
{
    //DeleteGraph(screenID_);
}

void TitleScene::InitSound(void)
{
    ChangeVolumeSoundMem(150, lpSoundMng.GetID("./sound/decide.mp3")[0]);
    ChangeVolumeSoundMem(150, lpSoundMng.GetID("./sound/titlescene.mp3")[0]);
    ChangeVolumeSoundMem(150, lpSoundMng.GetID("./sound/early.mp3")[0]);
    ChangeVolumeSoundMem(150, lpSoundMng.GetID("./sound/mid.mp3")[0]);
    ChangeVolumeSoundMem(150, lpSoundMng.GetID("./sound/last.mp3")[0]);
    ChangeVolumeSoundMem(150, lpSoundMng.GetID("./sound/selectscene.mp3")[0]);
    ChangeVolumeSoundMem(200, lpSoundMng.GetID("./sound/coll.mp3")[0]);
    ChangeVolumeSoundMem(180, lpSoundMng.GetID("./sound/fall.mp3")[0]);
    ChangeVolumeSoundMem(180, lpSoundMng.GetID("./sound/big.mp3")[0]);
    ChangeVolumeSoundMem(180, lpSoundMng.GetID("./sound/small.mp3")[0]);
    ChangeVolumeSoundMem(180, lpSoundMng.GetID("./sound/drum.mp3")[0]);
    ChangeVolumeSoundMem(180, lpSoundMng.GetID("./sound/checkPoint.mp3")[0]);
    ChangeVolumeSoundMem(150, lpSoundMng.GetID("./sound/clearscene.mp3")[0]);
    ChangeVolumeSoundMem(180, lpSoundMng.GetID("./sound/dash.mp3")[0]);
}
