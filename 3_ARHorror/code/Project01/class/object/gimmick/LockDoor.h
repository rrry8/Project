#pragma once
#include "Gimmick.h"

class AnimationController;

class LockDoor :
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
    /// <param name="�������邽�߂ɕK�v�ȃA�C�e����"></param>
    LockDoor(Vector3 pos, Vector3 angle, std::string name, int num,ItemID itemID);
    ~LockDoor();

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

    //�I�u�W�F�N�g�����`�F�b�N
    void CheckName(void);

};

