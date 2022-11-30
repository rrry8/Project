#pragma once
#include<vector>
#include<chrono>
#include<string>
#include "BaseScene.h"
#include"../tmx/TmxObj.h"

class Player;
class Camera;
class Drum;
class CheckPoint;

class GameScene :
    public BaseScene
{
public:
	GameScene();
    GameScene(std::string map);
	~GameScene();

    // ����������
    void Init(void) override;

    // �X�V�X�e�b�v
    UniqueScene Update(UniqueScene scene) override;

    // �`�揈��
    void DrawScreen(void) override;
    void DrawStage(void);

    // �������
    void Release(void) override;
private:


    //Tmx���
    TmxObj tmxObj_;
protected:
    //�̂�����`��
    virtual bool DrawSaw(int gid, int x, int y, Vector2 tilesize, Vector2 offset);

    //�v���C���[���
    std::unique_ptr<Player> player_;

    //�J�������
    std::unique_ptr<Camera> camera_;

    //�h�����ʏ��
    std::unique_ptr<Drum> drum_;

    //���ԃ|�C���g���
    std::shared_ptr<CheckPoint> check_;

    //�̂�����̉�]
    double angle_;

    //�̂�����̃n���h��
    int sawImage_;

    //�^�C���̃n���h��
    int tileImage_;

    //�J�ڎ��̔w�i�n���h��
    int gameBG_ = MakeScreen(screenSizeX_, screenSizeY_, true);
};

