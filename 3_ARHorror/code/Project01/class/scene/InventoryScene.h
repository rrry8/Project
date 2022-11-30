#pragma once
#include "BaseScene.h"
#include <vector>
#include "../tmx/TmxObj.h"
#include "../common/Geometry.h"
#include "../common/SocketServer.h"

class Controller;
class MouseIcon;
class Item;
class OriginalShader;
struct InventoryObj;
enum class ItemID;
enum class ControllerType;

constexpr int WEIGHTS_NUM = 7;

//ブラー構造体
struct BlurData
{
    int renderTarget;   //レンダリングターゲット
    int vsHandle;       //頂点シェーダハンドル
    std::array<VERTEX3DSHADER, 4> verts{};  //Primitve用の頂点情報
};

class InventoryScene :
    public BaseScene
{
public:
    InventoryScene(UniqueScene ownScene,int bgHandle,ControllerType type, const std::vector<InventoryObj>& inventoryList, const std::vector<int>& photo);
    ~InventoryScene();

    // 初期化処理
    void Init(void) override;

    // 更新ステップ
    UniqueScene Update(UniqueScene scene) override;
    // 描画処理
    void DrawScreen(void) override;

    // 解放処理
    void Release(void) override;

    //シーンのIDを返す
    SceneID GetSceneID(void) override { return SceneID::Inventory; };

private:
    //ブラー関係初期化
    void InitBlur(void);

    //ブラーの重み計算
    void CalcBlurWeights(void);

    //写真とマウスの位置の当たり判定
    void CollMousePosAndPhoto(void);

    //ぼかし描画
    void DrawBlur(int vs,int ps,int target,const std::array<VERTEX3DSHADER, 4>& verts);
    //写真描画
    void DrawPhoto(std::string name,Vector2 pos,Vector2 imageSize);

    //インベントリ初期化
    void InitInventory(void);
    //インベントリ更新
    void UpdateInventory(void);
    //インベントリ描画
    void DrawInventory(void);
    //テーブルの初期化
    void InitTable(void);

    //コントローラ情報
    std::unique_ptr<Controller> controller_;

    //フレームバッファや背景描画用
    std::array<VERTEX2D, 4> totalBlurVerts_;
    
    //テーブル
    std::map<std::string , std::string> inventoryTable_;    //インベントリの外枠のテーブル
    std::map<std::string, std::string> optionTable_;        //操作のテーブル
    std::map<std::string, Vector3> itemTable_;              //アイテムの枠テーブル
    std::map<ItemID, std::string> explanationTable_;        //説明のテーブル

    // アイテムリスト
    std::vector<InventoryObj> inventoryList_;

    //写真関係
    std::vector<int> photoVector_;          //写真のハンドル
    std::unique_ptr<OriginalShader> photo_; //描画用シェーダクラス
    float photoAddPos_;

    //ブラー情報
    BlurData blurX_;    //横
    BlurData blurY_;    //縦

    //前のシーン(主にゲームシーン)
    UniqueScene beforeScene_;

    //背景画像(前のシーンの画面)
    int backGroundHandle_;

    //メインレンダリングターゲット
    int mainRenderTarget_;

    //ピクセルシェーダハンドル
    int psBlurXandYHandle_; //横ブラーと縦ブラー用
    int psPhotoHandle_;     //写真用

    //ぼかしの重み
    float weights_[WEIGHTS_NUM];

    TmxObj tmxObj_;
    ItemID hitItemID_;
    bool isDrawTips_;
};
