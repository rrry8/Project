#pragma once
#include "BaseScene.h"
//#include <vector>
#include<memory>
#include "../Obj/Obj.h"
#include"../../tmx/TmxObj.h"
#include "btBulletDynamicsCommon.h"
#include "../Obj/Gimmick/Gimmick.h"
#include "../Obj/Stage.h"

static constexpr int PLAYER_MAX_NUM = 30;			//��������v���C���[�̍ő吔

static constexpr int PLAYER_OFFSET = 40;			//��������v���C���[�̍ő吔

class Camera;
class Crane;
class Scrap;
//struct OriginalShader;
struct StackPlayer
{
    int handle = 0;
    btRigidBody* body;
    Vector3 pos_ = Vector3Zero;
    bool isAlive =false;
    int time = 0;
};

class TitleScene :
    public BaseScene
{
public:
    TitleScene();
    ~TitleScene();
private:

    //������
    bool Init(void)override;
    //�X�V
    uniqueScene Update(double delta, uniqueScene ownScene)override;
    //�`��
    void DrawOwnScreen(double delta)override;
    //�V�[��ID�擾
    Scene GetSceneID(void) override { return Scene::Title; };

    //���f���̃��[�h
    bool LoadModel(void);
    //�v���C���[����
    void CreatePlyaers(void);
    //�v���C���[�X�V
    void UpdatePlyaers(void);
    //�v���C���[�j��
    void DestroyPlayers(void);
    //�v���C���[�̃A�j���[�V����
    void AnimationPlayer(StackPlayer& player_, std::unique_ptr<AnimationController>& animController_, rapidxml::xml_node<>* node);

    void PlayerMesh(void);
    //�J�������
    std::unique_ptr<Camera> camera_;
    //�N���[�����(��)
    std::unique_ptr<Crane> crane_;
    //�X�N���b�v
    std::list <std::unique_ptr<Scrap>> scrap_;
    //
    StackPlayer players_[PLAYER_MAX_NUM];

    //���t���[����
    int frame_;

    btVector3 posw_;

    StackPlayer player_;
    StackPlayer player2_;
    bool CreateFlag_;
    //�N���[���̈ړ��J�n
    bool MoveStartCraneFlag_;
    //�A�j���[�V����
    std::unique_ptr<AnimationController> animController_;
    std::unique_ptr<AnimationController> animController2_;

    std::vector<char>TitleVec_;
    rapidxml::xml_node<>* AnimationPlayerNode_;
    rapidxml::xml_document<> TitleDoc;
    rapidxml::xml_node<>* AnimationPlayerNode2_;

    int ShadowMapHandle_;

   /* MV1_REF_POLYGONLIST RefMesh_;
    VERTEX3D Vertex[1000];*/

    //std::unique_ptr<OriginalShader> originalShader_;
    TextureMap texmap;
};

