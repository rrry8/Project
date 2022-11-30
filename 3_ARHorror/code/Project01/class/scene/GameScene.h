#pragma once
#include<vector>
#include<chrono>
#include<string>
#include "BaseScene.h"
#include "../common/Geometry.h"
#include "../common/Light.h"
#include "../tmx/TmxObj.h"

constexpr int DIRECTION_LIGHT_NUM = 2;  // ディレクションライトの数
constexpr int POINT_LIGHT_NUM = 3;   // ポイントライトの数

struct GameLight
{
    // ディレクションライト用のメンバ
    DirectionlLight directionLights[DIRECTION_LIGHT_NUM];
    PointLight       pointLights[POINT_LIGHT_NUM];
    SpotLight        spotLights;

    Vector3 ambientLight;   // アンビエントライト
    float specPow;
};
struct DOF
{
    Vector3 Focus;
    float pad0;
    Vector3 Area;
    float pad1;
};
enum class StatusUI
{
    Stop,
    MoveIn,
    MoveOut,
    Max
};
struct MoveUI
{
    Vector2 Destination;
    Vector2 Pos;
    StatusUI IsMove;
    int StopCnt;
};
class Camera;
class Controller;
class Stage;
class Player;
class Enemy;

class CBufferMng;
class SocketServer;
class Goal;
class Mirror;
class OriginalShader;

class GameScene :
    public BaseScene
{
public:
	GameScene();
	~GameScene();

    // 初期化処理
    void Init(void) override;

    // 更新ステップ
    UniqueScene Update(UniqueScene scene) override;

    // 描画処理
    void DrawScreen(void) override;

    // 解放処理
    void Release(void) override;

    //シーンのIDを返す
    SceneID GetSceneID(void) override { return SceneID::Game; };

private:
    //コントローラ情報
    std::unique_ptr<Controller> controller_;

    //ステージ情報
    std::unique_ptr<Stage> stage_;

    //カメラ情報
    std::unique_ptr<Camera> camera_;

    //プレイヤー情報
    std::unique_ptr<Player> player_;

    //敵情報
    std::unique_ptr<Enemy> enemy_;

    //ゴール情報
    std::unique_ptr<Goal> goal_;

    //鏡
    std::unique_ptr<Mirror> mirror_;


    std::unique_ptr<OriginalShader> originalShader_;
    int screenCameraID_;
    int GaussScreenCameraID_;
   
    //インベントリの背景ハンドル
    int inventoryBackGroundHandle_;
    bool onoff_;

    TmxObj tmxObj_;     //衝突判定用tmx

    // 被写界深度処理用のパラメータを初期化
    float DepthOfField_InterpSize_;
    float DepthOfField_Focus_;
    float DepthOfField_FocusSize_;

    // 補間範囲とぼかし無しの範囲の合計距離
    float DepthOfField_TotalSize_;

    DOF Dof_;

    GameLight light_;

    template<typename T>
    void DebugKeyMove(unsigned int keyUp, unsigned int keyDown, int value, T& body);

    void InitCBuffer(void);
    void InitLights(void);
    void InitUI(void);
    void UpdateCBuffer(void);

    void UIUpdate(void);
    void UIDraw(void);
    void DrawCollCenter(Vector2 CenterPos, std::string imageStr);

    std::map<std::string, MoveUI> UIVariableList_;
};

