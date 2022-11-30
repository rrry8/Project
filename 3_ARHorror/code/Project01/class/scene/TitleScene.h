#pragma once
#include "BaseScene.h"
#include "../tmx/TmxObj.h"
#include "../common/Geometry.h"
#include "../common/Light.h"

//constexpr int DIRECTION_LIGHT_NUM = 1;  // ディレクションライトの数
//constexpr int POINT_LIGHT_NUM = 1;   // ポイントライトの数
//constexpr int SPOT_LIGHT_NUM = 1;  // ディレクションライトの数
//
//struct Light
//{
//    // ディレクションライト用のメンバ
//    DirectionalLight directionLights[DIRECTION_LIGHT_NUM];
//    PointLight       pointLights[POINT_LIGHT_NUM];
//    SpotLight        SpotLights[SPOT_LIGHT_NUM];
//
//    Vector3 ambientLight;   // アンビエントライト
//    float specPow;
//};

struct TitleLight
{
    //DirectionalLight directionalLights;
    SpotLight        SpotLights;
};

class Controller;
class Camera;
class OriginalShader;
class TitleScene :
    public BaseScene
{
public:
    TitleScene();
    ~TitleScene();

    // 初期化処理
    void Init() override;

    // 更新ステップ
    UniqueScene Update(UniqueScene scene) override;

    // 描画処理
    void DrawScreen(void) override;

    // 解放処理
    void Release(void) override;

    //シーンのIDを返す
    SceneID GetSceneID(void) override { return SceneID::Title; };

private:

    //コントローラ情報
    std::unique_ptr<Controller> controller_;

    //カメラ情報
    std::unique_ptr<Camera> camera_;

    TmxObj tmxObj_;

    Vector3 pos_;
    Vector2Int mousePos_;
    Vector2Int oldMousePos_;
    std::string modleStr_;

    std::unique_ptr<OriginalShader> originalShader_;

    TitleLight titleLight_;
    void InitController();
    std::string ClickHitTmxCol();
    void DrawTmxCollPosition();
    //先の読み込み
    void LoadingAhead();
    //バッファの初期化
    void InitCBuffer();
    //バッファのアップデート
    void UpdateCBuffer();
};
