#pragma once
#include<list>
#include "../Obj.h"
#include"../../common/Transform.h"
#include "../../common/Geometry.h"
#include"../../common/Capsule.h"

class AnimationController;
class Dijkstra;

struct NodeConnection;

//アニメーション
enum EnemyAnimation
{
    Idle,               //立ち
    Run,                //走る
    Damage,             //ひるみ
    FemaleDynamicPose,  //空飛び
    Walk,               //歩き
    Max
};

class Enemy :
    public Obj
{
public:
    Enemy();
    ~Enemy();

    //初期化
    void Init(void) override;
    //破棄
    void Release(void) override;
    //更新
    void Update(void) override;
    void Update(Capsule capsule,const Vector3 playerPos,bool hitFlash);
    //描画
    void Draw(void) override;

    Vector3 GetPosition(void);

    bool GetHitPlayerFlag(void);

private:
    //プレイヤーを探索する処理
    void SearchPlayer(const Vector3 playerPos);

    //プレイヤー探索(ture:発見/false:発見していない)
    bool Search(const Vector3 playerPos);

    //プレイヤーを追跡する処理
    void HuntPlayer(void);

    //ダイクストラ処理
    void DijkstraProcess(Vector3 playerPos);

    //衝突処理
    bool CheckHitPlayer(void);

    //フラッシュを受けたときの処理
    void DamageFlash(bool hitFlash);

    //フラッシュ状態から回復する処理
    void RecoverFlash(void);

    //スタン時間回復
    void RecoverStanTime(void);

    //スタン時間短縮
    void ReduceStanTime(void);

    //モデル制御
    Transform transform_;

    //アニメーションコントローラ
    std::unique_ptr<AnimationController> animController_;

    //追跡関係
    bool huntFlag_;         //プレイヤー発見フラグ
    double stepHuntTime_;   //追跡時間

    //目的地
    Vector3 targetPos_;
    
    //回転
    Quaternion targetRot_;  //目的の方向
    double stepRotTime_;    //回転時間

    //スタン関係
    bool stanFlag_;         //スタンフラグ
    float nowStanTime_;      //敵のスタン時間(スタンするほど短くなる)
    double stepTime_;   //スタン時間

    //ダイクストラ情報
    std::unique_ptr<Dijkstra> dijkstra_;

    //プレイヤーのカプセル情報
    Capsule playerCapsule_;

    //プレイヤー衝突フラグ
    bool hitPlayerFlag_;
};

