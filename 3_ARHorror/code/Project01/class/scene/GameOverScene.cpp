#include "GameOverScene.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "LoadScene.h"
#include"../camera/Camera.h"
#include "../input/PadInput.h"
#include "../input/KeyInput.h"
#include "../tmx/TmxObj.h"
#include "../common/manager/ImageMng.h"
#include "../common/CheckHit.h"
#include "SceneManager.h"
#include "../common/OriginalShader.h"
#include "../common/manager/ModelMng.h"

constexpr float LOGO_SIZE_X = 483.5f;
constexpr float LOGO_SIZE_Y = 244.5f;

constexpr int DRAW_OFFSET = 50;
GameOverScene::GameOverScene(UniqueScene ownScene, ControllerType type, SceneID beforeID, Vector3 playerPos, Vector3 enemyPos)
{
    beforeScene_ = std::move(ownScene);
    beforeSceneID_ = beforeID;

    if (type == ControllerType::Pad)
    {
        controller_ = std::make_unique<PadInput>();
    }
    else
    {
        controller_ = std::make_unique<KeyInput>();
    }

    playerPos_ = playerPos;
    enemyPos_ = enemyPos;

    Init();
    DrawScreen();
}

GameOverScene::~GameOverScene()
{
	Release();
}

void GameOverScene::Init(void)
{
    //ロード
    tmxObj_.LoadTmx("resource/tmx/gameover.tmx", false);
    lpImageMng.GetID("resource/image/Font/gameover.png","gameover");
    lpImageMng.GetID("resource/image/Font/restart.png","reStartTag");
    lpImageMng.GetID("resource/image/Font/totitle.png", "toTitleTag");
    lpImageMng.GetID("resource/image/gameover/Blood5.png", "selectB");
    
    InList();
    
    //カメラ
    camera_ = std::make_unique<Camera>();	
    camera_->Init();

    //srand(static_cast<unsigned int>(time(nullptr)));
    brightness_ = lpSceneMng.GetBrightnessScrren();//輝度
    Cnt_ = 0;//死亡演出の表示カウント

    //シェーダ関連
    int VertexShader = LoadVertexShader("resource/shader/vs/NormalModel_vs.vso");
    int PixelShader = LoadPixelShader("resource/shader/ps/GameModel_ps.pso");
    //int PixelShader = LoadPixelShader("resource/shader/ps/renderGBuffer.pso");
    originalShader_ = std::make_unique<OriginalShader>(VertexShader, PixelShader);
}

UniqueScene GameOverScene::Update(UniqueScene scene)
{
    

    if (!directionEndflg_)
    {
        //死亡演出が終わってなかったら
        //カメラの更新
        camera_->Update(enemyPos_);
        
    }
    else
    {
        //死亡演出が終わっていたら

        controller_->Update();//入力情報の更新
        //衝突判定
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
        //TRACE("volume %d\n", Volume_/2);
        lpSceneMng.SetBrightnessScrren(brightness_);
    }
    DrawScreen();
    ++Cnt_;
    if (Cnt_ > 180)
    {
        directionEndflg_ = true;
    }
	return scene;
}

void GameOverScene::DrawScreen(void)
{
    SetDrawScreen(screenID_);
    ClsDrawScreen();

    camera_->SetBeforeDraw(playerPos_);//プレイヤーの死亡した位置にカメラを描画

    if (!directionEndflg_)//死亡演出が終わっているかどうか
    {
        originalShader_->Draw(lpModelMng.GetID("resource/model/stage/map2.mv1")[0]);
    }
    else
    {
        DrawExtendGraph(0,
            0,
            lpSceneMng.GetScreenSize().x,
            lpSceneMng.GetScreenSize().y,
            lpImageMng.GetID("resource/image/gameOver/horrorBg.jpg")[0],
            true);
        //tmxからのコリジョンの場所に描画
        DrawColl();
    }
}

void GameOverScene::Release(void)
{
}

void GameOverScene::InList()
{
    //UIの情報をリストに格納
    displayList_ =
    {
        {"logo","gameover"},
    };
    
    toTitleList_ =
    {
        {"",""}
    };
    reStartList_ =
    {
        {"",""}
    };
    selectList_ =
    {
        {"reStartTag",&GameOverScene::UpdateReStart},
        {"toTitleTag",&GameOverScene::UpdateToTitle }
    };

    supportList_ =
    {
        {"toTitleTag","supportToTitle"},
        {"reStartTag","supportReStart"},
    };
}

void GameOverScene::HitCollAndAssignProcess()
{
    Vector2Int pos;
    GetMousePoint(&pos.x, &pos.y);
    for (auto& coll : tmxObj_.GetGameoverCollList())
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

void GameOverScene::HitCollClickAndAssignProcess(std::pair<std::string, Collision> coll)
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

std::pair<std::string, std::string> GameOverScene::HitCollMouse(std::pair<std::string, Collision> coll)
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



void GameOverScene::UpdateToTitle()//タイトルへ
{
    returningScene_ = std::make_unique<TitleScene>();
    DrawFuncPtr_ = &GameOverScene::DrawToTitle;
}

void GameOverScene::UpdateReStart()//リスタート
{
    returningScene_ = std::make_unique<GameScene>();
    DrawFuncPtr_ = &GameOverScene::DrawReStart;
}

void GameOverScene::DrawToTitle()//タイトルへ　を表示
{
    for (auto& coll : tmxObj_.GetGameoverCollList())
    {
        DrawList(coll, toTitleList_);
    }
}

void GameOverScene::DrawReStart()//リスタート　を表示
{
    for (auto& coll : tmxObj_.GetGameoverCollList())
    {
        DrawList(coll, reStartList_);
    }
}

void GameOverScene::DrawColl()//UIのコリジョン情報をもとにUIを配置
{
    for (auto& coll : tmxObj_.GetGameoverCollList())
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

void GameOverScene::DrawCollCenterY(std::pair<std::string, Collision> coll, std::string imageStr)//描画対象のY座標の原点を中心に移動させて描画
{
    Vector2Int GraphSize;
    Vector2 cen = { coll.second.first.x + coll.second.second.x / 2 ,coll.second.first.y + coll.second.second.y / 2 };

    GetGraphSize(lpImageMng.GetID(imageStr)[0], &GraphSize.x, &GraphSize.y);
    GraphSize.y /= 2;
    DrawGraph(coll.second.first.x, cen.y - GraphSize.y, lpImageMng.GetID(imageStr)[0], true);
}

void GameOverScene::DrawCollCenter(std::pair<std::string, Collision> coll, std::string imageStr)//描画対象を原点を中心にずらして描画
{
    Vector2Int GraphSize;
    Vector2 cen = { coll.second.first.x + coll.second.second.x / 2 ,coll.second.first.y + coll.second.second.y / 2 };

    GetGraphSize(lpImageMng.GetID(imageStr)[0], &GraphSize.x, &GraphSize.y);
    GraphSize.x /= 2;
    GraphSize.y /= 2;
    DrawGraph(cen.x - GraphSize.x, cen.y - GraphSize.y, lpImageMng.GetID(imageStr)[0], true);
}

void GameOverScene::DrawSelectList(std::pair<std::string, Collision> coll)
{
    //項目に触れているものを描画
    if (coll.first == hitMouse_.first)
    {
        //SetDrawBlendMode(DX_BLENDMODE_ALPHA, std::sin(DX_PI * 2 / 60 * Cnt_) * 300 + 300);
        if (Cnt_ < 255)
        {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, Cnt_*Cnt_);
        }
        DrawExtendGraph(coll.second.first.x - DRAW_OFFSET,
            coll.second.first.y,
            coll.second.first.x + coll.second.second.x - DRAW_OFFSET,
            coll.second.first.y + coll.second.second.y, lpImageMng.GetID("selectB")[0],
            true);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
    if (coll.first == "operation")
    {
        DrawCollCenter(coll, hitMouse_.second);
        _dbgDrawFormatString(0, 0, 0xffffff, "%s", hitMouse_.second.c_str());
    }
    ////項目をクリックしているものを描画
    //if (coll.first == selection_)
    //{
    //    DrawExtendGraph(coll.second.first.x - DRAW_OFFSET,
    //        coll.second.first.y,
    //        coll.second.first.x + coll.second.second.x - DRAW_OFFSET,
    //        coll.second.first.y + coll.second.second.y, lpImageMng.GetID("selectB")[0],
    //        true);
    //}

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

void GameOverScene::DrawList(std::pair<std::string, Collision> coll, std::map<std::string, std::string> maplist)
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
