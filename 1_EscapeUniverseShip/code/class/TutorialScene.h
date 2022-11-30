#pragma once
#include<memory>
#include "GameScene.h"
#include"../tmx/TmxObj.h"

class Controller;

class TutorialScene :
    public GameScene
{
public:
    TutorialScene();
    ~TutorialScene();

    // ����������
    void Init(void) override;

    // �X�V�X�e�b�v
    UniqueScene Update(UniqueScene scene) override;

    // �`�揈��
    void DrawScreen(void) override;
    void DrawStage(void);

    // �������
    void Release(void) override;

private:
    //Tmx���
    TmxObj tmxObj_;

    bool gravFlag_;

    int textNum_;

    //�`���[�g���A���̃e�L�X�g

    //�L�[���
    std::unique_ptr<Controller> controller_;
};

