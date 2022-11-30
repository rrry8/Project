#pragma once
#include "BaseScene.h"
#include "../tmx/TmxObj.h"
#include "../common/Geometry.h"
#include "../common/Light.h"

//constexpr int DIRECTION_LIGHT_NUM = 1;  // �f�B���N�V�������C�g�̐�
//constexpr int POINT_LIGHT_NUM = 1;   // �|�C���g���C�g�̐�
//constexpr int SPOT_LIGHT_NUM = 1;  // �f�B���N�V�������C�g�̐�
//
//struct Light
//{
//    // �f�B���N�V�������C�g�p�̃����o
//    DirectionalLight directionLights[DIRECTION_LIGHT_NUM];
//    PointLight       pointLights[POINT_LIGHT_NUM];
//    SpotLight        SpotLights[SPOT_LIGHT_NUM];
//
//    Vector3 ambientLight;   // �A���r�G���g���C�g
//    float specPow;
//};

struct TitleLight
{
    //DirectionalLight directionalLights;
    SpotLight        SpotLights;
};

class Controller;
class Camera;
class OriginalShader;
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

    //�V�[����ID��Ԃ�
    SceneID GetSceneID(void) override { return SceneID::Title; };

private:

    //�R���g���[�����
    std::unique_ptr<Controller> controller_;

    //�J�������
    std::unique_ptr<Camera> camera_;

    TmxObj tmxObj_;

    Vector3 pos_;
    Vector2Int mousePos_;
    Vector2Int oldMousePos_;
    std::string modleStr_;

    std::unique_ptr<OriginalShader> originalShader_;

    TitleLight titleLight_;
    void InitController();
    std::string ClickHitTmxCol();
    void DrawTmxCollPosition();
    //��̓ǂݍ���
    void LoadingAhead();
    //�o�b�t�@�̏�����
    void InitCBuffer();
    //�o�b�t�@�̃A�b�v�f�[�g
    void UpdateCBuffer();
};
