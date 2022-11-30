#pragma once
#include "BaseScene.h"
#include <vector>
#include "../tmx/TmxObj.h"
#include "../common/Geometry.h"
#include "../common/SocketServer.h"

class Controller;
class MouseIcon;
class Item;
class OriginalShader;
struct InventoryObj;
enum class ItemID;
enum class ControllerType;

constexpr int WEIGHTS_NUM = 7;

//�u���[�\����
struct BlurData
{
    int renderTarget;   //�����_�����O�^�[�Q�b�g
    int vsHandle;       //���_�V�F�[�_�n���h��
    std::array<VERTEX3DSHADER, 4> verts{};  //Primitve�p�̒��_���
};

class InventoryScene :
    public BaseScene
{
public:
    InventoryScene(UniqueScene ownScene,int bgHandle,ControllerType type, const std::vector<InventoryObj>& inventoryList, const std::vector<int>& photo);
    ~InventoryScene();

    // ����������
    void Init(void) override;

    // �X�V�X�e�b�v
    UniqueScene Update(UniqueScene scene) override;
    // �`�揈��
    void DrawScreen(void) override;

    // �������
    void Release(void) override;

    //�V�[����ID��Ԃ�
    SceneID GetSceneID(void) override { return SceneID::Inventory; };

private:
    //�u���[�֌W������
    void InitBlur(void);

    //�u���[�̏d�݌v�Z
    void CalcBlurWeights(void);

    //�ʐ^�ƃ}�E�X�̈ʒu�̓����蔻��
    void CollMousePosAndPhoto(void);

    //�ڂ����`��
    void DrawBlur(int vs,int ps,int target,const std::array<VERTEX3DSHADER, 4>& verts);
    //�ʐ^�`��
    void DrawPhoto(std::string name,Vector2 pos,Vector2 imageSize);

    //�C���x���g��������
    void InitInventory(void);
    //�C���x���g���X�V
    void UpdateInventory(void);
    //�C���x���g���`��
    void DrawInventory(void);
    //�e�[�u���̏�����
    void InitTable(void);

    //�R���g���[�����
    std::unique_ptr<Controller> controller_;

    //�t���[���o�b�t�@��w�i�`��p
    std::array<VERTEX2D, 4> totalBlurVerts_;
    
    //�e�[�u��
    std::map<std::string , std::string> inventoryTable_;    //�C���x���g���̊O�g�̃e�[�u��
    std::map<std::string, std::string> optionTable_;        //����̃e�[�u��
    std::map<std::string, Vector3> itemTable_;              //�A�C�e���̘g�e�[�u��
    std::map<ItemID, std::string> explanationTable_;        //�����̃e�[�u��

    // �A�C�e�����X�g
    std::vector<InventoryObj> inventoryList_;

    //�ʐ^�֌W
    std::vector<int> photoVector_;          //�ʐ^�̃n���h��
    std::unique_ptr<OriginalShader> photo_; //�`��p�V�F�[�_�N���X
    float photoAddPos_;

    //�u���[���
    BlurData blurX_;    //��
    BlurData blurY_;    //�c

    //�O�̃V�[��(��ɃQ�[���V�[��)
    UniqueScene beforeScene_;

    //�w�i�摜(�O�̃V�[���̉��)
    int backGroundHandle_;

    //���C�������_�����O�^�[�Q�b�g
    int mainRenderTarget_;

    //�s�N�Z���V�F�[�_�n���h��
    int psBlurXandYHandle_; //���u���[�Əc�u���[�p
    int psPhotoHandle_;     //�ʐ^�p

    //�ڂ����̏d��
    float weights_[WEIGHTS_NUM];

    TmxObj tmxObj_;
    ItemID hitItemID_;
    bool isDrawTips_;
};
