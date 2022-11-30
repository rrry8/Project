#include<DxLib.h>
#include "InventoryScene.h"
#include "SceneManager.h"
#include "../object/Item/Item.h"

#include "../input/PadInput.h"
#include "../input/KeyInput.h"

#include "../tmx/TmxObj.h"
#include "../common/CheckHit.h"
#include "../common/Geometry.h"

#include "../common/manager/ModelMng.h"
#include "../common/manager/ImageMng.h"
#include"../common/OriginalShader.h"
#include "../common/SocketServer.h"

#include "../../_debug/_DebugConOut.h"

//ぼかしの度合い
constexpr float BLUR_DEGREE = 8.0f;

//↓何のオフセットか説明求
constexpr float OFFSET = 50.0f;

//写真にマウスを合わせたときに追加する値
constexpr float   PHOTO_ADD_POS = 5.0f;

//フレームの色(仮)
constexpr int   FRAME_COLOR = 125;


InventoryScene::InventoryScene(UniqueScene beforeScene,int bgHandle,ControllerType type, const std::vector<InventoryObj>& inventoryList, const std::vector<int>& photo)
{
    beforeScene_ = std::move(beforeScene);

    if (type == ControllerType::Pad)
    {
        controller_ = std::make_unique<PadInput>();
    }
    else
    {
        controller_ = std::make_unique<KeyInput>();
    }

    //背景
    backGroundHandle_ = bgHandle;
    controller_->Update();

    inventoryList_ = inventoryList;
    photoVector_ = photo;

    Init();

    //ちらつき防止
    SetDrawScreen(screenID_);
    ClsDrawScreen();
    DrawPrimitive2D(totalBlurVerts_.data(), totalBlurVerts_.size(), DX_PRIMTYPE_TRIANGLESTRIP, backGroundHandle_, false);
}

InventoryScene::~InventoryScene()
{
    Release();
}

void InventoryScene::Init(void)
{
    //テーブル初期化
    InitTable();

    psBlurXandYHandle_ = LoadPixelShader("./resource/shader/ps/blur_ps.pso");
    mainRenderTarget_ = MakeScreen(screenSizeX_, screenSizeY_);

    for (auto& v : totalBlurVerts_)
    {
        v.rhw = 1.0f;
        v.dif = GetColorU8(255, 255, 255, 255);
        v.pos.z = 0.0f;
    }

    totalBlurVerts_[0].pos.x = 0.0f;
    totalBlurVerts_[0].pos.y = 0.0f;

    totalBlurVerts_[1].pos.x = static_cast<float>(screenSizeX_);
    totalBlurVerts_[1].pos.y = 0.0f;

    totalBlurVerts_[2].pos.x = 0.0f;
    totalBlurVerts_[2].pos.y = static_cast<float>(screenSizeY_);

    totalBlurVerts_[3].pos.x = static_cast<float>(screenSizeX_);
    totalBlurVerts_[3].pos.y = static_cast<float>(screenSizeY_);

    totalBlurVerts_[0].u = 0.0f;
    totalBlurVerts_[0].v = 0.0f;

    totalBlurVerts_[1].u = 1.0f;
    totalBlurVerts_[1].v = 0.0f;

    totalBlurVerts_[2].u = 0.0f;
    totalBlurVerts_[2].v = 1.0f;

    totalBlurVerts_[3].u = 1.0f;
    totalBlurVerts_[3].v = 1.0f;

    //重み計算
    CalcBlurWeights();

    for (int n = 0; n < WEIGHTS_NUM; n++)
    {
        lpSceneMng.SetWriterData(n, weights_[n]);
    }

    InitBlur();

    if (lpSceneMng.GetInventoryCnt() ==1)
    {
        isDrawTips_ = true;
    }

    //写真関係
    psPhotoHandle_  = LoadPixelShader("./resource/shader/ps/Memo_ps.pso");
    photo_          = std::make_unique<OriginalShader>(psPhotoHandle_);
    photoAddPos_ = 0.0f;

    InitInventory();
}

void InventoryScene::InitBlur(void)
{
    //横ブラー
    //横幅の解像度を画面サイズの半分に
    blurX_.renderTarget = MakeScreen(screenSizeX_ / 2, screenSizeY_);
    blurX_.vsHandle = LoadVertexShader("./resource/shader/vs/blurX_vs.vso");

    for (auto& v : blurX_.verts)
    {
        v.dif = GetColorU8(255, 255, 255, 255);
        v.spc = GetColorU8(0, 0, 0, 0);
    }
    //左上
    blurX_.verts[0].pos.x = 0.0f;
    blurX_.verts[0].pos.y = static_cast<float>(screenSizeY_);
    //右上
    blurX_.verts[1].pos.x = static_cast<float>(screenSizeX_ / 2);
    blurX_.verts[1].pos.y = static_cast<float>(screenSizeY_);
    //左下
    blurX_.verts[2].pos.x = 0.0f;
    blurX_.verts[2].pos.y = 0.0f;
    //右下
    blurX_.verts[3].pos.x = static_cast<float>(screenSizeX_ / 2);
    blurX_.verts[3].pos.y = 0.0f;
    //UV座標
    blurX_.verts[0].u = 0.0f;
    blurX_.verts[0].v = 0.0f;

    blurX_.verts[1].u = 1.0f;
    blurX_.verts[1].v = 0.0f;

    blurX_.verts[2].u = 0.0f;
    blurX_.verts[2].v = 1.0f;

    blurX_.verts[3].u = 1.0f;
    blurX_.verts[3].v = 1.0f;

    //縦ブラー
    //縦ブラーは縦幅の解像度を画面サイズの半分に
    blurY_.renderTarget = MakeScreen(screenSizeX_, screenSizeY_ / 2);
    blurY_.vsHandle = LoadVertexShader("./resource/shader/vs/blurY_vs.vso");

    for (auto& v : blurY_.verts)
    {
        v.dif = GetColorU8(255, 255, 255, 255);
        v.spc = GetColorU8(0, 0, 0, 0);
    }
    //左上
    blurY_.verts[0].pos.x = 0.0f;
    blurY_.verts[0].pos.y = static_cast<float>(screenSizeY_ / 2);
    //右上
    blurY_.verts[1].pos.x = static_cast<float>(screenSizeX_);
    blurY_.verts[1].pos.y = static_cast<float>(screenSizeY_ / 2);
    //左下
    blurY_.verts[2].pos.x = 0.0f;
    blurY_.verts[2].pos.y = 0.0f;
    //右下
    blurY_.verts[3].pos.x = static_cast<float>(screenSizeX_);
    blurY_.verts[3].pos.y = 0.0f;

    //UV座標
    blurY_.verts[0].u = 0.0f;
    blurY_.verts[0].v = 0.0f;

    blurY_.verts[1].u = 1.0f;
    blurY_.verts[1].v = 0.0f;

    blurY_.verts[2].u = 0.0f;
    blurY_.verts[2].v = 1.0f;

    blurY_.verts[3].u = 1.0f;
    blurY_.verts[3].v = 1.0f;

}

UniqueScene InventoryScene::Update(UniqueScene scene)
{
    controller_->Update();
    
    if (controller_->CheckInputKey(KeyID::Inventory))
    {
        //マウスカーソルを真ん中に戻してカメラ回転をしないようにする
        //真ん中に戻さなかった場合はインベントリから戻った際に動かした分回転してしまう
        SetMousePoint(lpSceneMng.GetScreenSize().x / 2, lpSceneMng.GetScreenSize().y / 2);
        return std::move(beforeScene_);
    }

    UpdateInventory();
    
    //デバッグ用
    if (CheckHitKey(KEY_INPUT_6))
    {
        isDrawTips_ = false;
    }

    DrawScreen();
    return scene;
}

void InventoryScene::DrawScreen(void)
{
    //メインレンダリングターゲットに描画
    SetDrawScreen(mainRenderTarget_);
    ClsDrawScreen();

    //背景
    DrawPrimitive2D(totalBlurVerts_.data(), totalBlurVerts_.size(), DX_PRIMTYPE_TRIANGLESTRIP, backGroundHandle_, false);

    //横ブラーをかける
    SetDrawScreen(blurX_.renderTarget);
    ClsDrawScreen();
    DrawBlur(blurX_.vsHandle, psBlurXandYHandle_, mainRenderTarget_, blurX_.verts); //メインレンダリングターゲットのハンドル

    //縦ブラーをかける
    SetDrawScreen(blurY_.renderTarget);
    ClsDrawScreen();
    DrawBlur(blurY_.vsHandle, psBlurXandYHandle_, blurX_.renderTarget, blurY_.verts);  //横ブラーのハンドル

    //最終描画
    SetDrawScreen(screenID_);
    ClsDrawScreen();

    
    //縦ブラーのものを描画
    DrawPrimitive2D(totalBlurVerts_.data(), totalBlurVerts_.size(), DX_PRIMTYPE_TRIANGLESTRIP, blurY_.renderTarget, false);
    DrawString(0, 150, "インベントリ", 0x00ffff);

    DrawInventory();
}

void InventoryScene::Release(void)
{
    DeleteGraph(mainRenderTarget_);
    DeleteGraph(blurX_.renderTarget);
    DeleteGraph(blurY_.renderTarget);

    DeleteShader(psBlurXandYHandle_);
    DeleteShader(blurX_.vsHandle);
    DeleteShader(blurY_.vsHandle);
    DeleteShader(psPhotoHandle_);
}

void InventoryScene::CalcBlurWeights(void)
{
    float totalWeight = 0.0f;

    for (int x = 0; x < WEIGHTS_NUM; x++)
    {
        weights_[x] = expf(-0.5f * static_cast<float>(x * x) / BLUR_DEGREE);
        totalWeight += 2.0f * weights_[x];
    }

    for (int i = 0; i < WEIGHTS_NUM; i++)
    {
        weights_[i] /= totalWeight;
    }
}

void InventoryScene::CollMousePosAndPhoto(void)
{
    //マウスの位置取得
    Vector2Int mPos;
    GetMousePoint(&mPos.x, &mPos.y);

    for (const auto& coll : tmxObj_.GetInventoryCollList())
    {
        //マウスの位置と写真の位置が同じかどうか判定
        if (CheckCollMouse()(mPos, coll.second))
        {
            std::string collName = coll.first;

            //衝突場所が写真ではない場合はcontinue
            if (collName != "photo1" &&
                collName != "photo2" &&
                collName != "photo3")
            {
                continue;
            }
            photoAddPos_ = 5.0f;
        }
    }
}

void InventoryScene::DrawBlur(int vs, int ps,int target,const std::array<VERTEX3DSHADER, 4>& verts)
{
    SetUseVertexShader(vs);
    SetUsePixelShader(ps);
    SetUseTextureToShader(0, target);
    DrawPrimitive3DToShader(verts.data(), verts.size(),DX_PRIMTYPE_TRIANGLESTRIP);
}

void InventoryScene::DrawPhoto(std::string name, Vector2 pos, Vector2 imageSize)
{
    //写真ではない場合は返す
    if (name != "photo1"&&
        name != "photo2"&&
        name != "photo3")
    {
        return;
    }

    //頂点情報
    std::array<Vector3, 4> vertPos;

    vertPos[0] = Vector3(pos.x, pos.y, 0.0f);
    vertPos[1] = Vector3(pos.x + imageSize.x, pos.y, 0.0f);
    vertPos[2] = Vector3(pos.x, pos.y + imageSize.y, 0.0f);
    vertPos[3] = Vector3(pos.x + imageSize.x, pos.y + imageSize.y, 0.0f);

    //写真の数値を抽出
    int num = atoi(name.substr(name.length() - 1, 1).c_str());

    //配列に数値を合わせて描画
    photo_->DrawOnlyPixel(vertPos, photoVector_[num - 1]);
}

void InventoryScene::InitInventory(void)
{
    //インベントリのデータ読み込み
    tmxObj_.LoadTmx("resource/tmx/inventory.tmx", false);
    for (auto& inventory : inventoryList_)
    {
        //値の初期化とサイズの調整
        inventory.pos_ = { 0.0f,0.0f,0.0f };
        MV1SetScale(lpModelMng.GetID(inventory.modelString_)[inventory.modelNum_], VScale(inventory.scale_.toVECTOR(), 0.1));
    }

    for (auto& coll : tmxObj_.GetInventoryCollList())
    {
        for (auto& itemTable : itemTable_)
        {
            if (coll.first == itemTable.first)
            {
                for (auto& inventory : inventoryList_)
                {
                    //値のセット
                    if (inventory.pos_ == Vector3{ 0.0f,0.0f,0.0f })
                    {
                        //値とインベントリの番地のセット
                        inventory.pos_ = { coll.second.first.x + OFFSET,lpSceneMng.GetScreenSize().y - coll.second.first.y - OFFSET,0 };
                        inventory.inventoryNum_ = itemTable.first;
                        break;
                    }
                }
            }
        }
    }
}

void InventoryScene::UpdateInventory(void)
{
    hitItemID_ = ItemID::Non;
    Vector2Int mPos;
    GetMousePoint(&mPos.x, &mPos.y);

    for (auto& coll : tmxObj_.GetInventoryCollList())
    {
        for (auto& inventory : inventoryList_)
        {
            if (coll.first != inventory.inventoryNum_)
            {
                continue;
            }
            for (auto& itemTable : itemTable_)
            {
                if (itemTable.first != inventory.inventoryNum_)
                {
                    continue;
                }
                //マウスの当たった所の色を変える
                //どのIDか格納
                if (CheckCollMouse()(mPos, coll.second))
                {
                    hitItemID_ = inventory.itemID_;
                    itemTable.second = { 255,255,255 };
                }
                else
                {
                    itemTable.second = { 125,125,125 };
                }
               
                if (inventory.inventoryNum_ == lpSocketServer.GetinventorySelect())
                {
                    hitItemID_ = inventory.itemID_;
                    itemTable.second = { 255,255,255 };
                }
            }
            
        }
    }
    
    for (auto& inventory : inventoryList_)
    {
        MV1SetPosition(lpModelMng.GetID(inventory.modelString_)[inventory.modelNum_], inventory.pos_.toVECTOR());
    }
}

void InventoryScene::DrawInventory(void)
{
    int color = FRAME_COLOR;
    for (auto& coll : tmxObj_.GetInventoryCollList())
    {
        for (auto& inventory : inventoryTable_)
        {
            if (coll.first == inventory.first)
            {
                //ストリングに何も入ってなかったら普通の四角を書く
                if (inventory.second == "")
                {
                    DrawBox(coll.second.first.x,
                        coll.second.first.y,
                        coll.second.first.x + coll.second.second.x,
                        coll.second.first.y + coll.second.second.y,
                        GetColor(color, color, color), false);
                    color += 1;
                }
                else
                {
                    //入っていたらそれを描画する
                    DrawExtendGraph(coll.second.first.x,
                        coll.second.first.y,
                        coll.second.first.x + coll.second.second.x,
                        coll.second.first.y + coll.second.second.y, lpImageMng.GetID(inventory.second)[0], true);
                    break;
                }
            }

        }
        //アイテムの枠の描画
        for (auto& itemTable : itemTable_)
        {
            if (coll.first == itemTable.first)
            {
                DrawBox(coll.second.first.x,
                    coll.second.first.y,
                    coll.second.first.x + coll.second.second.x,
                    coll.second.first.y + coll.second.second.y,
                    GetColor(itemTable.second.x, itemTable.second.y, itemTable.second.z), false);
            }
        }
        //写真描画
        DrawPhoto(coll.first, { coll.second.first }, { coll.second.second });
    }
    //モデル描画
    for (auto& inventory : inventoryList_)
    {
        MV1DrawModel(lpModelMng.GetID(inventory.modelString_)[inventory.modelNum_]);
    }
    //文字の描画（今はストリング最終的に画像）
    for (auto& coll : tmxObj_.GetInventoryCollList())
    {
        if (coll.first == "explanation")
        {
            for (auto& explanation : explanationTable_)
            {
                //ID確認
                if (hitItemID_ == explanation.first)
                {
                    DrawFormatString(coll.second.first.x, coll.second.first.y, 0xffffff, "%s", explanation.second.c_str());

                }
            }
        }
        if (coll.first == "target")
        {
            DrawFormatString(coll.second.first.x + OFFSET, coll.second.first.y + OFFSET, 0x000000, "玄関の鍵を見つけて脱出しよう");
            DrawFormatString(coll.second.first.x + OFFSET, coll.second.first.y + OFFSET*2, 0x000000, "探している物");
            DrawGraph(coll.second.first.x + OFFSET*5, coll.second.first.y + OFFSET * 1.5,lpImageMng.GetID("resource/image/marker/marker.png")[0], true);
        }
    }
    for (auto& coll : tmxObj_.GetInventoryCollList())
    {
        for (auto& option : optionTable_)
        {
            if (coll.first == option.first)
            {
                DrawFormatString(coll.second.first.x + OFFSET, coll.second.first.y + OFFSET / 2, 0xffff55, "%s", option.second.c_str());
                if (isDrawTips_)
                {
                    if(coll.first == "invtyTips")
                    DrawBox(coll.second.first.x,
                        coll.second.first.y,
                        coll.second.first.x + coll.second.second.x,
                        coll.second.first.y + coll.second.second.y,
                        GetColor(144, 144, 144), false);
                }
            }
        }
    }
}

void InventoryScene::InitTable(void)
{
    inventoryTable_ = {
            {"belongings",	"resource/image/inventory/Inventory.png"},
            {"target",	"resource/image/inventory/memo.png"},
            {"explanation",""}
    };
    Vector3 Zero = { 125,125,125 };
    itemTable_ = {
        {"1",Zero},
        {"2",Zero},
        {"3",Zero},
        {"4",Zero},
        {"5",Zero},
        {"6",Zero},
        {"7",Zero},
        {"8",Zero},
        {"9",Zero},
        {"10",Zero},
        {"11",Zero},
        {"12",Zero},
        {"13",Zero},
        {"14",Zero},
        {"15",Zero},
        {"16",Zero}
    };

    explanationTable_ = {
        {ItemID::Key1,"玄関の鍵"},
        {ItemID::Key2,"壊れた鍵"},
        {ItemID::Key3,"錆びた鍵"},
        {ItemID::Key4,"壊れた鍵"},
        {ItemID::Key5,"壊れた鍵"},
        {ItemID::Phot,"新聞"}/*,
        {ItemID::Key1,""},
        {ItemID::Key1,""},
        {ItemID::Key1,""},
        {ItemID::Key1,""},
        {ItemID::Key1,""},*/
    };
    optionTable_ = {
            {"tab",	"tabでゲームに戻る"},
            {"mouse","マウスを合わせてアイテムの確認"},
            {"invtyTips",""}
    };
}
