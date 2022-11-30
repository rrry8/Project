#pragma once
#include<vector>
#include<chrono>
#include<string>
#include "BaseScene.h"
#include "../common/Geometry.h"
#include "../common/Light.h"
#include "../tmx/TmxObj.h"

constexpr int DIRECTION_LIGHT_NUM = 2;  // �f�B���N�V�������C�g�̐�
constexpr int POINT_LIGHT_NUM = 3;   // �|�C���g���C�g�̐�

struct GameLight
{
    // �f�B���N�V�������C�g�p�̃����o
    DirectionlLight directionLights[DIRECTION_LIGHT_NUM];
    PointLight       pointLights[POINT_LIGHT_NUM];
    SpotLight        spotLights;

    Vector3 ambientLight;   // �A���r�G���g���C�g
    float specPow;
};
struct DOF
{
    Vector3 Focus;
    float pad0;
    Vector3 Area;
    float pad1;
};
enum class StatusUI
{
    Stop,
    MoveIn,
    MoveOut,
    Max
};
struct MoveUI
{
    Vector2 Destination;
    Vector2 Pos;
    StatusUI IsMove;
    int StopCnt;
};
class Camera;
class Controller;
class Stage;
class Player;
class Enemy;

class CBufferMng;
class SocketServer;
class Goal;
class Mirror;
class OriginalShader;

class GameScene :
    public BaseScene
{
public:
	GameScene();
	~GameScene();

    // ����������
    void Init(void) override;

    // �X�V�X�e�b�v
    UniqueScene Update(UniqueScene scene) override;

    // �`�揈��
    void DrawScreen(void) override;

    // �������
    void Release(void) override;

    //�V�[����ID��Ԃ�
    SceneID GetSceneID(void) override { return SceneID::Game; };

private:
    //�R���g���[�����
    std::unique_ptr<Controller> controller_;

    //�X�e�[�W���
    std::unique_ptr<Stage> stage_;

    //�J�������
    std::unique_ptr<Camera> camera_;

    //�v���C���[���
    std::unique_ptr<Player> player_;

    //�G���
    std::unique_ptr<Enemy> enemy_;

    //�S�[�����
    std::unique_ptr<Goal> goal_;

    //��
    std::unique_ptr<Mirror> mirror_;


    std::unique_ptr<OriginalShader> originalShader_;
    int screenCameraID_;
    int GaussScreenCameraID_;
   
    //�C���x���g���̔w�i�n���h��
    int inventoryBackGroundHandle_;
    bool onoff_;

    TmxObj tmxObj_;     //�Փ˔���ptmx

    // ��ʊE�[�x�����p�̃p�����[�^��������
    float DepthOfField_InterpSize_;
    float DepthOfField_Focus_;
    float DepthOfField_FocusSize_;

    // ��Ԕ͈͂Ƃڂ��������͈̔͂̍��v����
    float DepthOfField_TotalSize_;

    DOF Dof_;

    GameLight light_;

    template<typename T>
    void DebugKeyMove(unsigned int keyUp, unsigned int keyDown, int value, T& body);

    void InitCBuffer(void);
    void InitLights(void);
    void InitUI(void);
    void UpdateCBuffer(void);

    void UIUpdate(void);
    void UIDraw(void);
    void DrawCollCenter(Vector2 CenterPos, std::string imageStr);

    std::map<std::string, MoveUI> UIVariableList_;
};

