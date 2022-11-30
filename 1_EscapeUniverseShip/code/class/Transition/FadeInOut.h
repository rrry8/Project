#pragma once
#include "TransitionScene.h"
class FadeInOut :
    public TransitionScene
{
public:
    FadeInOut(double LimitTime, UniqueScene beforeScene, UniqueScene afterScene);
    ~FadeInOut();
private:
    double LimitTime_;

    bool UpdateTransition(double delta) override;

    void DrawScreen() override;
};

