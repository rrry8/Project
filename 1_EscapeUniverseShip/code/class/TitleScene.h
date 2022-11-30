#pragma once
#include "BaseScene.h"

class Controller;

class TitleScene :
    public BaseScene
{
public:
    TitleScene();
    ~TitleScene();

    // ����������
    void Init() override;

    // �X�V�X�e�b�v
    UniqueScene Update(UniqueScene scene) override;

    // �`�揈��
    void DrawScreen(void) override;

    // �������
    void Release(void) override;

private:
    void InitSound(void);

    //�L�[���
    std::unique_ptr<Controller> controller_;

    //�t�H���g���
    double fluffyTime_;
    double con_;
    double plus_;

    int posY_;
};

