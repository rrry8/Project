#pragma once
#include "../Obj.h"
#include"../../common/Geometry.h"
#include"../../common/Transform.h"
#include"../../common/Capsule.h"

class Stamina;
class Controller;
class MinCamera;
struct GimmickObj;
class Gimmick;
struct ItemObj;
struct InventoryObj;
class Item;
enum class ItemID;
enum class ControllerType;
class HandLight;

enum class State
{
    Walk,   //歩く
    Fall,   //落下
    Max
};


class Player :
    public Obj
{
public:
    Player(ControllerType type, const std::vector<std::shared_ptr<Gimmick>>& gList, const std::vector<std::shared_ptr<Item>>& iList);
    ~Player();

    //初期化
    void Init(void) override;
    //破棄
    void Release(void) override;
    //更新
    void Update(void) override;
    void Update(Vector3 cameraAngle,Vector3 targetPos);
    //描画
    void Draw(void) override;
    void CameraDraw(void);
    //位置を取得
    Vector3 GetPosition(void);
    bool GetIsOnCamera();
    bool GetIsGameClear();

    //ダッシュをしているか返す
    bool GetDashFlag(void);

    //カメラのレンダリングターゲット設定
    void SetMinCameraRenderTarget(int screenID);

    //フラッシュの当たり判定
    bool CheckHitFlash(Vector3 targetPos);

    //カメラの写真情報を返す
    std::vector<int>& GetMinCameraPhotoVector(void);

    //当たり判定用の変数を返す
    std::vector<Collider>& GetColliders(void);
    std::vector<InventoryObj>& GetInventoryList(void);
    Capsule GetCapsule(void);

private:
    //移動処理
    void MovePosition(Vector3 cameraAngle);
    //斜め移動の距離制限処理
    bool CheckDiagonalMove(void);

    //ステージとの当たり判定
    void CollisionStage(void);
    //ギミックとの当たり判定
    void CollisionGimmick(void);

    void SlideLinePosition(void);

    void HitCapsuleAndGoal(void);

    void AddInventory(Vector3 targetPos);

    void InteractGimmick(Vector3 targetPos);
   
    void GimmickEvent(Vector3 targetPos);
    void DialCountUpdate(std::shared_ptr<Gimmick> &gimmick,int modelNum,char numchar);
    //位置
    Vector3 pos_;

    //回転
    Quaternion quaRot_;

    VECTOR PlayerRot_;
    //情報
    std::unique_ptr<Controller> controller_; //コントローラ   
    std::unique_ptr<Stamina> stamina_;       //スタミナ

    bool pickUpFlag_;

    //当たり判定用
    std::vector<Collider> colliders_;   //マップ関係
    Capsule capsule_;  //プレイヤーのカプセル
    std::unique_ptr<MinCamera> minCamera_;  //プレイヤーのカプセル
    //状態
    State state_;
    float fallGravity_;

    int collisionHandle_;
    std::vector<std::shared_ptr<Gimmick>> gimmickList_;
    std::vector<std::shared_ptr<Item>> itemList_;

    std::vector<InventoryObj> inventoryList_;
    int dial_[4];
    //ダイヤル番号
    std::list<int> dial2_{0,0,0,0};
    //ダイヤル番号(文字列)
    std::string dialNumStr_="0000";
    bool dashFlag_;

    std::unique_ptr<HandLight> handLight_;
    bool isGameClear_;
    bool isHitGoal_;
};
