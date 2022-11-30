#pragma once
#include "BaseScene.h"

class Controller;

class ClearScene :
    public BaseScene
{
public:
    ClearScene();
    ~ClearScene();

    // 初期化処理
    void Init(void) override;

    // 更新ステップ
    UniqueScene Update(UniqueScene scene) override;

    // 描画処理
    void DrawScreen(void) override;

    // 解放処理
    void Release(void) override;

private:

    //フォント情報
    double fluffyTime_;
    double con_;
    double plus_;

    int gameBG_;

    //キー情報
    std::unique_ptr<Controller> controller_;
};

