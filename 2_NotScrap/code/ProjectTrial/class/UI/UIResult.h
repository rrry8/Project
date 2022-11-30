#pragma once
#include "UIBase.h"
#include<memory>
#include"../../tmx/TmxObj.h"
#include "../Common/Raycast.h"

class Camera;


class UIResult :
    public UIBase
{
public:
    UIResult();
    ~UIResult();
private:

    bool Init(void)override;
    void Update(double delta)override;
    void DrawOwnScreen(double delta)override;
    void DrawShader(float x, float y,int shader, int image, int norm);
    Scene GetSceneID(void) override { return Scene::Result; };
    bool GetLoadingEndFlg(void)override;
    void SetIsNext(bool flg)override;
    TmxObj tmxObj_;
    Raycast raycast_;

    std::vector<char>stateVec_;
    rapidxml::xml_node<>* stateNode_;
    rapidxml::xml_document<> stateDoc;
    //�J�������
    std::unique_ptr<Camera> camera_;

    double exRate;
    bool exFlg_;

    //�V�F�[�_
    int shaderHandle_;                      //�n���h��
    int cbuff_;                             //�萔�o�b�t�@
    float* writer_;                         //�n�����
};