#pragma once
#include "BaseScene.h"
class LoadScene :
    public BaseScene
{

public:
    LoadScene();
    ~LoadScene();
    
    // ����������
    void Init(void) override;

    // �X�V�X�e�b�v
    UniqueScene Update(UniqueScene scene) override;

    // �`�揈��
    void DrawScreen(void) override;

    // �������
    void Release(void) override;

    //�V�[����ID��Ԃ�
    SceneID GetSceneID(void) override { return SceneID::Load; };

private:
    bool IsLoadingEnd;
    int LoadVol;

    void Load();
    void UpdateProgress();
};

