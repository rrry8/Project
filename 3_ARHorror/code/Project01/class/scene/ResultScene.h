#pragma once
#include "BaseScene.h"
#include "../tmx/TmxObj.h"

class Controller;
enum class ControllerType;
class ResultScene :
    public BaseScene
{
public:
    ResultScene(ControllerType type);
    ~ResultScene();

    // ����������
    void Init(void) override;

    // �X�V�X�e�b�v
    UniqueScene Update(UniqueScene scene) override;

    // �`�揈��
    void DrawScreen(void) override;

    // �������
    void Release(void) override;

    //�V�[����ID��Ԃ�
    SceneID GetSceneID(void) override { return SceneID::Result; };
private:
    //�R���g���[�����
    std::unique_ptr<Controller> controller_;
    ControllerType ContorollerType_;

    std::map<std::string, std::string> resultList_;

    TmxObj tmxObj_;
};
