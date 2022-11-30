#pragma once
#include "BaseScene.h"
#include "../tmx/TmxObj.h"

class Controller;
enum class ControllerType;
class ResultScene :
    public BaseScene
{
public:
    ResultScene(ControllerType type);
    ~ResultScene();

    // 初期化処理
    void Init(void) override;

    // 更新ステップ
    UniqueScene Update(UniqueScene scene) override;

    // 描画処理
    void DrawScreen(void) override;

    // 解放処理
    void Release(void) override;

    //シーンのIDを返す
    SceneID GetSceneID(void) override { return SceneID::Result; };
private:
    //コントローラ情報
    std::unique_ptr<Controller> controller_;
    ControllerType ContorollerType_;

    std::map<std::string, std::string> resultList_;

    TmxObj tmxObj_;
};
