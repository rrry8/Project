#pragma once
#include "../Obj.h"
#include "../../common/Geometry.h"
#include "../../common/manager/ModelMng.h"

//ギミックアニメーション種類
enum class ANIM_GIMMICK {
    NONE,
    OPEN,   //開ける
    CLOSE,  //閉じる
};



//ギミックオブジェクト情報
struct GimmickObj
{
    std::string objName_;   //オブジェクト名
    Vector3 pos_;           //座標
    Vector3 rotate_;	//回転
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
    /// バイナリデータ読み込み
    /// </summary>
    /// <param name="格納したい配列"></param>
    /// <param name="バイナリデータ"></param>
    static void LoadBinData(std::vector<GimmickObj>& gList, const char* bData);
    /// <summary>
    /// バイナリデータ読み込み
    /// </summary>
    /// <param name="格納したい配列"></param>
    /// <param name="バイナリデータ"></param>
    static void LoadBinData(std::vector<GimmickObj>& gList, const char* bData,std::string objName);


    /// <summary>
    /// wstringに変換
    /// </summary>
    /// <param name="変換したい文字列(string)"></param>
    /// <returns></returns>
    static std::wstring ConvertToWstring(std::string& str);

    /// <summary>
    /// モデル読み込み
    /// </summary>
    /// <param name="配列"></param>
    /// <param name="指定するオブジェクト名"></param>
    /// <param name="モデルパス"></param>
    //static void LoadModel(std::vector<GimmickObj>& gList, std::wstring objName, const char* mPath);

    /// <summary>
    /// モデルの数を返す
    /// </summary>
    /// <param name="void"></param>
    /// <returns></returns>
    virtual int GetModelNumber(void) = 0;

    /// <summary>
    /// モデルハンドルの文字列を返す
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    virtual std::string GetModelString(void) = 0;

    /// <summary>
    /// アニメーションの再生フラグを返す
    /// </summary>
    /// <param name="void"></param>
    /// <returns></returns>
    virtual bool GetIsOpenFlg(void) { return isOpen_; }

    /// <summary>
    /// アニメーションの再生フラグを設定
    /// </summary>
    /// <param name="再生フラグ"></param>
    virtual void SetIsOpenFlg(bool flg) { isOpen_ = flg; }

    /// <summary>
    /// オブジェクト名を返す
    /// </summary>
    /// <param name="void"></param>
    /// <returns></returns>
    virtual std::string GetName(void) { return name_; }

    /// <summary>
    /// ロック状態を返す
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    virtual bool GetIsLockFlg(void) { return isLock_; }

    /// <summary>
    /// ロック状態をセット
    /// </summary>
    /// <param name="lockFlg"></param>
    virtual void SetIsLockFlg(bool lockFlg) { isLock_ = lockFlg; }

    /// <summary>
    /// アニメーション再生フラグ
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    virtual bool GetIsPlayFlg(void) { return isPlayFlg_; }

    /// <summary>
    /// インタラクトフラグセット
    /// </summary>
    /// <param name="再生フラグ"></param>
    virtual void SetInteractFlg(bool interactFlg) { interactFlg_ = interactFlg; }

    /// <summary>
    /// 当たった時の法線を設定
    /// </summary>
    /// <param name="hitNormal"></param>
    virtual void SetHitNormal(VECTOR hitNormal) { hitNormal_ = hitNormal; }

    /// <summary>
    /// 扉を開けるために必要な鍵IDを返す
    /// </summary>
    /// <returns></returns>
    virtual ItemID GetItemID() { return itemID_; };
    /// <summary>
    /// ダイアル番号を返す
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    virtual int GetDialNumber(void) { return dialNumber_; }

    /// <summary>
    /// 解除番号を返す
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    virtual std::string GetUnlockNumber(void) { return unlockNum_;  }
private:
protected:

    //複製数
    int num_ = 0;

    //拡大値
    Vector3 scale_ = { 1.0f ,1.0f,1.0f };

    //座標
    Vector3 pos_ = {0.0f,0.0f,0.0f};

    //回転
    Vector3 angle_ = { 0.0f,0.0f,0.0f };

    //開閉状態
    bool isOpen_ = true;

    //ロック状態
    bool isLock_ = true;

    //インタラクトフラグ
    bool interactFlg_ = false;

    //アニメーション再生フラグ
    bool isPlayFlg_ = false;

    //オブジェクト名
    std::string name_ ="";

    //ロックされている扉を開けるために必要な鍵のID
    ItemID itemID_ = ItemID(0);

    //モデルのファイルパス
    std::string path_ = "";

    //指定した方向から扉を開けるための法線
    float normal_ = 0.0f;

    //あたった時の法線
    VECTOR hitNormal_ = { 0.0f ,0.0f,0.0f };

    //ダイアルの数字
    int dialNumber_ = 0.0f;

    //
    std::string unlockNum_ = "0000";

};