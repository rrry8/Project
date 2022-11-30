#include<DxLib.h>
#include "ResultScene.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "../../_debug/_DebugConOut.h"


#include "../input/PadInput.h"
#include "../input/KeyInput.h"
#include "../tmx/TmxObj.h"
#include "../common/manager/ImageMng.h"
#include "../common/CheckHit.h"

ResultScene::ResultScene(ControllerType type)
{
    ContorollerType_ = type;
    Init();
    DrawScreen();
}

ResultScene::~ResultScene()
{
}

void ResultScene::Init(void)
{
    tmxObj_.LoadTmx("resource/tmx/result.tmx", false);

    
    if (ContorollerType_ == ControllerType::Pad)
    {
        controller_ = std::make_unique<PadInput>();
    }
    else
    {
        controller_ = std::make_unique<KeyInput>();
    }

    resultList_ =
    {
        {"resultCongratulation","resultCongratulation"},
        {"resultLogo","gameclear"},
        {"resultTitle","Totitle"},
        {"resultMouse",""},
        {"resultTab",""},
        {"resultOperation",""}
    };
}

UniqueScene ResultScene::Update(UniqueScene scene)
{
    if (CheckHitKey(KEY_INPUT_SPACE))
    {
        return std::make_unique<TitleScene>();
    }
    controller_->Update();
    Vector2Int pos;
    GetMousePoint(&pos.x, &pos.y);
    if (controller_->CheckInputKey(KeyID::Decide))
    {
        for (auto& coll : tmxObj_.GetResultCollList())
        {
            if (CheckCollMouse()(pos, coll.second))
            {
                if (coll.first == "resultTitle")
                {
                    return std::make_unique<TitleScene>();
                }
            }
        }
    }
    DrawScreen();
    return scene;
}

void ResultScene::DrawScreen(void)
{
    SetDrawScreen(screenID_);
    ClsDrawScreen();
    DrawBox(0, 0, 1920, 1080,
        0x776667,
        true);
    DrawFormatString(0, 0, 0xffffff, "ResultScene");
    DrawFormatString(lpSceneMng.GetScreenSize().x/2, lpSceneMng.GetScreenSize().y / 2, 0xffffff, "ゲームクリア！");
    DrawFormatString(lpSceneMng.GetScreenSize().x / 2, lpSceneMng.GetScreenSize().y / 2+50, 0xffffff, "スペースでゲームシーンに戻る");

    for (auto& coll : tmxObj_.GetResultCollList())
    {
        Vector2 cen = { coll.second.first.x + coll.second.second.x / 2 ,coll.second.first.y + coll.second.second.y / 2 };
        DrawBox(coll.second.first.x, coll.second.first.y, coll.second.first.x + coll.second.second.x, coll.second.first.y + coll.second.second.y, 0x552222, true);
        for (auto& list : resultList_)
        {
            if (coll.first == list.first)
            {
                Vector2Int GraphSize;
                GetGraphSize(lpImageMng.GetID(list.second)[0], &GraphSize.x, &GraphSize.y);
                GraphSize.x /= 2;
                GraphSize.y /= 2;
                DrawGraph(cen.x - GraphSize.x, cen.y - GraphSize.y, lpImageMng.GetID(list.second)[0], true);
            }
        }



            
        continue;


    }
    //TRACE("ResultScene\n");
}

void ResultScene::Release(void)
{
}
