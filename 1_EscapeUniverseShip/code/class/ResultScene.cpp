#include<DxLib.h>
#include"SceneManager.h"
#include "ResultScene.h"
#include"Input/Controller.h"
#include"Input/KeyInput.h"
#include"Input/PadInput.h"
#include"GameScene.h"
#include"SelectScene.h"
#include"Transition/FadeInOut.h"
#include"Transition/RetryCount.h"
#include"SceneManager.h"
#include"Other/ImageMng.h"
#include"Other/SoundMng.h"

//�Q�[���I�[�o�[�t�H���g�̃T�C�Y
constexpr int GameoverSizeX = 657;
constexpr int GameoverSizeY = 337;

//���g���C�̃T�C�Y
constexpr int RetrySizeX = 274;
constexpr int RetrySizeY = 63;

//Return Select�̃T�C�Y
constexpr int SelectSizeX = 794;
constexpr int SelectSizeY = 72;

constexpr int offsetY = 50;

//�ӂ�ӂ�̕ω�����܂ł̎���
constexpr double fluffyLimit = 0.35;

ResultScene::ResultScene(UniqueScene scene,int id)
{
    ownerScene_ = std::move(scene);
    gameBG_ = id;
    Init();
    DrawScreen();
}

ResultScene::~ResultScene()
{
}

void ResultScene::Init(void)
{
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
    controller_->Update();

    con_ = 1.0;
    fluffyTime_ = 0.0;
    plus_ = 0.004;
    playerAngle_ = 0.0;

    nowSelect_ = SELECET::Retry;
}

UniqueScene ResultScene::Update(UniqueScene scene)
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
        if (nowSelect_ == SELECET::Retry)
        {
            nowSelect_ = SELECET::Select;
        }
        else if (nowSelect_ == SELECET::Select)
        {
            nowSelect_ = SELECET::Retry;
        }

        PlaySoundMem(lpSoundMng.GetID("./sound/select.mp3")[0], DX_PLAYTYPE_BACK);
        con_ = 1.0;
        plus_ = abs(plus_);
        fluffyTime_ = 0.0;
    }

    if (UsedController[KeyID::Decide][static_cast<int>(ControllerAge::Now)] && !UsedController[KeyID::Decide][static_cast<int>(ControllerAge::Old)])
    {
        PlaySoundMem(lpSoundMng.GetID("./sound/decide.mp3")[0], DX_PLAYTYPE_BACK);

        if (nowSelect_ == SELECET::Retry)
        {
            return std::make_unique<RetryCount>(1.0, std::move(scene), std::move(ownerScene_));
        }
        else if (nowSelect_ == SELECET::Select)
        {
            StopSound();
            return std::make_unique<FadeInOut>(1.0, std::move(scene), std::make_unique<SelectScene>());
        }
    }

    con_ += plus_;
    fluffyTime_ += lpSceneMng.GetDeltaTime();
    if (fluffyLimit <= fluffyTime_)
    {
        plus_ = -plus_;
        fluffyTime_ = 0.0;
    }

    playerAngle_ += 0.01;
    DrawScreen();
    return scene;
}

void ResultScene::DrawScreen(void)
{
    auto viewsize = lpSceneMng.GetViewSize();

    SetDrawScreen(screenID_);
    ClsDrawScreen();

    //�w�i
    DrawGraph(0, 0,gameBG_, true);

    //�Q�[���I�[�o�[�t�H���g
    DrawGraph(viewsize.x / 2 - GameoverSizeX / 2, offsetY, lpImageMng.GetID("./Image/font/gameover.png")[0], true);

    Vector2 retryPos = { viewsize.x / 2,GameoverSizeY + offsetY * 2 + RetrySizeY / 2 };
    Vector2 returnPos = { viewsize.x / 2,GameoverSizeY + RetrySizeY + offsetY * 3 + SelectSizeY / 2 };

    //�t�H���g�\��
    if (nowSelect_ == SELECET::Retry)
    {
        DrawRotaGraph(retryPos.x,
            retryPos.y,
            con_,0.0,
            lpImageMng.GetID("./Image/font/retry.png")[0],
            true);

        DrawRotaGraph(returnPos.x,
            returnPos.y,
            1.0, 0.0,
            lpImageMng.GetID("./Image/font/nonreturnselect.png")[0],
            true);
    }
    else if (nowSelect_ == SELECET::Select)
    {
        DrawRotaGraph(retryPos.x,
            retryPos.y,
            1.0, 0.0,
            lpImageMng.GetID("./Image/font/nonretry.png")[0],
            true);

        DrawRotaGraph(returnPos.x,
            returnPos.y,
            con_,0.0,
            lpImageMng.GetID("./Image/font/returnselect.png")[0],
            true);
    }
}

void ResultScene::Release(void)
{
}

