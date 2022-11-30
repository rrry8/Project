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

    UniqueScene ownerScene_;

    //�t�H���g���
    double fluffyTime_;
    double con_;
    double plus_;

    SELECT nowSelect_;

    int gameBG_;
};

