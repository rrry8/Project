#pragma once
#include "../Obj.h"
#include"../../common/Geometry.h"
#include"../../common/Transform.h"
#include"../../common/Capsule.h"

class Stamina;
class Controller;
class MinCamera;
struct GimmickObj;
class Gimmick;
struct ItemObj;
struct InventoryObj;
class Item;
enum class ItemID;
enum class ControllerType;
class HandLight;

enum class State
{
    Walk,   //����
    Fall,   //����
    Max
};


class Player :
    public Obj
{
public:
    Player(ControllerType type, const std::vector<std::shared_ptr<Gimmick>>& gList, const std::vector<std::shared_ptr<Item>>& iList);
    ~Player();

    //������
    void Init(void) override;
    //�j��
    void Release(void) override;
    //�X�V
    void Update(void) override;
    void Update(Vector3 cameraAngle,Vector3 targetPos);
    //�`��
    void Draw(void) override;
    void CameraDraw(void);
    //�ʒu���擾
    Vector3 GetPosition(void);
    bool GetIsOnCamera();
    bool GetIsGameClear();

    //�_�b�V�������Ă��邩�Ԃ�
    bool GetDashFlag(void);

    //�J�����̃����_�����O�^�[�Q�b�g�ݒ�
    void SetMinCameraRenderTarget(int screenID);

    //�t���b�V���̓����蔻��
    bool CheckHitFlash(Vector3 targetPos);

    //�J�����̎ʐ^����Ԃ�
    std::vector<int>& GetMinCameraPhotoVector(void);

    //�����蔻��p�̕ϐ���Ԃ�
    std::vector<Collider>& GetColliders(void);
    std::vector<InventoryObj>& GetInventoryList(void);
    Capsule GetCapsule(void);

private:
    //�ړ�����
    void MovePosition(Vector3 cameraAngle);
    //�΂߈ړ��̋�����������
    bool CheckDiagonalMove(void);

    //�X�e�[�W�Ƃ̓����蔻��
    void CollisionStage(void);
    //�M�~�b�N�Ƃ̓����蔻��
    void CollisionGimmick(void);

    void SlideLinePosition(void);

    void HitCapsuleAndGoal(void);

    void AddInventory(Vector3 targetPos);

    void InteractGimmick(Vector3 targetPos);
   
    void GimmickEvent(Vector3 targetPos);
    void DialCountUpdate(std::shared_ptr<Gimmick> &gimmick,int modelNum,char numchar);
    //�ʒu
    Vector3 pos_;

    //��]
    Quaternion quaRot_;

    VECTOR PlayerRot_;
    //���
    std::unique_ptr<Controller> controller_; //�R���g���[��   
    std::unique_ptr<Stamina> stamina_;       //�X�^�~�i

    bool pickUpFlag_;

    //�����蔻��p
    std::vector<Collider> colliders_;   //�}�b�v�֌W
    Capsule capsule_;  //�v���C���[�̃J�v�Z��
    std::unique_ptr<MinCamera> minCamera_;  //�v���C���[�̃J�v�Z��
    //���
    State state_;
    float fallGravity_;

    int collisionHandle_;
    std::vector<std::shared_ptr<Gimmick>> gimmickList_;
    std::vector<std::shared_ptr<Item>> itemList_;

    std::vector<InventoryObj> inventoryList_;
    int dial_[4];
    //�_�C�����ԍ�
    std::list<int> dial2_{0,0,0,0};
    //�_�C�����ԍ�(������)
    std::string dialNumStr_="0000";
    bool dashFlag_;

    std::unique_ptr<HandLight> handLight_;
    bool isGameClear_;
    bool isHitGoal_;
};
