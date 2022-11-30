#pragma once
#include "BaseScene.h"

enum class SELECT
{
    Resume,
    Select,
    MAX
};

class Controller;

class PauseScene :
    public BaseScene
{
public:
    PauseScene(UniqueScene scene, int id);
    ~PauseScene();

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

    UniqueScene ownerScene_;

    //フォント情報
    double fluffyTime_;
    double con_;
    double plus_;

    SELECT nowSelect_;

    int gameBG_;
};

