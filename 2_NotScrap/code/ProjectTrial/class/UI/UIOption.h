#pragma once
#include "UIBase.h"
#include<memory>
#include"../../tmx/TmxObj.h"
#include "../Common/Raycast.h"
#include "../Scene/OptionScene.h"

class Camera;

class UIOption :
    public UIBase
{
public:
    UIOption();
    ~UIOption();
private:

    bool Init(void)override;
    void Update(double delta)override;
    void DrawOwnScreen(double delta)override;
    Scene GetSceneID(void) override { return Scene::Option; };
    bool GetLoadingEndFlg(void)override;
    void SetIsNext(bool flg)override;
    TmxObj tmxObj_;
    Raycast raycast_;
    std::vector<char>stateVec_;
    rapidxml::xml_node<>* stateNode_;
    rapidxml::xml_document<> stateDoc;
    //カメラ情報
    std::unique_ptr<Camera> camera_;
    OptionState optionState_;//設定画面の今開いている項目
   // uniqueScene mOptionScene;
};
