#include<DxLib.h>
#include<string>
#include "SelectScene.h"
#include"GameScene.h"
#include"TitleScene.h"
#include"TutorialScene.h"
#include"Transition/FadeInOut.h"
#include"Transition/MoveCountDown.h"
#include"Obj/Stage.h"
#include"Input/Controller.h"
#include"Other/ImageMng.h"
#include"Other/SoundMng.h"

SelectScene::SelectScene()
{
    Init();
    DrawScreen();
}

SelectScene::~SelectScene()
{
}

void SelectScene::Init()
{
    stage_ = std::make_unique<Stage>(GetJoypadNum());
    PlaySoundMem(lpSoundMng.GetID("./sound/selectscene.mp3")[0], DX_PLAYTYPE_LOOP);
}

UniqueScene SelectScene::Update(UniqueScene scene)
{
    auto stage = stage_->Update();
    if (stage != STAGE::None)
    {
        std::string selectedStage = "./tmx/map.tmx";
        StopSoundMem(lpSoundMng.GetID("./sound/selectscene.mp3")[0]);
        switch (stage)
        {
        case STAGE::One:
            PlaySoundMem(lpSoundMng.GetID("./sound/early.mp3")[0], DX_PLAYTYPE_LOOP);
            break;
        case STAGE::Two:
            PlaySoundMem(lpSoundMng.GetID("./sound/early.mp3")[0], DX_PLAYTYPE_LOOP);
            break;
        case STAGE::Three:
            PlaySoundMem(lpSoundMng.GetID("./sound/early.mp3")[0], DX_PLAYTYPE_LOOP);
            break;
        case STAGE::Four:
            PlaySoundMem(lpSoundMng.GetID("./sound/early.mp3")[0], DX_PLAYTYPE_LOOP);
            break;
        case STAGE::Five:
            PlaySoundMem(lpSoundMng.GetID("./sound/mid.mp3")[0], DX_PLAYTYPE_LOOP);
            break;
        case STAGE::Six:
            PlaySoundMem(lpSoundMng.GetID("./sound/mid.mp3")[0], DX_PLAYTYPE_LOOP);
            break;
        case STAGE::Seven:
            PlaySoundMem(lpSoundMng.GetID("./sound/mid.mp3")[0], DX_PLAYTYPE_LOOP);
            break;
        case STAGE::Eight:
            PlaySoundMem(lpSoundMng.GetID("./sound/last.mp3")[0], DX_PLAYTYPE_LOOP);
            break;
        case STAGE::Tutorial:
            StopSoundMem(lpSoundMng.GetID("./sound/selectscene.mp3")[0]);
            PlaySoundMem(lpSoundMng.GetID("./sound/selectscene.mp3")[0], DX_PLAYTYPE_LOOP);
            DrawScreen();
            return std::make_unique<FadeInOut>(3.0, std::move(scene), std::make_unique<TutorialScene>());
            break;
        case STAGE::End:
            return std::make_unique<TitleScene>();
        }

        std::string font = "././Image/font/stage.png";
        std::string font2 = "././Image/font/nonstage.png";
        std::string num = std::to_string(static_cast<int>(stage));

        auto s = std::to_string(static_cast<int>(stage));
        Vector2 pos = stage_->GetPos();
        return std::make_unique<MoveCountDown>(0.0,
            font.insert(20, s),
            font2.insert(23, s),
            pos,
            std::move(scene),
            std::make_unique<GameScene>(selectedStage.insert(selectedStage.length() - 4, num))
            );
    }
    DrawScreen();

    return scene;
}

void SelectScene::DrawScreen(void)
{
    SetDrawScreen(screenID_);
    ClsDrawScreen();

    stage_->Draw();
}

void SelectScene::Release(void)
{
}
