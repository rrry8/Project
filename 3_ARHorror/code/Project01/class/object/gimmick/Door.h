#pragma once
#include "Gimmick.h"
class AnimationController;

class Door :
    public Gimmick
{
public:
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    /// <param name="�������W"></param>
    /// <param name="�����p�x"></param>
    /// <param name="�I�u�W�F�N�g��"></param>
    /// <param name="������"></param>
    Door(Vector3 pos,Vector3 angle, std::string name, int num);
    ~Door();

    //������
    void Init(void)override;

    //�X�V
    void Update(void)override;

    //�`��
    void Draw(void)override;

    //�J��
    void Release(void)override;

    //���f���̌���Ԃ�
    int GetModelNumber(void)override;

    //���f���n���h���̕������Ԃ�
    std::string GetModelString(void)override;

private:
    //�A�j���[�V����
    std::unique_ptr<AnimationController> animController_;
    //�M�~�b�N�A�j���[�V�������
    ANIM_GIMMICK state_;
    //�A�j���[�V��������
    void Animation(void);
};

