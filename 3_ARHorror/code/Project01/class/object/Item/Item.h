#pragma once

#include "../Obj.h"
#include "../../common/Geometry.h"

enum class ItemID
{
    Non,
    Key1,
    Key2,
    Key3,
    Key4,
    Key5,
    Phot,
    Max
};

//�A�C�e���I�u�W�F�N�g���
struct ItemObj
{
    int path_;              //�t�@�C���p�X(.dat)
    std::string objName_;   //�I�u�W�F�N�g��
    Vector3 pos_;           //���W
    Vector3 rotate_;	//��]
    ItemObj(int path, std::string objName, const Vector3& pos, const Vector3& rotate) :
        path_(path),
        objName_(objName),
        pos_(pos),
        rotate_(rotate) {};
};

struct InventoryObj
{
    std::string modelString_;    //���f���n���h��
    int modelNum_;
    ItemID itemID_;   //�A�C�e��ID
    Vector3 pos_;
    Vector3 scale_;
    std::string inventoryNum_;
};

class Item
    :public Obj
{
public:
    Item();
    ~Item();
    virtual void Draw(bool cameraflag) {};
   
    //�o�C�i���f�[�^�ǂݍ���(�i�[�������z��A�o�C�i���f�[�^�A���f���̃t�@�C���p�X)
    static void LoadBinData(std::vector<ItemObj>& iList, const char* bData);

    //string->wstring�ɕϊ�(�ϊ�������������)
    static std::wstring ConvertToWstring(std::string& str);

    /// <summary>
    /// ���f�����[�h(�z��A�w�肷��I�u�W�F�N�g���A���f���p�X)
    /// </summary>
    /// <param name="iList"></param>
    /// <param name="objName"></param>
    /// <param name="mPath"></param>
    static void LoadModel(std::vector<ItemObj>& iList, std::wstring objName, const char* mPath);

    //static bool CheckObjName(std::vector<GimmickObj>& gList, std::wstring objName);
    
    /// <summary>
    /// ���f���n���h����Ԃ�
    /// </summary>
    /// <param name="void"></param>
    /// <returns></returns>
    virtual std::string GetModelString(void) { return modelString_; }
    virtual ItemID GetItemID(void) = 0;
    virtual int GetModelNumber(void) { return modelNum_; };
    virtual void SetPickUpFlg(bool flg) { pickupFlg_ = flg; }


    virtual bool GetPickUpFlg(void) { return pickupFlg_; }

private:
    //���f������
    //Transform transform_;

protected:
    //int modelH_ = 0;
    std::string modelString_;
    int modelNum_;
    Vector3 pos_;
    Vector3 size_;
    float scale_ = 0.0f;
    bool pickupFlg_ = false;
    ItemID itemId_;
};

