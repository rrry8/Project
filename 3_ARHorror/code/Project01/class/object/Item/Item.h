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

//アイテムオブジェクト情報
struct ItemObj
{
    int path_;              //ファイルパス(.dat)
    std::string objName_;   //オブジェクト名
    Vector3 pos_;           //座標
    Vector3 rotate_;	//回転
    ItemObj(int path, std::string objName, const Vector3& pos, const Vector3& rotate) :
        path_(path),
        objName_(objName),
        pos_(pos),
        rotate_(rotate) {};
};

struct InventoryObj
{
    std::string modelString_;    //モデルハンドル
    int modelNum_;
    ItemID itemID_;   //アイテムID
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
   
    //バイナリデータ読み込み(格納したい配列、バイナリデータ、モデルのファイルパス)
    static void LoadBinData(std::vector<ItemObj>& iList, const char* bData);

    //string->wstringに変換(変換したい文字列)
    static std::wstring ConvertToWstring(std::string& str);

    /// <summary>
    /// モデルロード(配列、指定するオブジェクト名、モデルパス)
    /// </summary>
    /// <param name="iList"></param>
    /// <param name="objName"></param>
    /// <param name="mPath"></param>
    static void LoadModel(std::vector<ItemObj>& iList, std::wstring objName, const char* mPath);

    //static bool CheckObjName(std::vector<GimmickObj>& gList, std::wstring objName);
    
    /// <summary>
    /// モデルハンドルを返す
    /// </summary>
    /// <param name="void"></param>
    /// <returns></returns>
    virtual std::string GetModelString(void) { return modelString_; }
    virtual ItemID GetItemID(void) = 0;
    virtual int GetModelNumber(void) { return modelNum_; };
    virtual void SetPickUpFlg(bool flg) { pickupFlg_ = flg; }


    virtual bool GetPickUpFlg(void) { return pickupFlg_; }

private:
    //モデル制御
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

