#include "OptionScene.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "../tmx/TmxObj.h"
#include "../input/PadInput.h"
#include "../input/KeyInput.h"

#include "../common/CheckHit.h"
#include "../common/manager/ImageMng.h"
#include "../common/manager/SoundMng.h"
#include "../common/manager/CBufferMng.h"
#include "../../_debug/_DebugDispOut.h"


constexpr int DRAW_OFFSET = 50;
OptionScene::OptionScene(UniqueScene ownScene,ControllerType type, SceneID beforeID)
{
    beforeScene_ = std::move(ownScene);
    
    if (type == ControllerType::Pad)
    {
        controller_ = std::make_unique<PadInput>();
    }
    else
    {
        controller_ = std::make_unique<KeyInput>();
    }
    beforeSceneID_ = beforeID;

    Init();
    DrawScreen();
}

OptionScene::~OptionScene()
{
    //音量の設定
    lpSoundMng.SetAllVolume(Volume_/2);
}

void OptionScene::Init(void)
{
    //tmxロード
    tmxObj_.LoadTmx("resource/tmx/option.tmx", false);
    //画像にフィルターをかける
    InitGraphFilter();
    //リストに格納
    InList();
    //変数の初期化
    InitVariable();
   
}

UniqueScene OptionScene::Update(UniqueScene scene)
{
    //コントローラ
    controller_->Update();
    
    //衝突判定と押したボタンの代入
    HitCollAndAssignProcess();

    //関数ポインタの実装
    if (UpdateFuncPtr_ != nullptr)
    {
        (this->*UpdateFuncPtr_)();
    }

    //関数ポインタで返すシーンが出来たらそれを返す
    if (returningScene_ != nullptr)
    {
        return std::move(returningScene_);
    }

    //時間の更新
    Cnt_++;

    //明るさの更新
    lpSceneMng.SetBrightnessScrren(brightness_);

    DrawScreen();
    return scene;
}

void OptionScene::DrawScreen(void)
{
    SetDrawScreen(screenID_);
    ClsDrawScreen();
    //背景描画
    DrawExtendGraph(0,
        0,
        lpSceneMng.GetScreenSize().x,
        lpSceneMng.GetScreenSize().y,
        lpImageMng.GetID("resource/image/option/bg1.png")[0],
        true);
    //tmxからのコリジョンの場所に描画
    DrawColl();
}

void OptionScene::Release(void)
{
}

void OptionScene::InitGraphFilter()
{
    GraphFilter(lpImageMng.GetID("resource/image/option/bg1.png")[0], DX_GRAPH_FILTER_GAUSS, 16, 500);
    GraphFilter(lpImageMng.GetID("resource/image/option/bg1.png")[0], DX_GRAPH_FILTER_HSB, 0, 0, 0, -150);
    GraphFilter(lpImageMng.GetID("selection")[0], DX_GRAPH_FILTER_HSB, 0, 0, 0, 205);
    GraphFilter(lpImageMng.GetID("selection1")[0], DX_GRAPH_FILTER_HSB, 0, 0, 0, 100);
    GraphFilter(lpImageMng.GetID("selection2")[0], DX_GRAPH_FILTER_HSB, 0, 0, 0, 256);
}

void OptionScene::InList()
{
    displayList_ =
    {
        {"optionTag","optionTag"},
    };
    controlList_ =
    {
        {"key","key"},
        {"keyboard","select5"},
        {"smp","smp"},
        {"smartphone","select5"}
    };

    audioList_ =
    {
        {"volume","volume"},
        {"masterValume","masterValume"},
        {"valumeControl","select5"},
        {"non","select5"}
    };

    graphicList_ =
    {
        {"brightness","screenBrightness"},
        {"screenBrightness","brightness"},
        {"brightnessAdjustment","select5"},
    };
    titleRetureList_ =
    {
        {"",""}
    };
    retureList_ =
    {
        {"",""}
    };
    selectList_ =
    {
        {"controlTag",&OptionScene::UpdateControl},
        {"audioTag",&OptionScene::UpdateAudio },
        {"graphicTag",&OptionScene::UpdateGraphic},
        {"titleReturnTag",&OptionScene::UpdateToTitle},
        {"returnTag",&OptionScene::UpdateReturnScreen}
    };

    supportList_ =
    {
        {"controlTag","supportControl"},
        {"audioTag","supportAudio"},
        {"graphicTag","supportGraphic"},
        {"titleReturnTag","supportTitleReturn"},
        {"returnTag","supportReturn"},
    };
}

void OptionScene::InitVariable()
{
    Volume_ = lpSoundMng.GetAllVolume();
    if (Volume_ == 0)
    {
        Volume_ = 125;
    }
    brightness_ = lpSceneMng.GetBrightnessScrren();
    Cnt_ = 0;
}

void OptionScene::HitCollAndAssignProcess()
{
    Vector2Int pos;
    GetMousePoint(&pos.x, &pos.y);
    for (auto& coll : tmxObj_.GetOptionCollList())
    {
        if (CheckCollMouse()(pos, coll.second))
        {
            if (controller_->CheckInputKey(KeyID::Decide))
            {
                //クリックしたときに当たっていたら関数ポインタに処理を入れる
                HitCollClickAndAssignProcess(coll);
            }
            //マウスが当たっていたら当たっている画像のstringを代入する
            OldhitMouse_ = hitMouse_;
            hitMouse_ = HitCollMouse(coll);
            //当たったのが変わったときに表示カウントをリセットする
            if (OldhitMouse_ != hitMouse_)
            {
                Cnt_ = 0;
            }
        }
    }
}

void OptionScene::HitCollClickAndAssignProcess(std::pair<std::string, Collision> coll)
{
    //クリックしたときに当たっているか
    for (auto& select : selectList_)
    {
        if (coll.first == select.first)
        {
            //選択したリストをドローするためのリストに格納している
            selection_ = select.first;
            UpdateFuncPtr_ = select.second;
        }
    }
}

//マウスが当たっているか
std::pair<std::string, std::string> OptionScene::HitCollMouse(std::pair<std::string, Collision> coll)
{
    for (auto& select : supportList_)
    {
        if (coll.first == select.first)
        {
            return { select.first,select.second };
        }
    }
    return { "","" };
}

void OptionScene::UpdateControl()
{
    DrawFuncPtr_ = &OptionScene::DrawControl;
}

void OptionScene::UpdateAudio()
{
    Vector2Int pos;
    GetMousePoint(&pos.x, &pos.y);
    for (auto& coll : tmxObj_.GetOptionCollList())
    {
        if (CheckCollMouse()(pos, coll.second))
        {
            if (!controller_->CheckLongInputKey(KeyID::Decide))
            {
                continue;
            }
            for (auto& UpdateAudio : audioList_)
            {
                if (coll.first != UpdateAudio.first)
                {
                    continue;
                }
                Volume_ = pos.x - coll.second.first.x;
            }

        }
    }
    DrawFuncPtr_ = &OptionScene::DrawAudio;
}

void OptionScene::UpdateGraphic()
{
    Vector2Int pos;
    GetMousePoint(&pos.x, &pos.y);
    for (auto& coll : tmxObj_.GetOptionCollList())
    {
        if (CheckCollMouse()(pos, coll.second))
        {
            if (!controller_->CheckLongInputKey(KeyID::Decide))
            {
                continue;
            }
            for (auto& graphic : graphicList_)
            {
                if (coll.first != graphic.first)
                {
                    continue;
                }
                brightness_ = pos.x - coll.second.first.x;
            }

        }
    }
    DrawFuncPtr_ = &OptionScene::DrawGraphic;
}

void OptionScene::UpdateToTitle()
{
    if (SceneID::Game == beforeSceneID_)
    {
        returningScene_ = std::make_unique<TitleScene>();
    }
    DrawFuncPtr_ = &OptionScene::DrawToTitle;
}

void OptionScene::UpdateReturnScreen()
{
    returningScene_ = std::move(beforeScene_);
    DrawFuncPtr_ = &OptionScene::DrawReturnScreen;
}

void OptionScene::DrawControl()
{
    for (auto& coll : tmxObj_.GetOptionCollList())
    {
        DrawList(coll, controlList_);
    }
}

void OptionScene::DrawAudio()
{
    for (auto& coll : tmxObj_.GetOptionCollList())
    {
        for (auto& audio : audioList_)
        {
            //同じstringじゃなかったらはじく
            if (coll.first == audio.first)
            {

                if (audio.first == "volume")
                {
                    DrawCollCenterY(coll, audio.second);
                }
                if (audio.first == "masterValume")
                {
                    DrawCollCenterY(coll, audio.second);
                }
                if (audio.first == "non")
                {
                    /*DrawBox(coll.second.first.x,
                        coll.second.first.y,
                        coll.second.first.x + coll.second.second.x,
                        coll.second.first.y + coll.second.second.y,
                        0xffffff, true);*/
                    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 70);
                    DrawExtendGraph(coll.second.first.x - DRAW_OFFSET,
                        coll.second.first.y,
                        coll.second.first.x + coll.second.second.x + DRAW_OFFSET,
                        coll.second.first.y + coll.second.second.y, lpImageMng.GetID(audio.second)[0],
                        true);
                    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 125);

                }
                if (audio.first == "valumeControl")
                {
                    SetDrawBlendMode(DX_BLENDMODE_ADD, 230);
                    DrawExtendGraph(coll.second.first.x,
                        coll.second.first.y - 10,
                        coll.second.first.x + coll.second.second.x,
                        coll.second.first.y + coll.second.second.y + 10, lpImageMng.GetID(audio.second)[0],
                        true);
                    DrawExtendGraph(coll.second.first.x + 15,
                        coll.second.first.y - 10,
                        coll.second.first.x + Volume_ + 20,
                        coll.second.first.y + coll.second.second.y + 10, lpImageMng.GetID("Bbox")[0],
                        true);
                    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 125);
                }
                continue;
            }
        }
    }
}

void OptionScene::DrawGraphic()
{
    for (auto& coll : tmxObj_.GetOptionCollList())
    {
        for (auto& audio : graphicList_)
        {
            //同じstringじゃなかったらはじく
            if (coll.first == audio.first)
            {
                if (audio.first == "brightness")
                {
                    DrawCollCenterY(coll, audio.second);
                }
                if (audio.first == "screenBrightness")
                {
                    DrawCollCenterY(coll, audio.second);
                }
                if (audio.first == "brightnessAdjustment")
                {
                    SetDrawBlendMode(DX_BLENDMODE_ADD, 230);
                    DrawExtendGraph(coll.second.first.x,
                        coll.second.first.y - 10,
                        coll.second.first.x + coll.second.second.x,
                        coll.second.first.y + coll.second.second.y + 10, lpImageMng.GetID(audio.second)[0],
                        true);
                    DrawExtendGraph(coll.second.first.x + 15,
                        coll.second.first.y - 10,
                        coll.second.first.x + brightness_ + 20,
                        coll.second.first.y + coll.second.second.y + 10, lpImageMng.GetID("Bbox")[0],
                        true);
                    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 125);
                }
                continue;
            }
        }
    }
}

void OptionScene::DrawToTitle()
{
    for (auto& coll : tmxObj_.GetOptionCollList())
    {
        DrawList(coll, titleRetureList_);
    }
}

void OptionScene::DrawReturnScreen()
{
    for (auto& coll : tmxObj_.GetOptionCollList())
    {
        DrawList(coll, retureList_);
    }
}

void OptionScene::DrawColl()
{
    for (auto& coll : tmxObj_.GetOptionCollList())
    {
        //表示するだけの描画
        DrawList(coll, displayList_);

        //selectListだけの描画
        DrawSelectList(coll);

        //関数ポインタの実装
        if (DrawFuncPtr_ != nullptr)
        {
            (this->*DrawFuncPtr_)();
        }
        //デバッグ描画
        /*DrawBox(coll.second.first.x,
            coll.second.first.y,
            coll.second.first.x + coll.second.second.x,
            coll.second.first.y + coll.second.second.y,
            0x222222, false);*/
    }
}

void OptionScene::DrawCollCenterY(std::pair<std::string, Collision> coll, std::string imageStr)
{
    Vector2Int GraphSize;
    Vector2 cen = { coll.second.first.x + coll.second.second.x / 2 ,coll.second.first.y + coll.second.second.y / 2 };

    GetGraphSize(lpImageMng.GetID(imageStr)[0], &GraphSize.x, &GraphSize.y);
    GraphSize.y /= 2;
    DrawGraph(coll.second.first.x, cen.y - GraphSize.y, lpImageMng.GetID(imageStr)[0], true);
}

void OptionScene::DrawCollCenter(std::pair<std::string, Collision> coll, std::string imageStr)
{
    Vector2Int GraphSize;
    Vector2 cen = { coll.second.first.x + coll.second.second.x / 2 ,coll.second.first.y + coll.second.second.y / 2 };

    GetGraphSize(lpImageMng.GetID(imageStr)[0], &GraphSize.x, &GraphSize.y);
    GraphSize.x /= 2;
    GraphSize.y /= 2;
    DrawGraph(cen.x - GraphSize.x, cen.y - GraphSize.y, lpImageMng.GetID(imageStr)[0], true);
}

void OptionScene::DrawSelectList(std::pair<std::string, Collision> coll)
{
    //titleReturnTagだけゲームシーンのみで出したいのでゲーム以外だったら返す
    if (coll.first == "titleReturnTag")
    {
        if (beforeSceneID_ != SceneID::Game)
        {
            return;
        }
    }

    //項目に触れているものを描画
    if (coll.first == hitMouse_.first)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::sin(DX_PI * 2 / 60 * Cnt_) * 300 + 100);
        DrawExtendGraph(coll.second.first.x - DRAW_OFFSET,
            coll.second.first.y,
            coll.second.first.x + coll.second.second.x - DRAW_OFFSET,
            coll.second.first.y + coll.second.second.y, lpImageMng.GetID("selection1")[0],
            true);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
    if (coll.first == "operation")
    {
        if (hitMouse_.first == "titleReturnTag")
        {
            if (beforeSceneID_ != SceneID::Game)
            {
                return;
            }
        }
        DrawCollCenter(coll, hitMouse_.second);
    }
    //項目をクリックしているものを描画
    if (coll.first == selection_)
    {
        DrawExtendGraph(coll.second.first.x - DRAW_OFFSET,
            coll.second.first.y,
            coll.second.first.x + coll.second.second.x - DRAW_OFFSET,
            coll.second.first.y + coll.second.second.y, lpImageMng.GetID("selection")[0],
            true);
    }

    //選択項目の描画
    for (auto& select : selectList_)
    {
        //同じものじゃなかったら弾く
        if (coll.first != select.first)
        {
            continue;
        }

        //描画
        DrawCollCenterY(coll, select.first);
        continue;

    }
}

void OptionScene::DrawList(std::pair<std::string, Collision> coll, std::map<std::string, std::string>maplist)
{
    for (auto& draw : maplist)
    {
        //同じstringじゃなかったらはじく
        if (coll.first != draw.first)
        {
            continue;
        }
        DrawCollCenterY(coll, draw.second);
        continue;
    }
}

