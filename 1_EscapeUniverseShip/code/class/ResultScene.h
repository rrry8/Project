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

    // ����������
    void Init(void) override;

    // �X�V�X�e�b�v
    UniqueScene Update(UniqueScene scene) override;

    // �`�揈��
    void DrawScreen(void) override;

    // �������
    void Release(void) override;

private:
    //�L�[���
    std::unique_ptr<Controller> controller_;

    SELECET nowSelect_;

    UniqueScene ownerScene_;

    //�t�H���g���
    double fluffyTime_;
    double con_;
    double plus_;

    //�v���[���[
    double playerAngle_;

    double noneTime_;

    int gameBG_;
};

