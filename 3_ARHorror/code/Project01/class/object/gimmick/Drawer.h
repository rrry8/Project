#pragma once
#include <vector>
#include "Gimmick.h"

class AnimationController;

//���Έʒu
enum class RELATIVE_LOC {
    LEFT_UP,
    RIGHT_UP,
    LEFT_DOWN,
    RIGHT_DOWN
};

class Drawer :
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
    Drawer(Vector3 pos,Vector3 angle, std::string name, int num);
    ~Drawer();

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

    //���f���̃n���h���̕������Ԃ�
    std::string GetModelString(void)override;

private:
    //�A�j���[�V����
    std::unique_ptr<AnimationController> animController_;

    //�M�~�b�N�A�j���[�V�������
    ANIM_GIMMICK state_;

    //�A�j���[�V��������
    void Animation(void);

};

