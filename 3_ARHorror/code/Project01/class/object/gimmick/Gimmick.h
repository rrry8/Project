#pragma once
#include "../Obj.h"
#include "../../common/Geometry.h"
#include "../../common/manager/ModelMng.h"

//�M�~�b�N�A�j���[�V�������
enum class ANIM_GIMMICK {
    NONE,
    OPEN,   //�J����
    CLOSE,  //����
};



//�M�~�b�N�I�u�W�F�N�g���
struct GimmickObj
{
    std::string objName_;   //�I�u�W�F�N�g��
    Vector3 pos_;           //���W
    Vector3 rotate_;	//��]
    GimmickObj(std::string objName,const Vector3& pos, const Vector3& rotate) :
        objName_(objName),
        pos_(pos),
        rotate_(rotate) {};
};

enum class ItemID;


class Gimmick :
    public Obj
{
public:
    Gimmick();
    ~Gimmick();
    /// <summary>
    /// �o�C�i���f�[�^�ǂݍ���
    /// </summary>
    /// <param name="�i�[�������z��"></param>
    /// <param name="�o�C�i���f�[�^"></param>
    static void LoadBinData(std::vector<GimmickObj>& gList, const char* bData);
    /// <summary>
    /// �o�C�i���f�[�^�ǂݍ���
    /// </summary>
    /// <param name="�i�[�������z��"></param>
    /// <param name="�o�C�i���f�[�^"></param>
    static void LoadBinData(std::vector<GimmickObj>& gList, const char* bData,std::string objName);


    /// <summary>
    /// wstring�ɕϊ�
    /// </summary>
    /// <param name="�ϊ�������������(string)"></param>
    /// <returns></returns>
    static std::wstring ConvertToWstring(std::string& str);

    /// <summary>
    /// ���f���ǂݍ���
    /// </summary>
    /// <param name="�z��"></param>
    /// <param name="�w�肷��I�u�W�F�N�g��"></param>
    /// <param name="���f���p�X"></param>
    //static void LoadModel(std::vector<GimmickObj>& gList, std::wstring objName, const char* mPath);

    /// <summary>
    /// ���f���̐���Ԃ�
    /// </summary>
    /// <param name="void"></param>
    /// <returns></returns>
    virtual int GetModelNumber(void) = 0;

    /// <summary>
    /// ���f���n���h���̕������Ԃ�
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    virtual std::string GetModelString(void) = 0;

    /// <summary>
    /// �A�j���[�V�����̍Đ��t���O��Ԃ�
    /// </summary>
    /// <param name="void"></param>
    /// <returns></returns>
    virtual bool GetIsOpenFlg(void) { return isOpen_; }

    /// <summary>
    /// �A�j���[�V�����̍Đ��t���O��ݒ�
    /// </summary>
    /// <param name="�Đ��t���O"></param>
    virtual void SetIsOpenFlg(bool flg) { isOpen_ = flg; }

    /// <summary>
    /// �I�u�W�F�N�g����Ԃ�
    /// </summary>
    /// <param name="void"></param>
    /// <returns></returns>
    virtual std::string GetName(void) { return name_; }

    /// <summary>
    /// ���b�N��Ԃ�Ԃ�
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    virtual bool GetIsLockFlg(void) { return isLock_; }

    /// <summary>
    /// ���b�N��Ԃ��Z�b�g
    /// </summary>
    /// <param name="lockFlg"></param>
    virtual void SetIsLockFlg(bool lockFlg) { isLock_ = lockFlg; }

    /// <summary>
    /// �A�j���[�V�����Đ��t���O
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    virtual bool GetIsPlayFlg(void) { return isPlayFlg_; }

    /// <summary>
    /// �C���^���N�g�t���O�Z�b�g
    /// </summary>
    /// <param name="�Đ��t���O"></param>
    virtual void SetInteractFlg(bool interactFlg) { interactFlg_ = interactFlg; }

    /// <summary>
    /// �����������̖@����ݒ�
    /// </summary>
    /// <param name="hitNormal"></param>
    virtual void SetHitNormal(VECTOR hitNormal) { hitNormal_ = hitNormal; }

    /// <summary>
    /// �����J���邽�߂ɕK�v�Ȍ�ID��Ԃ�
    /// </summary>
    /// <returns></returns>
    virtual ItemID GetItemID() { return itemID_; };
    /// <summary>
    /// �_�C�A���ԍ���Ԃ�
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    virtual int GetDialNumber(void) { return dialNumber_; }

    /// <summary>
    /// �����ԍ���Ԃ�
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    virtual std::string GetUnlockNumber(void) { return unlockNum_;  }
private:
protected:

    //������
    int num_ = 0;

    //�g��l
    Vector3 scale_ = { 1.0f ,1.0f,1.0f };

    //���W
    Vector3 pos_ = {0.0f,0.0f,0.0f};

    //��]
    Vector3 angle_ = { 0.0f,0.0f,0.0f };

    //�J���
    bool isOpen_ = true;

    //���b�N���
    bool isLock_ = true;

    //�C���^���N�g�t���O
    bool interactFlg_ = false;

    //�A�j���[�V�����Đ��t���O
    bool isPlayFlg_ = false;

    //�I�u�W�F�N�g��
    std::string name_ ="";

    //���b�N����Ă�������J���邽�߂ɕK�v�Ȍ���ID
    ItemID itemID_ = ItemID(0);

    //���f���̃t�@�C���p�X
    std::string path_ = "";

    //�w�肵��������������J���邽�߂̖@��
    float normal_ = 0.0f;

    //�����������̖@��
    VECTOR hitNormal_ = { 0.0f ,0.0f,0.0f };

    //�_�C�A���̐���
    int dialNumber_ = 0.0f;

    //
    std::string unlockNum_ = "0000";

};