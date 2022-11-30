#pragma once
#include "Gimmick.h"


class DrawerFrame :
    public Gimmick
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="初期座標"></param>
    /// <param name="初期角度"></param>
    /// <param name="オブジェクト名"></param>
    /// <param name="複製数"></param>
    DrawerFrame(Vector3 pos,Vector3 angle,std::string name,int num);
    ~DrawerFrame();

    //初期化
    void Init(void)override;

    //更新
    void Update(void)override;

    //描画
    void Draw(void)override;

    //開放
    void Release(void)override;

    //モデルの個数を返す
    int GetModelNumber(void)override;

    //モデルのハンドルの文字列を返す
    std::string GetModelString(void)override;
private:
};

