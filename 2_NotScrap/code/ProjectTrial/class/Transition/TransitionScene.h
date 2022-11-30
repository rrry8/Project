#pragma once
#include "../../class/Scene/BaseScene.h"

// �����I�ɃL���X�g����K�v������
enum class Transition
{
    CrossFade,
    FadeInOut,
    In,
    Out,
    Max
};


class TransitionScene :
    public BaseScene
{
public:
    TransitionScene(uniqueScene beforScene,uniqueScene afterScene);
    TransitionScene(uniqueScene afterScene);
    ~TransitionScene();
private:
    bool Init(void) override;
    uniqueScene Update(double delta, uniqueScene ownScene)override;
    virtual bool UpdateTransition(double delta) = 0;

protected:
    // �q���̃N���X�͐G��邪�O����͐G��Ȃ�
    Scene GetSceneID(void) override final{ return Scene::Transition; };

    uniqueScene beforScene_;
    uniqueScene afterScene_;

    double count;
};

