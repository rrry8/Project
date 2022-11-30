#pragma once
#include "BaseScene.h"

class Controller;

class ClearScene :
    public BaseScene
{
public:
    ClearScene();
    ~ClearScene();

    // ����������
    void Init(void) override;

    // �X�V�X�e�b�v
    UniqueScene Update(UniqueScene scene) override;

    // �`�揈��
    void DrawScreen(void) override;

    // �������
    void Release(void) override;

private:

    //�t�H���g���
    double fluffyTime_;
    double con_;
    double plus_;

    int gameBG_;

    //�L�[���
    std::unique_ptr<Controller> controller_;
};

