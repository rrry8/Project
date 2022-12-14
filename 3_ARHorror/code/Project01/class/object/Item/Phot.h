#pragma once
#include "Item.h"

class Phot
    : public Item
{
public:
    Phot(Vector3 pos);
    ~Phot();

    //初期化
    void Init(void)override;

    //更新
    void Update(void)override;

    //描画
    void Draw(void)override;
    void Draw(bool cameraflag);
    //開放
    void Release(void)override;

    ItemID GetItemID(void)override { return ItemID::Phot; };

private:

protected:

};

