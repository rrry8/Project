#pragma once
#include<memory>
#include"BaseScene.h"

class Stage;
class GameScene;

class SelectScene :
    public BaseScene
{
public:
    SelectScene();
    ~SelectScene();

    // ����������
    void Init() override;

    // �X�V�X�e�b�v
    UniqueScene Update(UniqueScene scene) override;

    // �`�揈��
    void DrawScreen(void) override;

    // �������
    void Release(void) override;

private:
    //�X�e�[�W���
    std::unique_ptr<Stage> stage_;

};

