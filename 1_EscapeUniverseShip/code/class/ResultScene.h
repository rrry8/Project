#pragma once
#include "BaseScene.h"

class Controller;

enum class SELECET
{
    Retry,
    Select,
    MAX
};

class ResultScene :
    public BaseScene
{
public:
    ResultScene(UniqueScene scene,int id);
    ~ResultScene();

    // 初期化処理
    void Init(void) override;

    // 更新ステップ
    UniqueScene Update(UniqueScene scene) override;

    // 描画処理
    void DrawScreen(void) override;

    // 解放処理
    void Release(void) override;

private:
    //キー情報
    std::unique_ptr<Controller> controller_;

    SELECET nowSelect_;

    UniqueScene ownerScene_;

    //フォント情報
    double fluffyTime_;
    double con_;
    double plus_;

    //プレーヤー
    double playerAngle_;

    double noneTime_;

    int gameBG_;
};

