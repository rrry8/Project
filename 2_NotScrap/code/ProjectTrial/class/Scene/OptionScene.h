#pragma once
#include "BaseScene.h"
#include "../../tmx/TmxObj.h"

constexpr auto SUB_ITEM_LEFT = 658;
constexpr auto SUB_ITEM_LEFT_VOL = 960;
constexpr auto KEYNAME_LEFT = 958;


enum class OptionState
{
    KeyConfig,
    Volume,
    Tips,
    Quit,
    Max
};

class OptionScene :
    public BaseScene
{
public:
    OptionScene(uniqueScene& oldscene);
    ~OptionScene();
    OptionState GetOptionState();
private:

    bool Init(void)override;
    uniqueScene Update(double delta, uniqueScene ownScene)override;
    void DrawOwnScreen(double delta)override;
    Scene GetSceneID(void) override { return Scene::Option; };
    void KeyChange(int subItem);
    
    Vector2Int MousePos;//�}�E�X�̍��W
    uniqueScene mOldScene;//�ЂƂO�̃V�[���i�[�p
    TmxObj tmxObj_;
    bool keyChangeMode_;//�L�[�ύX��Ԃɓ����Ă�����true
    bool keyResetFlg;//�L�[�����Z�b�g���ꂽ��true
    int cnt;//�x���\���p�J�E���g
    OptionState state_;//�ݒ��ʂ̏��
    int vol;//���ʊi�[�p�ϐ�
};

