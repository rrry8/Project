#pragma once
#include "BaseScene.h"
#include "../../tmx/TmxObj.h"

enum class MenuState
{
    TO_GAME,
    RESTART,
    OPTION,
    TO_SELECT,
    MAX
};


class MenuScene :
    public BaseScene
{
public:
    MenuScene(uniqueScene& oldscene);
    ~MenuScene();

private:

    bool Init(void)override;
    uniqueScene Update(double delta, uniqueScene ownScene)override;
    void DrawOwnScreen(double delta)override;
    Scene GetSceneID(void) override { return Scene::Menu; };

    Vector2Int MousePos;//�}�E�X�̍��W
    uniqueScene mOldScene;//�ЂƂO�̃V�[���i�[�p
    TmxObj tmxObj_;
    MenuState state_;//�ݒ��ʂ̏��
};