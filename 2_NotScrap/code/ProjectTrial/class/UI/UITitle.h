#pragma once
#include "UIBase.h"
#include<memory>
#include"../../tmx/TmxObj.h"
#include "../Common/Raycast.h"

#define ITEM_LEFT_BOX (1550)
#define ITEM_LEFT_SINGLE (1718)
#define ITEM_LEFT_MULTI (1718)
#define ITEM_LEFT_OPTION (1718)
#define ITEM_LEFT_QUIT (1718)

#define ItemMax (4)

class Camera;


class UITitle :
    public UIBase
{
public:
    UITitle();
    ~UITitle();
private:

    bool Init(void)override;
    void Update(double delta)override;
    void DrawOwnScreen(double delta)override;
    Scene GetSceneID(void) override { return Scene::Title; };
    bool GetLoadingEndFlg(void)override;
    void SetIsNext(bool flg)override;

    TmxObj tmxObj_;
    Raycast raycast_;

    std::vector<char>stateVec_;
    rapidxml::xml_node<>* stateNode_;
    rapidxml::xml_document<> stateDoc;
    //�J�������
    std::unique_ptr<Camera> camera_;
    int modelH;//�^�C�g���̊Ŕ̃��f���n���h��
    bool hitItem[ItemMax];//�����������ڂ�true
    int BrightVol;//�_�ł̖��邳
    bool BrightFlg;//�_�Ńt���O
    double exRate;//�g�k�̗�
    bool exFlg;//�΂̘g�̊g�k�t���O
    bool exFlg_;//�����̊g�k�t���O
    double moveVol[ItemMax];//���ڂ̘g�̗΂̕����𓮂������߂̕ϐ�
    int fastOffset_[ItemMax];//���ڂ����ɃX���C�h���邽�߂̕ϐ�
    bool IsNext_;//�^�C�g���ōŏ��ɉ������͂��ꂽ��true
    bool IsNextScene_;//�^�C�g���ŉ������ڂ��I�����ꂽ�ꍇ��true
};


