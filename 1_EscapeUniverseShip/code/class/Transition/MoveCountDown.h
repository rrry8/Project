#pragma once
#include<string>
#include "TransitionScene.h"

class Player;
class Camera;
class Controller;

class MoveCountDown :
    public TransitionScene
{
public:

    MoveCountDown(double LimitTime, UniqueScene beforeScene, UniqueScene afterScene);

    MoveCountDown(double LimitTime,std::string stage,std::string nonstage,Vector2 pos, UniqueScene beforeScene, UniqueScene afterScene);
    ~MoveCountDown();
private:

    double LimitTime_;

    bool UpdateTransition(double delta) override;
    void MoveFont(bool flag);

    void DrawScreen() override;
    void PlayerDraw(void);


    //�v���C���[���
    std::unique_ptr<Player> player_;

    //�J�������
    std::unique_ptr<Camera> camera_;

    //�L�[���
    std::unique_ptr<Controller> controller_;

    double angle_;
    std::string stage_;
    std::string nonstage_;

    Vector2 pos_;
    Vector2 vel;

    double blink_;
};

