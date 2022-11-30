#pragma once
#include<list>
#include "../Obj.h"
#include"../../common/Transform.h"
#include "../../common/Geometry.h"
#include"../../common/Capsule.h"

class AnimationController;
class Dijkstra;

struct NodeConnection;

//�A�j���[�V����
enum EnemyAnimation
{
    Idle,               //����
    Run,                //����
    Damage,             //�Ђ��
    FemaleDynamicPose,  //����
    Walk,               //����
    Max
};

class Enemy :
    public Obj
{
public:
    Enemy();
    ~Enemy();

    //������
    void Init(void) override;
    //�j��
    void Release(void) override;
    //�X�V
    void Update(void) override;
    void Update(Capsule capsule,const Vector3 playerPos,bool hitFlash);
    //�`��
    void Draw(void) override;

    Vector3 GetPosition(void);

    bool GetHitPlayerFlag(void);

private:
    //�v���C���[��T�����鏈��
    void SearchPlayer(const Vector3 playerPos);

    //�v���C���[�T��(ture:����/false:�������Ă��Ȃ�)
    bool Search(const Vector3 playerPos);

    //�v���C���[��ǐՂ��鏈��
    void HuntPlayer(void);

    //�_�C�N�X�g������
    void DijkstraProcess(Vector3 playerPos);

    //�Փˏ���
    bool CheckHitPlayer(void);

    //�t���b�V�����󂯂��Ƃ��̏���
    void DamageFlash(bool hitFlash);

    //�t���b�V����Ԃ���񕜂��鏈��
    void RecoverFlash(void);

    //�X�^�����ԉ�
    void RecoverStanTime(void);

    //�X�^�����ԒZ�k
    void ReduceStanTime(void);

    //���f������
    Transform transform_;

    //�A�j���[�V�����R���g���[��
    std::unique_ptr<AnimationController> animController_;

    //�ǐՊ֌W
    bool huntFlag_;         //�v���C���[�����t���O
    double stepHuntTime_;   //�ǐՎ���

    //�ړI�n
    Vector3 targetPos_;
    
    //��]
    Quaternion targetRot_;  //�ړI�̕���
    double stepRotTime_;    //��]����

    //�X�^���֌W
    bool stanFlag_;         //�X�^���t���O
    float nowStanTime_;      //�G�̃X�^������(�X�^������قǒZ���Ȃ�)
    double stepTime_;   //�X�^������

    //�_�C�N�X�g�����
    std::unique_ptr<Dijkstra> dijkstra_;

    //�v���C���[�̃J�v�Z�����
    Capsule playerCapsule_;

    //�v���C���[�Փ˃t���O
    bool hitPlayerFlag_;
};

