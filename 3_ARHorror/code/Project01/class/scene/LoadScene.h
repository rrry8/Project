#pragma once
#include "BaseScene.h"
class LoadScene :
    public BaseScene
{

public:
    LoadScene();
    ~LoadScene();
    
    // 初期化処理
    void Init(void) override;

    // 更新ステップ
    UniqueScene Update(UniqueScene scene) override;

    // 描画処理
    void DrawScreen(void) override;

    // 解放処理
    void Release(void) override;

    //シーンのIDを返す
    SceneID GetSceneID(void) override { return SceneID::Load; };

private:
    bool IsLoadingEnd;
    int LoadVol;

    void Load();
    void UpdateProgress();
};

